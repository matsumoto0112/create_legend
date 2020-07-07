#include "src/directx/directx12_device.h"

namespace legend {
namespace directx {
//コンストラクタ
DirectX12Device::DirectX12Device()
    : target_window_(),
      render_target_screen_size_(math::IntVector2::kZeroVector),
      current_resource_state_(
          D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON) {}

//デストラクタ
DirectX12Device::~DirectX12Device() { WaitForGPU(); }

//初期化
bool DirectX12Device::Init(std::shared_ptr<window::Window> target_window) {
  MY_ASSERTION(target_window != nullptr, L"");

  this->target_window_ = target_window;
  render_target_screen_size_ = math::IntVector2::kZeroVector;

  if (auto win = target_window_.lock(); win) {
    render_target_screen_size_ = win->GetScreenSize();
  }

  if (!adapter_.Init(device::DeviceOptionFlags::TEARING)) {
    return false;
  }
  if (!CreateDevice()) return false;

  MY_LOG(L"Create Device finished");
  return true;
}

bool DirectX12Device::InitAfter() {
  if (FAILED(command_list_->Close())) {
    MY_LOG(L"ID3D12GraphicsCommandList::Close failed");
    return false;
  }
  ID3D12CommandList* command_lists[] = {command_list_.Get()};
  command_queue_->ExecuteCommandLists(ARRAYSIZE(command_lists), command_lists);

  MoveToNextFrame();

  return true;
}

bool DirectX12Device::Prepare() {
  if (FAILED(command_allocator_[frame_index_]->Reset())) {
    return false;
  }
  if (FAILED(command_list_->Reset(command_allocator_[frame_index_].Get(),
                                  nullptr))) {
    return false;
  }

  CD3DX12_VIEWPORT viewport(0.0f, 0.0f,
                            static_cast<float>(render_target_screen_size_.x),
                            static_cast<float>(render_target_screen_size_.y));
  CD3DX12_RECT scissor_rect(0, 0,
                            static_cast<long>(render_target_screen_size_.x),
                            static_cast<long>(render_target_screen_size_.y));
  command_list_->RSSetViewports(1, &viewport);
  command_list_->RSSetScissorRects(1, &scissor_rect);

  swap_chain_.SetBackBuffer(*this);

  heap_manager_.BeginFrame();
  return true;
}

bool DirectX12Device::Present() {
  swap_chain_.DrawEnd(*this);

  if (FAILED(command_list_->Close())) {
    return false;
  }

  ID3D12CommandList* command_lists[] = {command_list_.Get()};
  command_queue_->ExecuteCommandLists(ARRAYSIZE(command_lists), command_lists);

  swap_chain_.Present();

  MoveToNextFrame();

  return true;
}

void DirectX12Device::SetBackBuffer() { swap_chain_.SetBackBuffer(*this); }

void DirectX12Device::WaitForGPU() noexcept {
  if (command_queue_ && fence_ && fence_event_.IsValid()) {
    u64 value = fence_values_[frame_index_];
    if (SUCCEEDED(command_queue_->Signal(fence_.Get(), value))) {
      if (SUCCEEDED(fence_->SetEventOnCompletion(value, fence_event_.Get()))) {
        WaitForSingleObjectEx(fence_event_.Get(), INFINITE, FALSE);
        fence_values_[frame_index_]++;
      }
    }
  }
}

DescriptorHandle DirectX12Device::GetHandle(DescriptorHeapType heap_type) {
  switch (heap_type) {
    case legend::directx::DescriptorHeapType::CBV_SRV_UAV:
      return heap_manager_.GetCbvSrvUavHeap().GetHandle();
    case legend::directx::DescriptorHeapType::RTV:
      return heap_manager_.GetRtvHeap().GetHandle();
    case legend::directx::DescriptorHeapType::DSV:
      return heap_manager_.GetDsvHeap().GetHandle();
    default:
      MY_ASSERTION(false, L"未定義のheap_typeが選択されました。");
      break;
  }
  return DescriptorHandle{};
}

void DirectX12Device::SetToGlobalHeap(u32 register_num,
                                      ResourceType resource_type,
                                      const DescriptorHandle& handle) {
  heap_manager_.SetHandleToLocalHeap(register_num, resource_type,
                                     handle.cpu_handle_);
}

bool DirectX12Device::CreateDevice() {
  if (FAILED(D3D12CreateDevice(adapter_.GetAdapter(),
                               device::defines::MIN_FEATURE_LEVEL,
                               IID_PPV_ARGS(&device_)))) {
    return false;
  }

  D3D12_COMMAND_QUEUE_DESC queue_desc = {};
  queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
  queue_desc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
  if (FAILED(device_->CreateCommandQueue(&queue_desc,
                                         IID_PPV_ARGS(&command_queue_)))) {
    MY_LOG(L"CreateCommandQueue failed");
    return false;
  }

  if (!heap_manager_.Init(*this)) return false;

  if (!swap_chain_.Init(adapter_, *target_window_.lock(), command_queue_.Get(),
                        DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM)) {
    return false;
  }
  if (!swap_chain_.CreateRenderTarget(*this)) {
    return false;
  }

  for (unsigned int i = 0; i < FRAME_COUNT; i++) {
    if (FAILED(device_->CreateCommandAllocator(
            D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
            IID_PPV_ARGS(&command_allocator_[i])))) {
      MY_LOG(L"CreateCommandAllocator %d failed", i);
      return false;
    }
  }

  if (FAILED(device_->CreateCommandList(
          0, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
          command_allocator_[0].Get(), nullptr,
          IID_PPV_ARGS(&command_list_)))) {
    MY_LOG(L"CreateCommandList failed");
    return false;
  }
  if (FAILED(device_->CreateFence(fence_values_[frame_index_],
                                  D3D12_FENCE_FLAGS::D3D12_FENCE_FLAG_NONE,
                                  IID_PPV_ARGS(&fence_)))) {
    MY_LOG(L"CreateFence failed");
    return false;
  }

  fence_values_[frame_index_]++;
  fence_event_.Attach(CreateEventW(nullptr, false, false, nullptr));
  if (!fence_event_.IsValid()) {
    MY_LOG(L"CreateEventW failed");
    return false;
  }

  return true;
}  // namespace legend

bool DirectX12Device::MoveToNextFrame() {
  const u64 fence_value = fence_values_[frame_index_];
  if (FAILED(command_queue_->Signal(fence_.Get(), fence_value))) {
    return false;
  }

  swap_chain_.UpdateCurrentFrameIndex();
  frame_index_ = swap_chain_.GetCurrentFrameIndex();

  if (fence_->GetCompletedValue() < fence_values_[frame_index_]) {
    if (FAILED(fence_->SetEventOnCompletion(fence_values_[frame_index_],
                                            fence_event_.Get()))) {
      return false;
    }
    WaitForSingleObjectEx(fence_event_.Get(), INFINITE, false);
  }
  fence_values_[frame_index_] = fence_value + 1;

  return true;
}

}  // namespace directx
}  // namespace legend
