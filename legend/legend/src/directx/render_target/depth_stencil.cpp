#include "src/directx/render_target/depth_stencil.h"

namespace legend {
namespace directx {
namespace render_target {

//コンストラクタ
DepthStencil::DepthStencil() {}

//デストラクタ
DepthStencil::~DepthStencil() {}

//初期化
bool DepthStencil::Init(device::IDirectXAccessor& accessor,
                        const DepthStencilDesc& ds_desc) {
  const CD3DX12_CLEAR_VALUE dsv_clear_value = CD3DX12_CLEAR_VALUE(
      ds_desc.format, ds_desc.depth_value, ds_desc.stencil_value);

  const buffer::CommittedResource::Tex2DDesc desc{
      ds_desc.name,
      ds_desc.format,
      ds_desc.width,
      ds_desc.height,
      D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL,
      dsv_clear_value,
      D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON};
  if (!resource_.InitAsTex2D(accessor, desc)) {
    return false;
  }

  this->handle_ = accessor.GetDSVHandle();
  this->format_ = ds_desc.format;
  this->depth_value_ = ds_desc.depth_value;
  this->stencil_value_ = ds_desc.stencil_value;

  D3D12_DEPTH_STENCIL_VIEW_DESC dsv_view = {};
  dsv_view.Flags = D3D12_DSV_FLAGS::D3D12_DSV_FLAG_NONE;
  dsv_view.Format = ds_desc.format;
  dsv_view.Texture2D.MipSlice = 0;
  dsv_view.ViewDimension = D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2D;
  accessor.GetDevice()->CreateDepthStencilView(resource_.GetResource(),
                                               &dsv_view, handle_.cpu_handle_);
  return true;
}

//クリアする
void DepthStencil::ClearDepthStencil(device::CommandList& command_list) const {
  command_list.GetCommandList()->ClearDepthStencilView(
      handle_.cpu_handle_, D3D12_CLEAR_FLAGS::D3D12_CLEAR_FLAG_DEPTH,
      depth_value_, stencil_value_, 0, nullptr);
}

//状態を遷移させる
void DepthStencil::Transition(device::CommandList& command_list,
                              D3D12_RESOURCE_STATES next_states) {
  resource_.Transition(command_list, next_states);
}

}  // namespace render_target
}  // namespace directx
}  // namespace legend