#include "src/directx/render_target/multi_render_target_texture.h"

#include "src/math/math_util.h"

namespace legend {
namespace directx {
namespace render_target {

//コンストラクタ
MultiRenderTargetTexture::MultiRenderTargetTexture() {}

//デストラクタ
MultiRenderTargetTexture::~MultiRenderTargetTexture() {}

//初期化
bool MultiRenderTargetTexture::Init(
    device::IDirectXAccessor& accessor,
    descriptor_heap::heap_parameter::LocalHeapID srv_local_heap_id,
    const Info& info) {
  return Init(accessor, srv_local_heap_id, std::vector{info});
}

bool MultiRenderTargetTexture::Init(
    device::IDirectXAccessor& accessor,
    descriptor_heap::heap_parameter::LocalHeapID srv_local_heap_id,
    const std::vector<Info>& infos) {
  this->render_target_num_ = static_cast<u32>(infos.size());
  render_targets_.resize(render_target_num_);
  viewports_.resize(render_target_num_);
  scissor_rects_.resize(render_target_num_);
  rtv_handles_.resize(render_target_num_);

  for (u32 i = 0; i < render_target_num_; i++) {
    RenderTargetTexture& target = render_targets_[i];
    RenderTarget::RenderTargetDesc desc{infos[i].name, infos[i].width,
                                        infos[i].height, infos[i].format,
                                        infos[i].clear_color};
    if (!target.render_target.Init(accessor, desc)) {
      return false;
    }
    target.register_num = infos[i].register_num;
    target.srv_handle = accessor.GetLocalHandle(
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
    rtv_handles_[i] = target.render_target.GetHandle().cpu_handle_;
  }

  return true;
}

bool MultiRenderTargetTexture::InitFromBuffer(
    device::IDirectXAccessor& accessor, ComPtr<ID3D12Resource> buffer,
    const util::Color4& clear_color, const std::wstring& name) {
  this->render_target_num_ = 1;
  render_targets_.resize(render_target_num_);
  viewports_.resize(render_target_num_);
  scissor_rects_.resize(render_target_num_);
  rtv_handles_.resize(render_target_num_);

  RenderTargetTexture& target = render_targets_[0];
  if (!target.render_target.InitFromBuffer(accessor, buffer, clear_color,
                                           name)) {
    return false;
  }
  target.register_num = -1;
  target.srv_handle = {};

  viewports_[0] = target.render_target.GetViewport();
  scissor_rects_[0] = target.render_target.GetScissorRect();
  rtv_handles_[0] = target.render_target.GetHandle().cpu_handle_;

  return true;
}

void MultiRenderTargetTexture::ClearRenderTarget(
    device::CommandList& command_list) const {
  for (auto&& rt : render_targets_) {
    rt.render_target.ClearRenderTarget(command_list);
  }
}

void MultiRenderTargetTexture::DrawEnd(device::CommandList& command_list) {
  for (auto&& rt : render_targets_) {
    rt.render_target.Transition(
        command_list, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
  }
}

void MultiRenderTargetTexture::PrepareToUseRenderTargets(
    directx::device::CommandList& command_list) {
  for (auto&& rt : render_targets_) {
    rt.render_target.Transition(
        command_list,
        D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);
  }
}

void MultiRenderTargetTexture::UseAsSRV(device::IDirectXAccessor& accessor,
                                        u32 render_target_number) {
  MY_ASSERTION(
      math::util::IsInRange(render_target_number, 0u, render_target_num_),
      L"render_target_numberが範囲外です。");

  const RenderTargetTexture& rtt = render_targets_[render_target_num_];
  accessor.RegisterHandle(rtt.register_num, shader::ResourceType::SRV,
                          rtt.srv_handle);
}

void MultiRenderTargetTexture::SetViewport(
    device::CommandList& command_list) const {
  command_list.GetCommandList()->RSSetViewports(render_target_num_,
                                                viewports_.data());
}

void MultiRenderTargetTexture::SetScissorRect(
    device::CommandList& command_list) const {
  command_list.GetCommandList()->RSSetScissorRects(render_target_num_,
                                                   scissor_rects_.data());
}

void MultiRenderTargetTexture::Transition(device::CommandList& command_list,
                                          D3D12_RESOURCE_STATES next_state) {
  for (auto&& rt : render_targets_) {
    rt.render_target.Transition(command_list, next_state);
  }
}

}  // namespace render_target
}  // namespace directx
}  // namespace legend