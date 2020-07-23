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
bool DirectX12Device::Init(std::weak_ptr<window::Window> target_window) {
  MY_ASSERTION(!target_window.expired(), L"");

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
  std::array<ID3D12CommandList*, FRAME_COUNT> command_lists;
  for (u32 i = 0; i < FRAME_COUNT; i++) {
    if (!command_lists_[i].Close()) {
      return false;
    }
    command_lists[i] = command_lists_[i].GetCommandList();
  }

  command_queue_->ExecuteCommandLists(static_cast<u32>(command_lists.size()),
                                      command_lists.data());

  MoveToNextFrame();

  return true;
}

bool DirectX12Device::Prepare() {
  if (FAILED(command_lists_[frame_index_].GetCommandAllocator()->Reset())) {
    return false;
  }
  if (FAILED(command_lists_[frame_index_].GetCommandList()->Reset(
          command_lists_[frame_index_].GetCommandAllocator(), nullptr))) {
    return false;
  }

  CD3DX12_VIEWPORT viewport(0.0f, 0.0f,
                            static_cast<float>(render_target_screen_size_.x),
                            static_cast<float>(render_target_screen_size_.y));
  CD3DX12_RECT scissor_rect(0, 0,
                            static_cast<long>(render_target_screen_size_.x),
                            static_cast<long>(render_target_screen_size_.y));
  command_lists_[frame_index_].GetCommandList()->RSSetViewports(1, &viewport);
  command_lists_[frame_index_].GetCommandList()->RSSetScissorRects(
      1, &scissor_rect);

  render_resource_manager_.SetRenderTarget(0);
  render_resource_manager_.SetRenderTargetsToCommandList(*this);
  render_resource_manager_.ClearCurrentRenderTarget(*this);

  heap_manager_.BeginNewFrame();
  heap_manager_.SetHeapToCommandList(*this);
  default_root_signature_->SetGraphicsCommandList(*this);
  return true;
}

bool DirectX12Device::Present() {
  render_resource_manager_.DrawEnd(*this);
  swap_chain_.DrawEnd(*this);

  if (FAILED(command_lists_[frame_index_].GetCommandList()->Close())) {
    return false;
  }

  ID3D12CommandList* command_lists[] = {
      command_lists_[frame_index_].GetCommandList()};
  command_queue_->ExecuteCommandLists(ARRAYSIZE(command_lists), command_lists);

  WaitForGPU();
  if (!swap_chain_.Present()) {
    return false;
  }

  if (!MoveToNextFrame()) {
    return false;
  }

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

void DirectX12Device::SetToGlobalHeap(
    u32 register_num, ResourceType resource_type,
    const descriptor_heap::DescriptorHandle& handle) {
  heap_manager_.SetHandleToLocalHeap(register_num, resource_type,
                                     handle.cpu_handle_);
}

descriptor_heap::DescriptorHandle DirectX12Device::GetBackBufferHandle() const {
  return swap_chain_.GetRenderTarget().GetHandle();
}

void DirectX12Device::ClearBackBufferTarget(IDirectXAccessor& accessor) {
  swap_chain_.ClearBackBuffer(accessor);
}

DXGI_FORMAT DirectX12Device::GetBackBufferFormat() const {
  return swap_chain_.GetRenderTarget().GetFormat();
}

void DirectX12Device::SetBackBuffer(IDirectXAccessor& accessor) {
  swap_chain_.SetBackBuffer(accessor);
}

bool DirectX12Device::CreateDevice() {
  if (FAILED(D3D12CreateDevice(adapter_.GetAdapter(),
                               device::defines::MIN_FEATURE_LEVEL,
                               IID_PPV_ARGS(&device_)))) {
    return false;
  }

  //コマンドキューの作成
  D3D12_COMMAND_QUEUE_DESC queue_desc = {};
  queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
  queue_desc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
  if (FAILED(device_->CreateCommandQueue(&queue_desc,
                                         IID_PPV_ARGS(&command_queue_)))) {
    MY_LOG(L"CreateCommandQueue failed");
    return false;
  }

  //ヒープ管理機能の作成
  if (!heap_manager_.Init(*this)) {
    return false;
  }

  //ルートシグネチャの作成
  default_root_signature_ = std::make_shared<shader::RootSignature>();
  if (!default_root_signature_->Init(*this, L"DefaultRootSignature")) {
    return false;
  }

  //スワップチェインの作成
  if (!swap_chain_.Init(*this, adapter_, *target_window_.lock(),
                        command_queue_.Get(),
                        DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM)) {
    return false;
  }

  //フレーム枚数分のアロケータ作成
  for (unsigned int i = 0; i < FRAME_COUNT; i++) {
    if (!command_lists_[i].Init(device_.Get(), command_queue_.Get())) {
      return false;
    }
  }

  if (!render_resource_manager_.Init()) {
    return false;
  }

  //同期用のフェンス作成
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
}
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
