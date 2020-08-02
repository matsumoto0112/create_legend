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
                     window::Window& target_window, DXGI_FORMAT format,
                     u32 back_buffer_count, ID3D12CommandQueue* command_queue) {
  this->allow_tearing_ = util::enum_util::IsBitpop(adapter.GetOptions() &
                                                   DeviceOptionFlags::TEARING);
  render_targets_.resize(back_buffer_count);

  const u32 screen_width = target_window.GetScreenSize().x;
  const u32 screen_height = target_window.GetScreenSize().y;

  //スワップチェインを作成する
  DXGI_SWAP_CHAIN_DESC1 swap_chain_desc = {};
  swap_chain_desc.BufferCount = back_buffer_count;
  swap_chain_desc.Width = screen_width;
  swap_chain_desc.Height = screen_height;
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
  for (u32 n = 0; n < back_buffer_count; n++) {
    ComPtr<ID3D12Resource> buffer;
    if (HRESULT hr = swap_chain_->GetBuffer(n, IID_PPV_ARGS(&buffer));
        FAILED(hr)) {
      MY_LOG(L"GetBuffer failed.\nReadon : %s",
             directx_helper::HrToWString(hr));
      return false;
    }
    if (!render_targets_[n].InitFromBuffer(
            accessor, buffer, util::Color4(0.2f, 0.2f, 0.2f, 1.0f),
            util::string_util::Format(L"Render Target %u", n))) {
      return false;
    }
  }

  this->viewport_ =
      CD3DX12_VIEWPORT(0.0f, 0.0f, screen_width * 1.0f, screen_height * 1.0f);
  this->scissor_rect_ = CD3DX12_RECT(0, 0, screen_width, screen_height);
  return true;
}

//バックバッファをクリアする
void SwapChain::ClearBackBuffer(IDirectXAccessor& accessor) {
  render_targets_[GetCurrentFrameIndex()].ClearRenderTarget(accessor);
}

//描画開始
void SwapChain::DrawBegin(IDirectXAccessor& accessor) {
  render_targets_[GetCurrentFrameIndex()].Transition(
      accessor, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);
  SetViewport(accessor);
  SetScissorRect(accessor);
}

//描画を終了する
void SwapChain::DrawEnd(IDirectXAccessor& accessor) {
  render_targets_[GetCurrentFrameIndex()].DrawEnd(accessor);
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

//ビューポートをセットする
void SwapChain::SetViewport(IDirectXAccessor& accessor) const {
  accessor.GetCommandList()->RSSetViewports(1, &viewport_);
}

//シザー矩形をセットする
void SwapChain::SetScissorRect(IDirectXAccessor& accessor) const {
  accessor.GetCommandList()->RSSetScissorRects(1, &scissor_rect_);
}

u32 SwapChain::GetCurrentFrameIndex() const {
  return swap_chain_->GetCurrentBackBufferIndex();
}

}  // namespace device
}  // namespace directx
}  // namespace legend