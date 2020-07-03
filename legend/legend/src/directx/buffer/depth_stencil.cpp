#include "src/directx/buffer/depth_stencil.h"

namespace legend {
namespace directx {
namespace buffer {

//コンストラクタ
DepthStencil::DepthStencil() {}

//デストラクタ
DepthStencil::~DepthStencil() {}

//初期化
bool DepthStencil::Init(IDirectXAccessor& accessor, DXGI_FORMAT format,
                        u32 width, u32 height, const ClearValue& clear_value,
                        const std::wstring& name) {
  CD3DX12_CLEAR_VALUE dsv_clear_value = {};
  dsv_clear_value.Format = format;
  dsv_clear_value.DepthStencil.Depth = clear_value.depth;
  dsv_clear_value.DepthStencil.Stencil = clear_value.stencil;

  CommittedResource::TextureBufferDesc dsv_desc(
      name, format, width, height,
      D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL,
      dsv_clear_value);
  if (!resource_.InitAsTex2D(accessor, dsv_desc)) {
    return false;
  }

  D3D12_DEPTH_STENCIL_VIEW_DESC dsv_view = {};
  dsv_view.Flags = D3D12_DSV_FLAGS::D3D12_DSV_FLAG_NONE;
  dsv_view.Format = format;
  dsv_view.Texture2D.MipSlice = 0;
  dsv_view.ViewDimension = D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2D;

  this->handle_ = accessor.GetHandle(DescriptorHeapType::DSV);
  accessor.GetDevice()->CreateDepthStencilView(
      resource_.GetResource(), &dsv_view, handle_.cpu_handle_);

  this->clear_value_ = clear_value;
  return true;
}

//デプス・ステンシルのクリア
void DepthStencil::ClearDepthStencil(IDirectXAccessor& accessor) const {
  constexpr D3D12_CLEAR_FLAGS CLEAR_FLAGS =
      D3D12_CLEAR_FLAGS::D3D12_CLEAR_FLAG_DEPTH |
      D3D12_CLEAR_FLAGS::D3D12_CLEAR_FLAG_STENCIL;

  accessor.GetCommandList()->ClearDepthStencilView(
      handle_.cpu_handle_, CLEAR_FLAGS, clear_value_.depth,
      clear_value_.stencil, 0, nullptr);
}

void DepthStencil::PrepareToSetCommandList(IDirectXAccessor& accessor) {
  resource_.Transition(
      accessor, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE);
}

}  // namespace buffer
}  // namespace directx
}  // namespace legend