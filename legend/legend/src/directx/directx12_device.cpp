#include "src/directx/directx12_device.h"

#include <d3dcompiler.h>

#include <filesystem>
#include <iostream>
#include <random>

#include "src/directx/buffer/index_buffer.h"
#include "src/directx/shader/graphics_pipeline_state.h"
#include "src/directx/shader/pixel_shader.h"
#include "src/directx/shader/vertex_shader.h"
#include "src/directx/vertex.h"
#include "src/math/vector_3.h"
#include "src/util/debug.h"
#include "src/util/typedef.h"

namespace {}  // namespace

namespace legend {
namespace directx {
//コンストラクタ
DirectX12Device::DirectX12Device()
    : target_window_(),
      render_target_screen_size_(math::IntVector2::kZeroVector),
      device_(nullptr),
      current_resource_state_(
          D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON) {}

//デストラクタ
DirectX12Device::~DirectX12Device() {}

//初期化
bool DirectX12Device::Init(std::shared_ptr<window::Window> target_window) {
  MY_ASSERTION(target_window != nullptr, L"");

  this->target_window_ = target_window;
  render_target_screen_size_ = math::IntVector2::kZeroVector;

  if (auto win = target_window_.lock(); win) {
    render_target_screen_size_ = win->GetScreenSize();
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

  constexpr D3D12_RESOURCE_STATES next_resource_state =
      D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET;
  if (current_resource_state_ != next_resource_state) {
    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        render_targets_[frame_index_].Get(), current_resource_state_,
        next_resource_state);
    command_list_->ResourceBarrier(1, &barrier);
    current_resource_state_ = next_resource_state;
  }

  CD3DX12_CPU_DESCRIPTOR_HANDLE rtv_handle(
      rtv_heap_->GetCPUDescriptorHandleForHeapStart(), frame_index_,
      rtv_heap_size_);
  command_list_->OMSetRenderTargets(1, &rtv_handle, false, nullptr);

  const std::array<float, 4> CLEAR_COLOR = {0.0f, 0.2f, 0.4f, 1.0f};
  command_list_->ClearRenderTargetView(rtv_handle, CLEAR_COLOR.data(), 0,
                                       nullptr);

  return true;
}

bool DirectX12Device::Present() {
  constexpr D3D12_RESOURCE_STATES next_resource_state =
      D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT;
  if (current_resource_state_ != next_resource_state) {
    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        render_targets_[frame_index_].Get(), current_resource_state_,
        next_resource_state);
    command_list_->ResourceBarrier(1, &barrier);
    current_resource_state_ = next_resource_state;
  }

  if (FAILED(command_list_->Close())) {
    return false;
  }

  ID3D12CommandList* command_lists[] = {command_list_.Get()};
  command_queue_->ExecuteCommandLists(ARRAYSIZE(command_lists), command_lists);

  if (FAILED(swap_chain_->Present(1, 0))) {
    return false;
  }

  MoveToNextFrame();

  return true;
}

bool DirectX12Device::CreateDevice() {
  unsigned int dxgi_flags = 0;

  //デバッグ時のみデバッグ情報を取得できるような設定をする
#if defined(_DEBUG)
  ComPtr<ID3D12Debug> debug_controller;
  if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller)))) {
    debug_controller->EnableDebugLayer();
    dxgi_flags |= DXGI_CREATE_FACTORY_DEBUG;
  }
#endif

  if (FAILED(CreateDXGIFactory2(dxgi_flags, IID_PPV_ARGS(&factory_)))) {
    MY_LOG(L"CreateDXGIFactory2 failed");
    return false;
  }

  ComPtr<IDXGIAdapter1> hardware_adapter = GetHardwareAdapter();
  if (!hardware_adapter) {
    MY_LOG(L"GetHardwareAdapter failed");
    return false;
  }

  if (FAILED(D3D12CreateDevice(hardware_adapter.Get(), D3D_FEATURE_LEVEL_11_0,
                               IID_PPV_ARGS(&device_)))) {
    MY_LOG(L"D3D12CreateDevice failed");
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

  DXGI_SWAP_CHAIN_DESC1 swap_chain_desc = {};
  swap_chain_desc.BufferCount = FRAME_COUNT;
  swap_chain_desc.Width = render_target_screen_size_.x;
  swap_chain_desc.Height = render_target_screen_size_.y;
  swap_chain_desc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
  swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
  swap_chain_desc.SampleDesc.Count = 1;

  if (target_window_.expired()) {
    MY_LOG(L"target_window_ not found.");
    return false;
  }

  ComPtr<IDXGISwapChain1> swap_chain;
  if (FAILED(factory_->CreateSwapChainForHwnd(
          command_queue_.Get(), target_window_.lock()->GetHWND(),
          &swap_chain_desc, nullptr, nullptr, &swap_chain))) {
    MY_LOG(L"CreateSwapChainForHwnd failed");
    return false;
  }

  if (FAILED(swap_chain.As(&swap_chain_))) {
    MY_LOG(L"Cast to IDXGISwapChain4 failed");
    return false;
  }

  frame_index_ = swap_chain_->GetCurrentBackBufferIndex();

  if (FAILED(factory_->MakeWindowAssociation(target_window_.lock()->GetHWND(),
                                             DXGI_MWA_NO_ALT_ENTER))) {
    MY_LOG(L"MakeWindowAssociation failed");
    return false;
  }

  D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_desc = {};
  rtv_heap_desc.NumDescriptors = FRAME_COUNT;
  rtv_heap_desc.Type =
      D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
  rtv_heap_desc.Flags =
      D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
  if (FAILED(device_->CreateDescriptorHeap(&rtv_heap_desc,
                                           IID_PPV_ARGS(&rtv_heap_)))) {
    MY_LOG(L"CreateDescriptorHeap failed");
    return false;
  }

  rtv_heap_size_ = device_->GetDescriptorHandleIncrementSize(
      D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

  for (unsigned int i = 0; i < FRAME_COUNT; i++) {
    if (FAILED(swap_chain_->GetBuffer(i, IID_PPV_ARGS(&render_targets_[i])))) {
      MY_LOG(L"GetBuffer buffer number %d isfailed", i);
      return false;
    }

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtv_handle(
        rtv_heap_->GetCPUDescriptorHandleForHeapStart(), i, rtv_heap_size_);
    device_->CreateRenderTargetView(render_targets_[i].Get(), nullptr,
                                    rtv_handle);
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

//ハードウェアアダプターを取得する
ComPtr<IDXGIAdapter1> DirectX12Device::GetHardwareAdapter() {
  ComPtr<IDXGIAdapter1> adapter;
  for (unsigned int adapter_index = 0;
       factory_->EnumAdapters1(adapter_index, &adapter); adapter_index++) {
    DXGI_ADAPTER_DESC1 desc;
    adapter->GetDesc1(&desc);
    if (desc.Flags & DXGI_ADAPTER_FLAG::DXGI_ADAPTER_FLAG_SOFTWARE) continue;
    if (SUCCEEDED(D3D12CreateDevice(adapter.Get(),
                                    D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0,
                                    _uuidof(ID3D12Device), nullptr))) {
      break;
    }
  }
  return adapter;
}

bool DirectX12Device::MoveToNextFrame() {
  const u64 fence_value = fence_values_[frame_index_];
  if (FAILED(command_queue_->Signal(fence_.Get(), fence_value))) {
    return false;
  }

  frame_index_ = swap_chain_->GetCurrentBackBufferIndex();

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
