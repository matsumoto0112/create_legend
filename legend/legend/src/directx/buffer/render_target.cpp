#include "src/directx/buffer/render_target.h"

#include "src/directx/buffer/depth_stencil.h"

namespace {
legend::directx::buffer::DepthStencil dsv_;
}  // namespace

namespace legend {
namespace directx {
namespace buffer {

//コンストラクタ
RenderTarget::RenderTarget() {}

//デストラクタ
RenderTarget::~RenderTarget() {}

//初期化
bool RenderTarget::Init(IDirectXAccessor& accessor, DXGI_FORMAT format,
                        u32 width, u32 height, const util::Color4& clear_color,
                        const std::wstring& name) {
  CD3DX12_CLEAR_VALUE clear_value = {};
  clear_value.Format = format;
  memcpy(clear_value.Color, clear_color.Get().data(), sizeof(float) * 4);

  CommittedResource::TextureBufferDesc desc(
      name, format, width, height,
      D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET,
      clear_value);
  if (!resource_.InitAsTex2D(accessor, desc)) {
    return false;
  }

  resource_.Transition(accessor,
                       D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON);

  this->clear_color_ = clear_color;
  this->rtv_handle_ = accessor.GetHandle(DescriptorHeapType::RTV);

  D3D12_RENDER_TARGET_VIEW_DESC rtv_desc = {};
  rtv_desc.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;
  rtv_desc.Format = format;
  accessor.GetDevice()->CreateRenderTargetView(
      resource_.GetResource(), &rtv_desc, rtv_handle_.cpu_handle_);

  return true;
}

bool RenderTarget::InitFromBuffer(IDirectXAccessor& accessor,
                                  ComPtr<ID3D12Resource> buffer,
                                  const util::Color4& clear_color) {
  if (!resource_.InitFromBuffer(accessor, buffer)) {
    return false;
  }

  this->clear_color_ = clear_color;
  this->rtv_handle_ = accessor.GetHandle(DescriptorHeapType::RTV);

  const D3D12_RESOURCE_DESC desc = resource_.GetResource()->GetDesc();
  D3D12_RENDER_TARGET_VIEW_DESC rtv_desc = {};
  rtv_desc.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;
  rtv_desc.Format = desc.Format;

  accessor.GetDevice()->CreateRenderTargetView(
      resource_.GetResource(), &rtv_desc, rtv_handle_.cpu_handle_);

  if (!dsv_.Init(accessor, DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT,
                 static_cast<u32>(desc.Width), desc.Height,
                 DepthStencil::ClearValue{1.0f, 0}, L"DSV")) {
    return false;
  }

  return true;
}

//レンダーターゲットをセットする
void RenderTarget::SetRenderTarget(IDirectXAccessor& accessor) {
  dsv_.PrepareToSetCommandList(accessor);
  resource_.Transition(
      accessor, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);
  accessor.GetCommandList()->OMSetRenderTargets(1, &rtv_handle_.cpu_handle_,
                                                TRUE, &dsv_.GetCPUHandle());
}

//レンダーターゲットをクリアする
void RenderTarget::ClearRenderTarget(IDirectXAccessor& accessor) const {
  accessor.GetCommandList()->ClearRenderTargetView(
      rtv_handle_.cpu_handle_, clear_color_.Get().data(), 0, nullptr);
  dsv_.ClearDepthStencil(accessor);
}

//描画終了
void RenderTarget::DrawEnd(IDirectXAccessor& accessor) {
  resource_.Transition(accessor,
                       D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);
}

}  // namespace buffer
}  // namespace directx
}  // namespace legend