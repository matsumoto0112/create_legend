#include "src/directx/buffer/render_target.h"

namespace {
static const float COLOR[4] = {0.0f, 1.0f, 0.0f, 1.0f};

}  // namespace

namespace legend {
namespace directx {
namespace buffer {

RenderTarget::RenderTarget() {}

RenderTarget::~RenderTarget() {}

bool RenderTarget::Init(IDirectXAccessor& accessor, u32 width, u32 height,
                        DXGI_FORMAT format, ID3D12DescriptorHeap* heap,
                        u32 index, const std::wstring& name) {
  CD3DX12_HEAP_PROPERTIES props(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT);
  D3D12_RESOURCE_DESC desc =
      CD3DX12_RESOURCE_DESC::Tex2D(format, width, height);
  desc.Flags |= D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
  desc.MipLevels = 1;
  CD3DX12_CLEAR_VALUE clear_value = {};
  clear_value.Format = format;
  memcpy(clear_value.Color, COLOR, sizeof(float) * 4);

  if (FAILED(accessor.GetDevice()->CreateCommittedResource(
          &props, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc,
          D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
          &clear_value, IID_PPV_ARGS(&resource_)))) {
    return false;
  }

  const u32 heap_size = accessor.GetDevice()->GetDescriptorHandleIncrementSize(
      D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
  cpu_handle_ = CD3DX12_CPU_DESCRIPTOR_HANDLE(
      heap->GetCPUDescriptorHandleForHeapStart(), index, heap_size);

  D3D12_RENDER_TARGET_VIEW_DESC rtv_desc = {};
  rtv_desc.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;
  rtv_desc.Format = format;

  accessor.GetDevice()->CreateRenderTargetView(resource_.Get(), &rtv_desc,
                                               cpu_handle_);

  return true;
}

void RenderTarget::CreateShaderResourceView(IDirectXAccessor& accessor,
                                            HeapManager& heap_manager) {
  srv_handle_ = heap_manager.GetLocalHandle();
  D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
  srv_desc.Texture2D.MipLevels = 1;
  srv_desc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
  srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
  srv_desc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;

  accessor.GetDevice()->CreateShaderResourceView(resource_.Get(), &srv_desc,
                                                 srv_handle_.cpu_handle_);
}

void RenderTarget::SetGraphicsCommandList(IDirectXAccessor& accessor) const {
  accessor.GetCommandList()->ResourceBarrier(
      1, &CD3DX12_RESOURCE_BARRIER::Transition(
             resource_.Get(),
             D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
             D3D12_RESOURCE_STATE_RENDER_TARGET));

  accessor.GetCommandList()->OMSetRenderTargets(1, &cpu_handle_, false,
                                                nullptr);
  accessor.GetCommandList()->ClearRenderTargetView(cpu_handle_, COLOR, 0,
                                                   nullptr);
}

void RenderTarget::EndDraw(IDirectXAccessor& accessor) {
  accessor.GetCommandList()->ResourceBarrier(
      1, &CD3DX12_RESOURCE_BARRIER::Transition(
             resource_.Get(),
             D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET,
             D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
}

}  // namespace buffer
}  // namespace directx
}  // namespace legend