#include "src/directx/render_target/render_resource_manager.h"
namespace legend {
namespace directx {
namespace render_target {

RenderResourceManager::RenderResourceManager() {}

RenderResourceManager::~RenderResourceManager() {}

bool RenderResourceManager::Init() {
  this->current_render_target_id_ = 0;
  this->current_depth_stencil_target_id_ = UINT_LEAST32_MAX;

  return true;
}

bool RenderResourceManager::CreateRenderTarget(
    IDirectXAccessor& accessor, u32 unique_id, DXGI_FORMAT format, u32 width,
    u32 height, const util::Color4& clear_color, const std::wstring& name) {
  MY_ASSERTION(unique_id != SWAP_CHAIN_RENDER_TARGET_ID,
               L"SwapChainのIDとして予約されています。");
  MY_ASSERTION(
      created_render_targets_.find(unique_id) == created_render_targets_.end(),
      L"unique_idが重複しています。");

  RenderTargetTexture render_target;
  if (!render_target.Init(accessor, 0, format, width, height, clear_color,
                          name)) {
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

void RenderResourceManager::SetRenderTarget(u32 unique_id) {
  this->current_render_target_id_ = unique_id;
}

void RenderResourceManager::SetDepthStencilTarget(u32 unique_id) {
  this->current_depth_stencil_target_id_ = unique_id;
}

void RenderResourceManager::ClearCurrentRenderTarget(
    IDirectXAccessor& accessor) {
  if (current_render_target_id_ == SWAP_CHAIN_RENDER_TARGET_ID) {
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
  MY_ASSERTION(current_render_target_id_ == SWAP_CHAIN_RENDER_TARGET_ID ||
                   created_render_targets_.find(current_render_target_id_) !=
                       created_render_targets_.end(),
               L"未登録のレンダーターゲットIDが選択されました。");
  MY_ASSERTION(current_depth_stencil_target_id_ == UINT_LEAST32_MAX ||
                   created_depth_stencil_targets_.find(
                       current_depth_stencil_target_id_) !=
                       created_depth_stencil_targets_.end(),
               L"未登録のデプス・ステンシルターゲットIDが選択されました。");

  D3D12_CPU_DESCRIPTOR_HANDLE rtv_handles[1];
  if (current_render_target_id_ == SWAP_CHAIN_RENDER_TARGET_ID) {
    rtv_handles[0] = accessor.GetBackBufferHandle().cpu_handle_;
  } else {
    rtv_handles[0] = created_render_targets_[current_render_target_id_]
                         .GetRenderTarget()
                         .GetHandle()
                         .cpu_handle_;
  }

  if (current_depth_stencil_target_id_ == UINT_LEAST32_MAX) {
    accessor.GetCommandList()->OMSetRenderTargets(1, rtv_handles, FALSE,
                                                  nullptr);
  } else {
    D3D12_CPU_DESCRIPTOR_HANDLE dsv_handle =
        created_depth_stencil_targets_[current_depth_stencil_target_id_]
            .GetCPUHandle();
    accessor.GetCommandList()->OMSetRenderTargets(1, rtv_handles, TRUE,
                                                  &dsv_handle);
  }
}
}  // namespace render_target
}  // namespace directx
}  // namespace legend
