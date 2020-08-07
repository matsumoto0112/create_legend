#include "src/directx/device/directx_device.h"

#include "src/directx/directx_helper.h"

namespace legend {
namespace directx {
namespace device {
using namespace directx::directx_helper;

//コンストラクタ
DirectXDevice::DirectXDevice() {}

//デストラクタ
DirectXDevice::~DirectXDevice() {}

//初期化
bool DirectXDevice::Init(u32 width, u32 height, HWND hwnd) {
  //デバイスに設定するオプションを定義する
  const DeviceOptionFlags flags = [](bool use_warp_device) {
    DeviceOptionFlags flags = DeviceOptionFlags::TEARING;
    if (use_warp_device) {
      flags |= DeviceOptionFlags::USE_WARP_DEVICE;
    }
    return flags;
  }(USE_WARP_DEVICE);

  //デバイス回りの初期化をまずやる
  if (!adapter_.Init(flags)) {
    return false;
  }

  if (Failed(D3D12CreateDevice(adapter_.GetAdapter(),
                               directx::device::defines::MIN_FEATURE_LEVEL,
                               IID_PPV_ARGS(&device_)))) {
    return false;
  }

  heap_manager_ = std::make_unique<descriptor_heap::HeapManager>();
  if (!heap_manager_->Init(*this)) {
    return false;
  }

  //コマンドキューを作成する
  D3D12_COMMAND_QUEUE_DESC queue_desc = {};
  queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
  queue_desc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
  if (Failed(device_->CreateCommandQueue(&queue_desc,
                                         IID_PPV_ARGS(&command_queue_)))) {
    return false;
  }
  command_queue_->SetName(L"CommandQueue");

  //描画リソース管理オブジェクトを作成する
  if (!render_resource_manager_.Init(*this, adapter_, FRAME_COUNT, width,
                                     height,
                                     DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
                                     hwnd, command_queue_.Get())) {
    return false;
  }
  //フレームインデックスをスワップチェインから取得する
  //今後、毎描画後に更新していく
  frame_index_ = render_resource_manager_.GetCurrentBackBufferIndex();

  for (u32 i = 0; i < FRAME_COUNT; i++) {
    FrameResource& resource = resources_[i];
    if (!resource.Init(*this)) {
      return false;
    }
  }

  if (Failed(device_->CreateFence(fence_value_,
                                  D3D12_FENCE_FLAGS::D3D12_FENCE_FLAG_NONE,
                                  IID_PPV_ARGS(&fence_)))) {
    return false;
  }

  fence_value_++;
  return true;
}

//描画準備
bool DirectXDevice::Prepare() {
  heap_manager_->BeginFrame();

  const UINT64 last_completed_fence = fence_->GetCompletedValue();
  current_resource_ = &resources_[frame_index_];

  if (current_resource_->fence_value_ > last_completed_fence) {
    HANDLE event_handle = CreateEvent(nullptr, false, false, nullptr);
    if (!event_handle) {
      return false;
    }
    if (Failed(fence_->SetEventOnCompletion(current_resource_->fence_value_,
                                            event_handle))) {
      return false;
    }
    WaitForSingleObject(event_handle, INFINITE);
    CloseHandle(event_handle);
  }

  current_resource_->Ready();

  render_resource_manager_.SetRenderTargets(
      current_resource_->GetCommandList(),
      directx::render_target::RenderTargetID::BACK_BUFFER, true,
      directx::render_target::DepthStencilTargetID::NONE, false);

  return true;
}

//描画処理
bool DirectXDevice::Present() {
  render_resource_manager_.DrawEnd(current_resource_->GetCommandList());

  if (Failed(current_resource_->GetCommandList().Close())) {
    return false;
  }

  ID3D12CommandList* const command_list[] = {
      current_resource_->GetCommandList().GetCommandList()};
  command_queue_->ExecuteCommandLists(_countof(command_list), command_list);
  if (!render_resource_manager_.Present()) {
    return false;
  }
  frame_index_ = render_resource_manager_.GetCurrentBackBufferIndex();

  //描画コマンドを送信する
  current_resource_->fence_value_ = fence_value_;
  if (Failed(command_queue_->Signal(fence_.Get(), fence_value_))) {
    return false;
  }
  fence_value_++;

  return true;
}

//デバイスの破棄
void DirectXDevice::Destroy() {
  const UINT64 fence = fence_value_;
  const UINT64 last_completed_fence = fence_->GetCompletedValue();

  //コマンドキューの実行
  if (Failed(command_queue_->Signal(fence_.Get(), fence_value_))) {
    return;
  }
  fence_value_++;

  //コマンドの実行を待機する
  HANDLE fence_event = CreateEvent(nullptr, false, false, nullptr);
  if (last_completed_fence < fence) {
    if (Failed(fence_->SetEventOnCompletion(fence, fence_event))) {
      return;
    }
    WaitForSingleObject(fence_event, INFINITE);
  }
  CloseHandle(fence_event);

  for (UINT i = 0; i < FRAME_COUNT; i++) {
    resources_[i].Destroy();
  }
}

//レンダーターゲットハンドルを取得する
descriptor_heap::DescriptorHandle DirectXDevice::GetRTVHandle() {
  return heap_manager_->GetRtvHeap()->GetHandle();
}

//デプス・ステンシルハンドルを取得する
descriptor_heap::DescriptorHandle DirectXDevice::GetDSVHandle() {
  return heap_manager_->GetDsvHeap()->GetHandle();
}

//ハンドルを登録する
void DirectXDevice::RegisterHandle(u32 register_num, shader::ResourceType type,
                                   descriptor_heap::DescriptorHandle handle) {
  heap_manager_->RegisterHandle(register_num, type, handle);
}

//ローカルハンドルを取得する
descriptor_heap::DescriptorHandle DirectXDevice::GetLocalHandle(
    descriptor_heap::heap_parameter::LocalHeapID heap_id) {
  return heap_manager_->GetLocalHeap(heap_id);
}

//コマンドの実行
void DirectXDevice::ExecuteCommandList(
    const std::vector<CommandList>& command_lists) {
  const u32 num = static_cast<u32>(command_lists.size());
  std::vector<ID3D12CommandList*> cmd_lists(num);
  for (u32 i = 0; i < num; i++) {
    cmd_lists[i] = command_lists[i].GetCommandList();
  }

  command_queue_->ExecuteCommandLists(num, cmd_lists.data());
}

//コマンドの実行を待機する
void DirectXDevice::WaitExecute() {
  HANDLE fence_event = CreateEvent(nullptr, false, false, nullptr);
  if (!fence_event) {
    return;
  }

  const UINT64 fence_to_wait_for = fence_value_;
  if (FAILED(command_queue_->Signal(fence_.Get(), fence_to_wait_for))) {
    return;
  }
  fence_value_++;

  if (FAILED(fence_->SetEventOnCompletion(fence_to_wait_for, fence_event))) {
    return;
  }
  WaitForSingleObject(fence_event, INFINITE);
  CloseHandle(fence_event);
}

}  // namespace device
}  // namespace directx
}  // namespace legend