#include "src/directx/device/swap_chain.h"

#include "src/directx/directx_helper.h"

namespace legend {
namespace directx {
namespace device {

//コンストラクタ
SwapChain::SwapChain() {}

//デストラクタ
SwapChain::~SwapChain() {}

//初期化
bool SwapChain::Init(IDirectXAccessor& accessor, DXGIAdapter& adapter,
                     window::Window& target_window,
                     ID3D12CommandQueue* command_queue, DXGI_FORMAT format) {
  this->allow_tearing_ = util::enum_util::IsBitpop(adapter.GetOptions() &
                                                   DeviceOptionFlags::TEARING);
  //スワップチェインを作成する
  DXGI_SWAP_CHAIN_DESC1 swap_chain_desc = {};
  swap_chain_desc.BufferCount = FRAME_COUNT;
  swap_chain_desc.Width = target_window.GetScreenSize().x;
  swap_chain_desc.Height = target_window.GetScreenSize().y;
  swap_chain_desc.Format = format;
  swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
  swap_chain_desc.SampleDesc.Count = 1;
  swap_chain_desc.Flags =
      allow_tearing_ ? DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING
                     : 0;

  ComPtr<IDXGISwapChain1> swap_chain_1;
  if (HRESULT hr = adapter.GetFactory()->CreateSwapChainForHwnd(
          command_queue, target_window.GetHWND(), &swap_chain_desc, nullptr,
          nullptr, &swap_chain_1);
      FAILED(hr)) {
    MY_LOG(L"CreateSwapChainForHwnd failed.\nReadon : %s",
           directx_helper::HrToWString(hr));
    return false;
  }

  if (HRESULT hr = swap_chain_1.As(&swap_chain_); FAILED(hr)) {
    MY_LOG(L"IDXGISwapChain1 As IDXGISwapChain3 failed.\nReadon : %s",
           directx_helper::HrToWString(hr));
    return false;
  }

  if (allow_tearing_) {
    adapter.GetFactory()->MakeWindowAssociation(target_window.GetHWND(),
                                                DXGI_MWA_NO_ALT_ENTER);
  }

  //レンダーターゲットを作成する
  for (u32 n = 0; n < FRAME_COUNT; n++) {
    ComPtr<ID3D12Resource> buffer;
    if (HRESULT hr = swap_chain_->GetBuffer(n, IID_PPV_ARGS(&buffer));
        FAILED(hr)) {
      MY_LOG(L"GetBuffer failed.\nReadon : %s",
             directx_helper::HrToWString(hr));
      return false;
    }
    if (!render_targets_[n].InitFromBuffer(
            accessor, buffer, util::Color4(0.0f, 0.0f, 0.0f, 0.0f),
            util::string_util::Format(L"Render Target %u", n))) {
      return false;
    }
    if (!render_targets_[n].CreateDepthStencil(
            accessor, DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT, 1280,
            720, buffer::DepthStencil::ClearValue(1.0f, 0),
            util::string_util::Format(L"DepthStencil %u", n))) {
      return false;
    }
  }

  this->frame_index_ = swap_chain_->GetCurrentBackBufferIndex();
  return true;
}

//バックバッファにセットする
void SwapChain::SetBackBuffer(IDirectXAccessor& accessor) {
  render_targets_[frame_index_].SetRenderTarget(accessor);
}

//バックバッファをクリアする
void SwapChain::ClearBackBuffer(IDirectXAccessor& accessor) {
  render_targets_[frame_index_].ClearRenderTarget(accessor);
}

//描画を終了する
void SwapChain::DrawEnd(IDirectXAccessor& accessor) {
  render_targets_[frame_index_].DrawEnd(accessor);
}

//バックバッファを表示する
bool SwapChain::Present() {
  HRESULT hr;
  if (allow_tearing_) {
    hr = swap_chain_->Present(0, DXGI_PRESENT_ALLOW_TEARING);
  } else {
    hr = swap_chain_->Present(1, 0);
  }
  if (FAILED(hr)) {
    return false;
  }

  return true;
}

//現在のフレームインデックスを更新する
void SwapChain::UpdateCurrentFrameIndex() {
  frame_index_ = swap_chain_->GetCurrentBackBufferIndex();
}

}  // namespace device
}  // namespace directx
}  // namespace legend