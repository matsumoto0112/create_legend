#include "src/directx/render_target/render_resource_manager.h"

#include "src/util/stl_extend.h"

namespace legend {
namespace directx {
namespace render_target {

RenderResourceManager::RenderResourceManager() {}

RenderResourceManager::~RenderResourceManager() {}

bool RenderResourceManager::Init() {
  this->current_render_target_id_ = RenderTargetID::BACK_BUFFER;
  this->current_depth_stencil_target_id_ = UINT_LEAST32_MAX;

  return true;
}

bool RenderResourceManager::CreateRenderTarget(IDirectXAccessor& accessor,
                                               RenderTargetID unique_id,
                                               DXGI_FORMAT format, u32 width,
                                               u32 height,
                                               const util::Color4& clear_color,
                                               const std::wstring& name) {
  MY_ASSERTION(
      created_render_targets_.find(unique_id) == created_render_targets_.end(),
      L"unique_idが重複しています。");

  MultiRenderTargetTexture render_target;
  const MultiRenderTargetTexture::Info ifo{0,      format,         width,
                                           height, util::Color4(), L""};
  if (!render_target.Init(accessor, ifo)) {
    return false;
  }

  created_render_targets_[unique_id] = render_target;
  return true;
}

bool RenderResourceManager::CreateRenderTargets(IDirectXAccessor& accessor,
                                                RenderTargetID unique_id,
                                                const std::vector<Info>& info) {
  MY_ASSERTION(
      created_render_targets_.find(unique_id) == created_render_targets_.end(),
      L"unique_idが重複しています。");

  MultiRenderTargetTexture render_target;
  if (!render_target.Init(accessor, info)) {
    return false;
  }

  created_render_targets_[unique_id] = render_target;
  return true;
}

bool RenderResourceManager::CreateDepthStencil(
    IDirectXAccessor& accessor, u32 unique_id, DXGI_FORMAT format, u32 width,
    u32 height, float depth_clear_value, u8 stencil_clear_value,
    const std::wstring& name) {
  MY_ASSERTION(created_depth_stencil_targets_.find(unique_id) ==
                   created_depth_stencil_targets_.end(),
               L"unique_idが重複しています。");

  DepthStencil depth_stencil;
  if (!depth_stencil.Init(accessor, format, width, height,
                          {depth_clear_value, stencil_clear_value}, name)) {
    return false;
  }

  created_depth_stencil_targets_[unique_id] = depth_stencil;
  return true;
}

void RenderResourceManager::SetRenderTarget(RenderTargetID unique_id) {
  this->current_render_target_id_ = unique_id;
}

void RenderResourceManager::SetDepthStencilTarget(u32 unique_id) {
  this->current_depth_stencil_target_id_ = unique_id;
}

void RenderResourceManager::WriteRenderTargetInfoToPipelineDesc(
    IDirectXAccessor& accessor, RenderTargetID unique_id,
    shader::GraphicsPipelineState& pipeline) {
  if (unique_id == RenderTargetID::BACK_BUFFER) {
    pipeline.SetRTVFormat(accessor.GetBackBufferFormat(), 0);
    pipeline.SetRenderTargetNum(1);
  } else {
    MY_ASSERTION(created_render_targets_.find(unique_id) !=
                     created_render_targets_.end(),
                 L"未登録のレンダーターゲットが選択されました。");
    created_render_targets_[unique_id].WriteInfoToPipelineDesc(&pipeline);
  }
}

void RenderResourceManager::UseRenderTargetToShaderResource(
    IDirectXAccessor& accessor, RenderTargetID unique_id,
    u32 render_target_number) {
  MY_ASSERTION(
      unique_id != RenderTargetID::BACK_BUFFER,
      L"SwapChainのレンダーターゲットはリソースとして使用できません。");
  created_render_targets_[unique_id].SetToGlobalHeap(accessor,
                                                     render_target_number);
}

void RenderResourceManager::ClearCurrentRenderTarget(
    IDirectXAccessor& accessor) {
  if (current_render_target_id_ == RenderTargetID::BACK_BUFFER) {
    accessor.ClearBackBufferTarget(accessor);
  } else {
    created_render_targets_[current_render_target_id_].ClearRenderTarget(
        accessor);
  }
}

void RenderResourceManager::ClearCurrentDepthStencilTarget(
    IDirectXAccessor& accessor) {
  created_depth_stencil_targets_[current_depth_stencil_target_id_]
      .ClearDepthStencil(accessor);
}

void RenderResourceManager::SetRenderTargetsToCommandList(
    IDirectXAccessor& accessor) {
  MY_ASSERTION(
      current_render_target_id_ == RenderTargetID::BACK_BUFFER ||
          util::Exist(created_render_targets_, current_render_target_id_),
      L"未登録のレンダーターゲットIDが選択されました。");
  MY_ASSERTION(current_depth_stencil_target_id_ == UINT_LEAST32_MAX ||
                   created_depth_stencil_targets_.find(
                       current_depth_stencil_target_id_) !=
                       created_depth_stencil_targets_.end(),
               L"未登録のデプス・ステンシルターゲットIDが選択されました。");

  if (current_render_target_id_ == RenderTargetID::BACK_BUFFER) {
    accessor.SetBackBuffer(accessor);
  } else {
    created_render_targets_[current_render_target_id_].PrepareToUseRenderTarget(
        accessor);
  }

  const std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> rtv_handles = [&]() {
    if (current_render_target_id_ == RenderTargetID::BACK_BUFFER) {
      return std::vector{accessor.GetBackBufferHandle().cpu_handle_};
    } else {
      return created_render_targets_[current_render_target_id_].GetRTVHandles();
    }
  }();
  // accessor
  const u32 render_target_num = static_cast<u32>(rtv_handles.size());
  if (current_depth_stencil_target_id_ == UINT_LEAST32_MAX) {
    accessor.GetCommandList()->OMSetRenderTargets(
        render_target_num, rtv_handles.data(), FALSE, nullptr);
  } else {
    D3D12_CPU_DESCRIPTOR_HANDLE dsv_handle =
        created_depth_stencil_targets_[current_depth_stencil_target_id_]
            .GetCPUHandle();
    accessor.GetCommandList()->OMSetRenderTargets(
        render_target_num, rtv_handles.data(), TRUE, &dsv_handle);
  }
}
void RenderResourceManager::DrawEnd(IDirectXAccessor& accessor) {
  if (current_render_target_id_ == RenderTargetID::BACK_BUFFER) {
    // accessor.
  } else {
    created_render_targets_[current_render_target_id_].DrawEnd(accessor);
  }
}

bool RenderResourceManager::IsRegisteredRenderTargetID(
    RenderTargetID unique_id) const {
  return util::Exist(created_render_targets_, unique_id);
}

}  // namespace render_target
}  // namespace directx
}  // namespace legend
