#include "src/directx/device/directx_device.h"

#include "src/directx/directx_helper.h"

namespace legend {
namespace directx {
namespace device {
using namespace directx::directx_helper;

DirectXDevice::DirectXDevice() {}

DirectXDevice::~DirectXDevice() {}

bool DirectXDevice::Init(u32 width, u32 height, HWND hwnd) {
  directx::device::DeviceOptionFlags flags =
      directx::device::DeviceOptionFlags::TEARING;
  if (USE_WARP_DEVICE) {
    flags |= directx::device::DeviceOptionFlags::USE_WARP_DEVICE;
  }

  if (!adapter_.Init(flags)) {
    return false;
  }
  if (FAILED(D3D12CreateDevice(adapter_.GetAdapter(),
                               directx::device::defines::MIN_FEATURE_LEVEL,
                               IID_PPV_ARGS(&device_)))) {
    return false;
  }

  heap_manager_ = std::make_unique<descriptor_heap::HeapManager>();
  if (!heap_manager_->Init(*this)) {
    return false;
  }

  D3D12_COMMAND_QUEUE_DESC queueDesc = {};
  queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
  queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
  if (!Succeeded(device_->CreateCommandQueue(&queueDesc,
                                             IID_PPV_ARGS(&command_queue_)))) {
    return false;
  }

  command_queue_->SetName(L"CommandQueue");

  if (!rt_manager_.Init(*this, adapter_, FRAME_COUNT, width, height,
                        DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, hwnd,
                        command_queue_.Get())) {
    return false;
  }
  frame_index_ = rt_manager_.GetCurrentBackBufferIndex();

  ComPtr<ID3D12CommandAllocator> command_allocator_;
  if (!Succeeded(device_->CreateCommandAllocator(
          D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
          IID_PPV_ARGS(&command_allocator_)))) {
    return false;
  }
  command_allocator_->SetName(L"CommandAllocator");

  ComPtr<ID3D12GraphicsCommandList> command_list;
  if (Failed(device_->CreateCommandList(
          0, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
          command_allocator_.Get(), nullptr, IID_PPV_ARGS(&command_list)))) {
    return false;
  }

  if (Failed(command_list->Close())) {
    return false;
  }

  ID3D12CommandList* command_lists[] = {command_list.Get()};
  command_queue_->ExecuteCommandLists(_countof(command_lists), command_lists);

  for (UINT i = 0; i < FRAME_COUNT; i++) {
    FrameResource& resource = resources_[i];
    if (!resource.Init(device_.Get())) {
      return false;
    }
    for (UINT j = 0; j < CommandListID::COUNT; j++) {
      if (!resource.AddCommandList(device_.Get())) {
        return false;
      }
    }
  }

  {
    if (!Succeeded(device_->CreateFence(
            fence_value_, D3D12_FENCE_FLAGS::D3D12_FENCE_FLAG_NONE,
            IID_PPV_ARGS(&fence_)))) {
      return false;
    }

    fence_value_++;
    HANDLE fence_event = CreateEvent(nullptr, false, false, nullptr);
    if (!fence_event) {
      return false;
    }

    const UINT64 fence_to_wait_for = fence_value_;
    if (!Succeeded(command_queue_->Signal(fence_.Get(), fence_to_wait_for))) {
      return false;
    }
    fence_value_++;
    if (!Succeeded(
            fence_->SetEventOnCompletion(fence_to_wait_for, fence_event))) {
      return false;
    }
    WaitForSingleObject(fence_event, INFINITE);
  }

  return true;
}

bool DirectXDevice::Prepare() {
  heap_manager_->BeginFrame();

  const UINT64 last_completed_fence = fence_->GetCompletedValue();
  current_resource_ = &resources_[frame_index_];

  if (current_resource_->fence_value_ > last_completed_fence) {
    HANDLE event_handle = CreateEvent(nullptr, false, false, nullptr);
    if (!event_handle) {
      return false;
    }
    if (!Succeeded(fence_->SetEventOnCompletion(current_resource_->fence_value_,
                                                event_handle))) {
      return false;
    }
    WaitForSingleObject(event_handle, INFINITE);
    CloseHandle(event_handle);
  }

  current_resource_->Ready();

  rt_manager_.SetRenderTargetID(
      directx::render_target::RenderTargetID::BACK_BUFFER);
  rt_manager_.SetRenderTargets(
      current_resource_->command_lists_[MID_COMMAND_LIST_ID]);
  rt_manager_.ClearCurrentRenderTarget(
      current_resource_->command_lists_[MID_COMMAND_LIST_ID]);

  return true;
}

bool DirectXDevice::Present() {
  rt_manager_.DrawEnd(current_resource_->command_lists_[POST_COMMAND_LIST_ID]);

  if (!Succeeded(
          current_resource_->command_lists_[PRE_COMMAND_LIST_ID].Close())) {
    return false;
  }
  if (!Succeeded(
          current_resource_->command_lists_[MID_COMMAND_LIST_ID].Close())) {
    return false;
  }
  if (!Succeeded(
          current_resource_->command_lists_[POST_COMMAND_LIST_ID].Close())) {
    return false;
  }

  command_queue_->ExecuteCommandLists(
      static_cast<u32>(current_resource_->batch_submit_.size()),
      current_resource_->batch_submit_.data());
  if (!rt_manager_.Present()) {
    return false;
  }
  frame_index_ = rt_manager_.GetCurrentBackBufferIndex();

  current_resource_->fence_value_ = fence_value_;
  command_queue_->Signal(fence_.Get(), fence_value_);
  fence_value_++;

  return true;
}

void DirectXDevice::Destroy() {
  const UINT64 fence = fence_value_;
  const UINT64 last_completed_fence = fence_->GetCompletedValue();

  if (!Succeeded(command_queue_->Signal(fence_.Get(), fence_value_))) {
    return;
  }
  fence_value_++;

  HANDLE fence_event = CreateEvent(nullptr, false, false, nullptr);
  if (last_completed_fence < fence) {
    if (!Succeeded(fence_->SetEventOnCompletion(fence, fence_event))) {
      return;
    }
    WaitForSingleObject(fence_event, INFINITE);
  }
  CloseHandle(fence_event);

  for (UINT i = 0; i < FRAME_COUNT; i++) {
    resources_[i].Destroy();
  }
}

descriptor_heap::DescriptorHandle DirectXDevice::GetRTVHandle() {
  return heap_manager_->GetRtvHeap()->GetHandle();
}

descriptor_heap::DescriptorHandle DirectXDevice::GetDSVHandle() {
  return heap_manager_->GetDsvHeap()->GetHandle();
}

void DirectXDevice::RegisterHandle(u32 register_num, shader::ResourceType type,
                                   descriptor_heap::DescriptorHandle handle) {
  heap_manager_->RegisterHandle(register_num, type, handle);
}

descriptor_heap::DescriptorHandle DirectXDevice::GetLocalHandle(
    descriptor_heap::heap_parameter::LocalHeapID heap_id) {
  return heap_manager_->GetLocalHeap(heap_id);
}

void DirectXDevice::WaitCommandList() {
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

void DirectXDevice::ExecuteCommandList(
    const std::vector<CommandList>& command_lists) {
  const u32 num = static_cast<u32>(command_lists.size());
  std::vector<ID3D12CommandList*> cmd_lists(num);
  for (u32 i = 0; i < num; i++) {
    cmd_lists[i] = command_lists[i].GetCommandList();
  }

  command_queue_->ExecuteCommandLists(num, cmd_lists.data());
}

}  // namespace device
}  // namespace directx
}  // namespace legend