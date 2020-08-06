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

  D3D12_COMMAND_QUEUE_DESC queueDesc = {};
  queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
  queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
  if (!Succeeded(device_->CreateCommandQueue(&queueDesc,
                                             IID_PPV_ARGS(&command_queue_)))) {
    return false;
  }
  command_queue_->SetName(L"CommandQueue");

  DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
  swapChainDesc.BufferCount = FRAME_COUNT;
  swapChainDesc.Width = width;
  swapChainDesc.Height = height;
  swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
  swapChainDesc.SampleDesc.Count = 1;

  ComPtr<IDXGISwapChain1> swapChain;
  if (!Succeeded(adapter_.GetFactory()->CreateSwapChainForHwnd(
          command_queue_.Get(), hwnd, &swapChainDesc, nullptr, nullptr,
          &swapChain))) {
    return false;
  }

  if (!Succeeded(adapter_.GetFactory()->MakeWindowAssociation(
          hwnd, DXGI_MWA_NO_ALT_ENTER))) {
    return false;
  }

  if (!Succeeded(swapChain.As(&swap_chain_))) {
    return false;
  }
  frame_index_ = swap_chain_->GetCurrentBackBufferIndex();

  if (!Succeeded(device_->CreateCommandAllocator(
          D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
          IID_PPV_ARGS(&command_allocator_)))) {
    return false;
  }
  command_allocator_->SetName(L"CommandAllocator");

  viewport_ = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(width),
                               static_cast<float>(height));
  scissor_rect_ = CD3DX12_RECT(0, 0, width, height);

  if (!heap_manager_.Init(*this)) {
    return false;
  }

  for (UINT i = 0; i < FRAME_COUNT; i++) {
    if (!Succeeded(
            swap_chain_->GetBuffer(i, IID_PPV_ARGS(&render_targets_[i])))) {
      return false;
    }
    device_->CreateRenderTargetView(
        render_targets_[i].Get(), nullptr,
        heap_manager_.GetRtvHeap()->GetHandle().cpu_handle_);
    std::wstringstream wss;
    wss << L"RenderTarget [" << i << L"]";
    render_targets_[i]->SetName(wss.str().c_str());
  }

  ComPtr<ID3D12GraphicsCommandList> command_list;
  if (!Succeeded(device_->CreateCommandList(
          0, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
          command_allocator_.Get(), nullptr, IID_PPV_ARGS(&command_list)))) {
    return false;
  }

  if (!Succeeded(command_list->Close())) {
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
    fence_event_ = CreateEvent(nullptr, false, false, nullptr);
    if (!fence_event_) {
      return false;
    }

    const UINT64 fence_to_wait_for = fence_value_;
    if (!Succeeded(command_queue_->Signal(fence_.Get(), fence_to_wait_for))) {
      return false;
    }
    fence_value_++;

    if (!Succeeded(
            fence_->SetEventOnCompletion(fence_to_wait_for, fence_event_))) {
      return false;
    }
    WaitForSingleObject(fence_event_, INFINITE);
  }

  return true;
}

bool DirectXDevice::Prepare() {
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

  return true;
}

bool DirectXDevice::Present() {
  current_resource_->Ready();
  current_resource_->command_lists_[PRE_COMMAND_LIST_ID]
      .GetCommandList()
      ->ResourceBarrier(
          1, &CD3DX12_RESOURCE_BARRIER::Transition(
                 render_targets_[frame_index_].Get(),
                 D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT,
                 D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET));
  if (!Succeeded(
          current_resource_->command_lists_[PRE_COMMAND_LIST_ID].Close())) {
    return false;
  }

  constexpr float clear_color[] = {0.2f, 0.2f, 0.2f, 1.0f};
  D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle =
      heap_manager_.GetRtvHeap()->GetForceHandle(frame_index_).cpu_handle_;
  current_resource_->command_lists_[MID_COMMAND_LIST_ID]
      .GetCommandList()
      ->ClearRenderTargetView(rtv_handle, clear_color, 0, nullptr);
  if (!Succeeded(
          current_resource_->command_lists_[MID_COMMAND_LIST_ID].Close())) {
    return false;
  }

  current_resource_->command_lists_[POST_COMMAND_LIST_ID]
      .GetCommandList()
      ->ResourceBarrier(
          1, &CD3DX12_RESOURCE_BARRIER::Transition(
                 render_targets_[frame_index_].Get(),
                 D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET,
                 D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT));
  if (!Succeeded(
          current_resource_->command_lists_[POST_COMMAND_LIST_ID].Close())) {
    return false;
  }

  command_queue_->ExecuteCommandLists(
      static_cast<u32>(current_resource_->batch_submit_.size()),
      current_resource_->batch_submit_.data());
  swap_chain_->Present(1, 0);
  frame_index_ = swap_chain_->GetCurrentBackBufferIndex();

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

  if (last_completed_fence < fence) {
    if (!Succeeded(fence_->SetEventOnCompletion(fence, fence_event_))) {
      return;
    }
    WaitForSingleObject(fence_event_, INFINITE);
  }
  CloseHandle(fence_event_);

  for (UINT i = 0; i < FRAME_COUNT; i++) {
    resources_[i].Destroy();
  }
}

bool FrameResource::Init(ID3D12Device* device) { return true; }

bool FrameResource::AddCommandList(ID3D12Device* device) {
  auto& cmd_list = command_lists_.emplace_back();
  if (!cmd_list.Init(device, nullptr)) {
    return false;
  }

  cmd_list.Close();
  batch_submit_.emplace_back(cmd_list.GetCommandList());
  return true;
}

bool FrameResource::Ready() {
  for (auto&& cmd_list : command_lists_) {
    if (!cmd_list.Reset()) {
      return false;
    }
  }

  return true;
}

void FrameResource::Destroy() {}

}  // namespace device
}  // namespace directx
}  // namespace legend