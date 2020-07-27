#include "src/directx/render_target/render_target.h"

namespace legend {
namespace directx {
namespace render_target {

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

  const buffer::CommittedResource::TextureBufferDesc desc(
      name, format, width, height,
      D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET,
      clear_value);
  if (!resource_.InitAsTex2D(accessor, desc)) {
    return false;
  }

  this->Transition(accessor,
                   D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON);

  this->clear_color_ = clear_color;
  this->rtv_handle_ = accessor.GetRTVHandle();
  this->format_ = format;
  this->viewport_ = CD3DX12_VIEWPORT(0.0f, 0.0f, width * 1.0f, height * 1.0f);
  this->scissor_rect_ = CD3DX12_RECT(0, 0, width, height);

  D3D12_RENDER_TARGET_VIEW_DESC rtv_desc = {};
  rtv_desc.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;
  rtv_desc.Format = format;
  accessor.GetDevice()->CreateRenderTargetView(
      resource_.GetResource(), &rtv_desc, rtv_handle_.cpu_handle_);

  return true;
}

//バッファから初期化する
bool RenderTarget::InitFromBuffer(IDirectXAccessor& accessor,
                                  ComPtr<ID3D12Resource> buffer,
                                  const util::Color4& clear_color,
                                  const std::wstring& name) {
  if (!resource_.InitFromBuffer(accessor, buffer, name)) {
    return false;
  }

  const D3D12_RESOURCE_DESC desc = resource_.GetResource()->GetDesc();
  D3D12_RENDER_TARGET_VIEW_DESC rtv_desc = {};
  rtv_desc.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;
  rtv_desc.Format = desc.Format;

  this->clear_color_ = clear_color;
  this->rtv_handle_ = accessor.GetRTVHandle();
  this->format_ = desc.Format;
  this->viewport_ =
      CD3DX12_VIEWPORT(0.0f, 0.0f, desc.Width * 1.0f, desc.Height * 1.0f);
  this->scissor_rect_ =
      CD3DX12_RECT(0, 0, static_cast<u32>(desc.Width), desc.Height);

  accessor.GetDevice()->CreateRenderTargetView(
      resource_.GetResource(), &rtv_desc, rtv_handle_.cpu_handle_);

  return true;
}

//レンダーターゲットをクリアする
void RenderTarget::ClearRenderTarget(IDirectXAccessor& accessor) const {
  accessor.GetCommandList()->ClearRenderTargetView(
      rtv_handle_.cpu_handle_, clear_color_.Get().data(), 0, nullptr);
}

//描画終了
void RenderTarget::DrawEnd(IDirectXAccessor& accessor) {
  this->Transition(accessor,
                   D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);
}

//状態遷移
void RenderTarget::Transition(IDirectXAccessor& accessor,
                              D3D12_RESOURCE_STATES next_state) {
  resource_.Transition(accessor, next_state);
}

//パイプラインステートデスクに情報を書き込む
void RenderTarget::WriteInfoToPipelineState(
    shader::GraphicsPipelineState* pipeline) const {
  pipeline->SetRTVFormat(format_, 0);
  pipeline->SetRenderTargetNum(1);
}

}  // namespace render_target
}  // namespace directx
}  // namespace legend