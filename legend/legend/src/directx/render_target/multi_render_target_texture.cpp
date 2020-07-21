#include "src/directx/render_target/multi_render_target_texture.h"

namespace legend {
namespace directx {
namespace render_target {

//コンストラクタ
MultiRenderTargetTexture::MultiRenderTargetTexture() {}

//デストラクタ
MultiRenderTargetTexture::~MultiRenderTargetTexture() {}

//初期化
bool MultiRenderTargetTexture::Init(IDirectXAccessor& accessor,
                                    const Info& info) {
  render_targets_.resize(1);
  RenderTargetTexture& target = render_targets_[0];
  if (!target.render_target.Init(accessor, info.format, info.width, info.height,
                                 info.clear_color, info.name)) {
    return false;
  }
  target.register_num = info.register_num;
  target.srv_handle = accessor.GetHandle(DescriptorHeapType::CBV_SRV_UAV);

  // accessor.
  D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
  srv_desc.Texture2D.MipLevels = 1;
  srv_desc.Format = info.format;
  srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
  srv_desc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;

  accessor.GetDevice()->CreateShaderResourceView(
      target.render_target.GetResource(), &srv_desc,
      target.srv_handle.cpu_handle_);

  return true;
}

bool MultiRenderTargetTexture::Init(IDirectXAccessor& accessor,
                                    const std::vector<Info>& infos) {
  const u32 render_target_num = static_cast<u32>(infos.size());
  render_targets_.resize(render_target_num);
  for (u32 i = 0; i < render_target_num; i++) {
    RenderTargetTexture& target = render_targets_[i];
    if (!target.render_target.Init(accessor, infos[i].format, infos[i].width,
                                   infos[i].height, infos[i].clear_color,
                                   infos[i].name)) {
      return false;
    }
    target.register_num = infos[i].register_num;
    target.srv_handle = accessor.GetHandle(DescriptorHeapType::CBV_SRV_UAV);

    // accessor.
    D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
    srv_desc.Texture2D.MipLevels = 1;
    srv_desc.Format = infos[i].format;
    srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srv_desc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;

    accessor.GetDevice()->CreateShaderResourceView(
        target.render_target.GetResource(), &srv_desc,
        target.srv_handle.cpu_handle_);
  }
  return true;
}

void MultiRenderTargetTexture::ClearRenderTarget(
    IDirectXAccessor& accessor) const {
  for (auto&& rtt : render_targets_) {
    rtt.render_target.ClearRenderTarget(accessor);
  }
}

void MultiRenderTargetTexture::DrawEnd(IDirectXAccessor& accessor) {
  for (auto&& rtt : render_targets_) {
    rtt.render_target.DrawEnd(accessor);
    rtt.render_target.Transition(
        accessor, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
  }
}

void MultiRenderTargetTexture::SetToGlobalHeap(IDirectXAccessor& accessor,
                                               u32 render_target_number) const {
  const RenderTargetTexture& rtt = render_targets_[render_target_number];
  accessor.SetToGlobalHeap(rtt.register_num, ResourceType::Srv, rtt.srv_handle);
}

void MultiRenderTargetTexture::WriteInfoToPipelineDesc(
    shader::GraphicsPipelineState& pipeline) {
  const u32 render_target_num = static_cast<u32>(render_targets_.size());
  for (u32 i = 0; i < render_target_num; i++) {
    pipeline.SetRTVFormat(render_targets_[i].render_target.GetFormat(), i);
  }
  pipeline.SetRenderTargetNum(render_target_num);
}

void MultiRenderTargetTexture::PrepareToUseRenderTarget(
    IDirectXAccessor& accessor) {
  for (auto&& rtt : render_targets_) {
    rtt.render_target.Transition(
        accessor, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);
  }
}

std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>
MultiRenderTargetTexture::GetRTVHandles() const {
  const u32 render_target_num = static_cast<u32>(render_targets_.size());
  std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> res(render_target_num);
  for (u32 i = 0; i < render_target_num; i++) {
    res[i] = render_targets_[i].render_target.GetHandle().cpu_handle_;
  }
  return res;
}

}  // namespace render_target
}  // namespace directx
}  // namespace legend