#include "src/directx/render_target/multi_render_target_texture.h"

namespace legend {
namespace directx {
namespace render_target {

//�R���X�g���N�^
MultiRenderTargetTexture::MultiRenderTargetTexture() {}

//�f�X�g���N�^
MultiRenderTargetTexture::~MultiRenderTargetTexture() {}

//������
bool MultiRenderTargetTexture::Init(
    IDirectXAccessor& accessor,
    descriptor_heap::heap_parameter::LocalHeapID srv_local_heap_id,
    const Info& info) {
  return Init(accessor, srv_local_heap_id, std::vector{info});
}

bool MultiRenderTargetTexture::Init(
    IDirectXAccessor& accessor,
    descriptor_heap::heap_parameter::LocalHeapID srv_local_heap_id,
    const std::vector<Info>& infos) {
  const u32 render_target_num = static_cast<u32>(infos.size());
  render_targets_.resize(render_target_num);
  viewports_.resize(render_target_num);
  scissor_rects_.resize(render_target_num);

  for (u32 i = 0; i < render_target_num; i++) {
    RenderTargetTexture& target = render_targets_[i];
    if (!target.render_target.Init(accessor, infos[i].format, infos[i].width,
                                   infos[i].height, infos[i].clear_color,
                                   infos[i].name)) {
      return false;
    }
    target.register_num = infos[i].register_num;
    target.srv_handle = accessor.GetLocalHeapHandle(
        descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID);

    D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
    srv_desc.Texture2D.MipLevels = 1;
    srv_desc.Format = infos[i].format;
    srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srv_desc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;

    accessor.GetDevice()->CreateShaderResourceView(
        target.render_target.GetResource(), &srv_desc,
        target.srv_handle.cpu_handle_);

    viewports_[i] = target.render_target.GetViewport();
    scissor_rects_[i] = target.render_target.GetScissorRect();
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

void MultiRenderTargetTexture::WriteInfoToPipelineState(
    shader::GraphicsPipelineState* pipeline) {
  const u32 render_target_num = static_cast<u32>(render_targets_.size());
  for (u32 i = 0; i < render_target_num; i++) {
    pipeline->SetRTVFormat(render_targets_[i].render_target.GetFormat(), i);
  }
  pipeline->SetRenderTargetNum(render_target_num);
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

void MultiRenderTargetTexture::SetViewport(IDirectXAccessor& accessor) const {
  const u32 viewport_num = static_cast<u32>(viewports_.size());
  accessor.GetCommandList()->RSSetViewports(viewport_num, viewports_.data());
}

void MultiRenderTargetTexture::SetScissorRect(
    IDirectXAccessor& accessor) const {
  const u32 rect_num = static_cast<u32>(scissor_rects_.size());
  accessor.GetCommandList()->RSSetScissorRects(rect_num, scissor_rects_.data());
}

}  // namespace render_target
}  // namespace directx
}  // namespace legend