#include "src/directx/buffer/render_target.h"

namespace legend {
namespace directx {
namespace buffer {

RenderTarget::RenderTarget() {}

RenderTarget::~RenderTarget() {}

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
  resource_.Transition(
      accessor,
      D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

  rtv_handle_ = accessor.GetHandle(DescriptorHeapType::RTV);

  D3D12_RENDER_TARGET_VIEW_DESC rtv_desc = {};
  rtv_desc.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;
  rtv_desc.Format = format;

  accessor.GetDevice()->CreateRenderTargetView(
      resource_.GetResource(), &rtv_desc, rtv_handle_.cpu_handle_);

  this->clear_color_ = clear_color;

  return true;
}

bool RenderTarget::InitFromBuffer(IDirectXAccessor& accessor,
                                  ComPtr<ID3D12Resource> buffer,
                                  const util::Color4& clear_color) {
  if (!resource_.InitFromBuffer(accessor, buffer)) {
    return false;
  }

  this->clear_color_ = clear_color;
  rtv_handle_ = accessor.GetHandle(DescriptorHeapType::RTV);

  D3D12_RENDER_TARGET_VIEW_DESC rtv_desc = {};
  rtv_desc.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;
  rtv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

  accessor.GetDevice()->CreateRenderTargetView(
      resource_.GetResource(), &rtv_desc, rtv_handle_.cpu_handle_);
  return true;
}

void RenderTarget::SetRenderTarget(IDirectXAccessor& accessor) {
  resource_.Transition(
      accessor, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);
  accessor.GetCommandList()->OMSetRenderTargets(1, &rtv_handle_.cpu_handle_,
                                                false, nullptr);
}

void RenderTarget::ClearRenderTarget(IDirectXAccessor& accessor) const {
  accessor.GetCommandList()->ClearRenderTargetView(
      rtv_handle_.cpu_handle_, clear_color_.Get().data(), 0, nullptr);
}

void RenderTarget::DrawEnd(IDirectXAccessor& accessor) {
  resource_.Transition(accessor,
                       D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);
}

}  // namespace buffer
}  // namespace directx
}  // namespace legend