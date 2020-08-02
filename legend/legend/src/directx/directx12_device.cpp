#include "src/directx/directx12_device.h"

#include "src/directx/directx_helper.h"

namespace legend {
namespace directx {
//コンストラクタ
DirectX12Device::DirectX12Device()
    : target_window_(),
      render_target_screen_size_(math::IntVector2::kZeroVector) {}

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
  if (!directx_helper::Succeeded(
          command_lists_[frame_index_].GetCommandAllocator()->Reset())) {
    return false;
  }
  if (!directx_helper::Succeeded(
          command_lists_[frame_index_].GetCommandList()->Reset(
              command_lists_[frame_index_].GetCommandAllocator(), nullptr))) {
    return false;
  }

  render_resource_manager_.SetRenderTargetID(
      render_target::RenderTargetID::BACK_BUFFER);
  render_resource_manager_.SetRenderTargetsToCommandList(*this);
  render_resource_manager_.ClearCurrentRenderTarget(*this);

  heap_manager_.BeginNewFrame();
  heap_manager_.SetHeapToCommandList(*this);
  default_root_signature_->SetGraphicsCommandList(*this);
  return true;
}

bool DirectX12Device::Present() {
  render_resource_manager_.DrawEnd(*this);

  ExecuteCommandList();

  if (!render_resource_manager_.Present()) {
    return false;
  }

  if (!MoveToNextFrame()) {
    return false;
  }

  return true;
}

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

bool DirectX12Device::ExecuteCommandList() {
  if (!directx_helper::Succeeded(
          command_lists_[frame_index_].GetCommandList()->Close())) {
    return false;
  }

  ID3D12CommandList* command_lists[] = {
      command_lists_[frame_index_].GetCommandList()};
  command_queue_->ExecuteCommandLists(ARRAYSIZE(command_lists), command_lists);

  // WaitForGPU();
  return true;
}

bool DirectX12Device::CreateDevice() {
  if (!directx_helper::Succeeded(D3D12CreateDevice(
          adapter_.GetAdapter(), device::defines::MIN_FEATURE_LEVEL,
          IID_PPV_ARGS(&device_)))) {
    return false;
  }

  //コマンドキューの作成
  D3D12_COMMAND_QUEUE_DESC queue_desc = {};
  queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
  queue_desc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
  if (!directx_helper::Succeeded(device_->CreateCommandQueue(
          &queue_desc, IID_PPV_ARGS(&command_queue_)))) {
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

  //フレーム枚数分のアロケータ作成
  for (unsigned int i = 0; i < FRAME_COUNT; i++) {
    if (!command_lists_[i].Init(device_.Get(), command_queue_.Get())) {
      return false;
    }
  }

  if (!render_resource_manager_.Init(
          *this, adapter_, *target_window_.lock(), FRAME_COUNT,
          DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, command_queue_.Get())) {
    return false;
  }

  //同期用のフェンス作成
  if (!directx_helper::Succeeded(device_->CreateFence(
          fence_values_[frame_index_], D3D12_FENCE_FLAGS::D3D12_FENCE_FLAG_NONE,
          IID_PPV_ARGS(&fence_)))) {
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
  if (!directx_helper::Succeeded(
          command_queue_->Signal(fence_.Get(), fence_value))) {
    return false;
  }

  frame_index_ = render_resource_manager_.GetCurrentFrameIndex();

  if (fence_->GetCompletedValue() < fence_values_[frame_index_]) {
    if (!directx_helper::Succeeded(fence_->SetEventOnCompletion(
            fence_values_[frame_index_], fence_event_.Get()))) {
      return false;
    }
    WaitForSingleObjectEx(fence_event_.Get(), INFINITE, false);
  }
  fence_values_[frame_index_] = fence_value + 1;

  return true;
}

}  // namespace directx
}  // namespace legend
