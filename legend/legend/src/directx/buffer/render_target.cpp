#include "src/directx/buffer/render_target.h"

namespace {
static const float COLOR[4] = {0.0f, 1.0f, 0.0f, 1.0f};

}  // namespace

namespace legend {
namespace directx {
namespace buffer {

RenderTarget::RenderTarget() {}

RenderTarget::~RenderTarget() {}

bool RenderTarget::Init(IDirectXAccessor& accessor, DXGI_FORMAT format,
                        u32 width, u32 height, const std::wstring& name) {
  CD3DX12_CLEAR_VALUE clear_value = {};
  clear_value.Format = format;
  memcpy(clear_value.Color, COLOR, sizeof(float) * 4);

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

  return true;
}

bool RenderTarget::InitFromBuffer(IDirectXAccessor& accessor,
                                  ComPtr<ID3D12Resource> buffer) {
  if (!resource_.InitFromBuffer(
          accessor, buffer,
          D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON)) {
    return false;
  }

  rtv_handle_ = accessor.GetHandle(DescriptorHeapType::RTV);

  D3D12_RENDER_TARGET_VIEW_DESC rtv_desc = {};
  rtv_desc.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;
  rtv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

  accessor.GetDevice()->CreateRenderTargetView(
      resource_.GetResource(), &rtv_desc, rtv_handle_.cpu_handle_);
  return true;
}

void RenderTarget::CreateShaderResourceView(IDirectXAccessor& accessor) {
  srv_handle_ = accessor.GetHandle(DescriptorHeapType::CBV_SRV_UAV);
  D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
  srv_desc.Texture2D.MipLevels = 1;
  srv_desc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
  srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
  srv_desc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;

  accessor.GetDevice()->CreateShaderResourceView(
      resource_.GetResource(), &srv_desc, srv_handle_.cpu_handle_);
}

void RenderTarget::SetGraphicsCommandList(IDirectXAccessor& accessor) const {
  accessor.GetCommandList()->ResourceBarrier(
      1, &CD3DX12_RESOURCE_BARRIER::Transition(
             resource_.GetResource(),
             D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
             D3D12_RESOURCE_STATE_RENDER_TARGET));

  accessor.GetCommandList()->OMSetRenderTargets(1, &rtv_handle_.cpu_handle_,
                                                false, nullptr);
  accessor.GetCommandList()->ClearRenderTargetView(rtv_handle_.cpu_handle_,
                                                   COLOR, 0, nullptr);
}

void RenderTarget::Transition(IDirectXAccessor& accessor,
                              D3D12_RESOURCE_STATES next_state) {
  resource_.Transition(accessor, next_state);
}

void RenderTarget::EndDraw(IDirectXAccessor& accessor) {
  accessor.GetCommandList()->ResourceBarrier(
      1,
      &CD3DX12_RESOURCE_BARRIER::Transition(
          resource_.GetResource(),
          D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET,
          D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
}

}  // namespace buffer
}  // namespace directx
}  // namespace legend