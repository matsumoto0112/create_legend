#include "src/directx/buffer/render_target_texture.h"

namespace legend {
namespace directx {
namespace buffer {

//コンストラクタ
RenderTargetTexture::RenderTargetTexture() {}

//デストラクタ
RenderTargetTexture::~RenderTargetTexture() {}

//初期化
bool RenderTargetTexture::Init(IDirectXAccessor& accessor, u32 register_num,
                               DXGI_FORMAT format, u32 width, u32 height,
                               const util::Color4& clear_color,
                               const std::wstring& name) {
  if (!render_target_.Init(accessor, format, width, height, clear_color,
                           name)) {
    return false;
  }

  this->register_num_ = register_num;
  srv_handle_ = accessor.GetHandle(DescriptorHeapType::CBV_SRV_UAV);

  D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
  srv_desc.Texture2D.MipLevels = 1;
  srv_desc.Format = format;
  srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
  srv_desc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;

  accessor.GetDevice()->CreateShaderResourceView(
      render_target_.GetResource(), &srv_desc, srv_handle_.cpu_handle_);

  return true;
}

//レンダーターゲットをセットする
void RenderTargetTexture::SetRenderTarget(IDirectXAccessor& accessor) {
  render_target_.SetRenderTarget(accessor);
}

//レンダーターゲットをクリアする
void RenderTargetTexture::ClearRenderTarget(IDirectXAccessor& accessor) const {
  render_target_.ClearRenderTarget(accessor);
}

//描画終了
void RenderTargetTexture::DrawEnd(IDirectXAccessor& accessor) {
  render_target_.DrawEnd(accessor);
  render_target_.resource_.Transition(
      accessor, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
}

//グローバルヒープにセットする
void RenderTargetTexture::SetToGlobalHeap(IDirectXAccessor& accessor) const {
  accessor.SetToGlobalHeap(register_num_, ResourceType::Srv, srv_handle_);
}

}  // namespace buffer
}  // namespace directx
}  // namespace legend