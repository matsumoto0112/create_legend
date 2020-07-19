#include "src/directx/buffer/multi_render_target_texture.h"

#include "src/util/string_util.h"

namespace legend {
namespace directx {
namespace buffer {
MultiRenderTargetTexture::MultiRenderTargetTexture() {}
MultiRenderTargetTexture::~MultiRenderTargetTexture() {}
bool MultiRenderTargetTexture::Init(
    IDirectXAccessor& accessor, DXGI_FORMAT format, u32 width, u32 height,
    const std::vector<RenderTargetCreateInfo>& infos,
    const std::wstring& name_base) {
  this->infos_ = infos;

  const u32 render_target_num = static_cast<u32>(infos.size());
  render_target_resources_.resize(render_target_num);
  handles_.resize(render_target_num);
  srv_handles_.resize(render_target_num);

  for (u32 i = 0; i < render_target_num; i++) {
    CommittedResource& resource = render_target_resources_[i];
    const std::wstring name =
        util::string_util::Format(L"%s[%d]", name_base, i);
    CD3DX12_CLEAR_VALUE clear_value = {};
    clear_value.Format = format;
    memcpy(clear_value.Color, infos[i].clear_color.Get().data(),
           sizeof(float) * 4);
    const CommittedResource::TextureBufferDesc desc(
        name, format, width, height,
        D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET,
        clear_value);
    if (!resource.InitAsTex2D(accessor, desc)) {
      return false;
    }
    resource.Transition(accessor,
                        D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON);

    handles_[i] = accessor.GetHandle(DescriptorHeapType::RTV);

    D3D12_RENDER_TARGET_VIEW_DESC rtv_desc = {};
    rtv_desc.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;
    rtv_desc.Format = format;
    accessor.GetDevice()->CreateRenderTargetView(
        resource.GetResource(), &rtv_desc, handles_[i].cpu_handle_);

    D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
    srv_desc.Texture2D.MipLevels = 1;
    srv_desc.Format = format;
    srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srv_desc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
    srv_handles_[i] = accessor.GetHandle(DescriptorHeapType::CBV_SRV_UAV);

    accessor.GetDevice()->CreateShaderResourceView(
        resource.GetResource(), &srv_desc, srv_handles_[i].cpu_handle_);
  }

  return true;
}
void MultiRenderTargetTexture::WriteInfoToPipelineStateDesc(
    D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipeline_state_desc) const {
  const u32 render_target_num =
      static_cast<u32>(render_target_resources_.size());
  for (u32 i = 0; i < render_target_num; i++) {
    (*pipeline_state_desc).RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM;
  }
  pipeline_state_desc->NumRenderTargets = render_target_num;
}
void MultiRenderTargetTexture::SetRenderTarget(IDirectXAccessor& accessor) {
  for (auto&& rt : render_target_resources_) {
    rt.Transition(accessor,
                  D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);
  }
  const u32 render_target_num =
      static_cast<u32>(render_target_resources_.size());
  std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> handles(render_target_num);
  for (u32 i = 0; i < render_target_num; i++) {
    handles[i] = handles_[i].cpu_handle_;
  }
  accessor.GetCommandList()->OMSetRenderTargets(render_target_num,
                                                handles.data(), FALSE, nullptr);
}

void MultiRenderTargetTexture::ClearRenderTarget(
    IDirectXAccessor& accessor) const {
  const u32 render_target_num =
      static_cast<u32>(render_target_resources_.size());

  for (u32 i = 0; i < render_target_num; i++) {
    accessor.GetCommandList()->ClearRenderTargetView(
        handles_[i].cpu_handle_, infos_[i].clear_color.Get().data(), 0,
        nullptr);
  }
}

void MultiRenderTargetTexture::DrawEnd(IDirectXAccessor& accessor) {
  for (auto&& rt : render_target_resources_) {
    rt.Transition(accessor,
                  D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
  }
}

void MultiRenderTargetTexture::SetToGlobalHeap(
    IDirectXAccessor& accessor) const {
  accessor.SetToGlobalHeap(0, ResourceType::Srv, srv_handles_[0]);
}

}  // namespace buffer
}  // namespace directx
}  // namespace legend