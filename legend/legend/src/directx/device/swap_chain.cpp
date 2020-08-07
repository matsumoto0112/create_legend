#include "src/directx/device/swap_chain.h"

#include "src/directx/directx_helper.h"

namespace legend {
namespace directx {
namespace device {

using directx_helper::Failed;
using directx_helper::Succeeded;

SwapChain::SwapChain() {}

SwapChain::~SwapChain() {}

bool SwapChain::Init(IDirectXAccessor& accessor, DXGIAdapter& adapter,
                     u32 frame_count, u32 width, u32 height, DXGI_FORMAT format,
                     HWND hwnd, ID3D12CommandQueue* command_queue) {
  allow_tearing_ = util::enum_util::IsBitpop(adapter.GetOptions() &
                                             DeviceOptionFlags::TEARING);

  DXGI_SWAP_CHAIN_DESC1 swap_chain_desc = {};
  swap_chain_desc.BufferCount = frame_count;
  swap_chain_desc.Width = width;
  swap_chain_desc.Height = height;
  swap_chain_desc.Format = format;
  swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
  swap_chain_desc.SampleDesc.Count = 1;
  swap_chain_desc.Flags =
      allow_tearing_ ? DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING
                     : 0;

  ComPtr<IDXGISwapChain1> swap_chain;
  if (Failed(adapter.GetFactory()->CreateSwapChainForHwnd(
          command_queue, hwnd, &swap_chain_desc, nullptr, nullptr,
          &swap_chain))) {
    return false;
  }

  if (allow_tearing_) {
    if (Failed(adapter.GetFactory()->MakeWindowAssociation(
            hwnd, DXGI_MWA_NO_ALT_ENTER))) {
      return false;
    }
  }

  if (Failed(swap_chain.As(&swap_chain_))) {
    return false;
  }

  return true;
}

ComPtr<ID3D12Resource> SwapChain::GetBuffer(u32 index) const {
  ComPtr<ID3D12Resource> res;
  if (Failed(swap_chain_->GetBuffer(index, IID_PPV_ARGS(&res)))) {
    return nullptr;
  }
  return res;
}

bool SwapChain::Present() {
  HRESULT hr;
  if (allow_tearing_) {
    hr = swap_chain_->Present(0, DXGI_PRESENT_ALLOW_TEARING);
  } else {
    hr = swap_chain_->Present(1, 0);
  }
  if (Failed(hr)) {
    return false;
  }

  return true;
}

u32 SwapChain::GetCurrentBackBufferIndex() const {
  return swap_chain_->GetCurrentBackBufferIndex();
}

}  // namespace device
}  // namespace directx
}  // namespace legend