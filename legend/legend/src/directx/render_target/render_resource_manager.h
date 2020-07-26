#ifndef LEGEND_DIRECTX_RENDER_TARGET_RENDER_RESOURCE_MANAGER_H_
#define LEGEND_DIRECTX_RENDER_TARGET_RENDER_RESOURCE_MANAGER_H_

/**
 * @brief レンダーターゲットリソース管理クラス定義
 */

#include "src/directx/device/dxgi_adapter.h"
#include "src/directx/device/swap_chain.h"
#include "src/directx/directx_accessor.h"
#include "src/directx/render_target/depth_stencil.h"
#include "src/directx/render_target/multi_render_target_texture.h"
#include "src/directx/render_target/render_target_id.h"

namespace legend {
namespace directx {
namespace render_target {

/**
 * @brief レンダーターゲットリソース管理クラス
 */
class RenderResourceManager {
 public:
  using Info = MultiRenderTargetTexture::Info;

 public:
  RenderResourceManager();
  ~RenderResourceManager();

  bool Init(IDirectXAccessor& accessor, device::DXGIAdapter& adapter,
            window::Window& target_window, u32 frame_count, DXGI_FORMAT format,
            ID3D12CommandQueue* command_queue);
  bool CreateRenderTarget(IDirectXAccessor& accessor, RenderTargetID unique_id,
                          DXGI_FORMAT format, u32 width, u32 height,
                          const util::Color4& clear_color,
                          const std::wstring& name);
  bool CreateRenderTargets(IDirectXAccessor& accessor, RenderTargetID unique_id,
                           const std::vector<Info>& info);
  bool CreateDepthStencil(IDirectXAccessor& accessor, u32 unique_id,
                          DXGI_FORMAT format, u32 width, u32 height,
                          float depth_clear_value, u8 stencil_clear_value,
                          const std::wstring& name);
  void SetRenderTarget(RenderTargetID unique_id);
  void SetDepthStencilTarget(u32 unique_id);
  void WriteRenderTargetInfoToPipelineDesc(
      IDirectXAccessor& accessor, RenderTargetID unique_id,
      shader::GraphicsPipelineState& pipeline);
  void UseRenderTargetToShaderResource(IDirectXAccessor& accessor,
                                       RenderTargetID unique_id,
                                       u32 render_target_number);
  void ClearCurrentRenderTarget(IDirectXAccessor& accessor);
  void ClearCurrentDepthStencilTarget(IDirectXAccessor& accessor);
  void SetRenderTargetsToCommandList(IDirectXAccessor& accessor);
  void DrawEnd(IDirectXAccessor& accessor);

  bool IsRegisteredRenderTargetID(RenderTargetID unique_id) const;
  bool Present();
  u32 GetCurrentFrameIndex();

 private:
  void SetBackBufferToCommandList(IDirectXAccessor& accessor);

 private:
  device::SwapChain swap_chain_;
  std::unordered_map<RenderTargetID, MultiRenderTargetTexture>
      created_render_targets_;
  std::unordered_map<u32, DepthStencil> created_depth_stencil_targets_;
  RenderTargetID current_render_target_id_;
  u32 current_depth_stencil_target_id_;
};

}  // namespace render_target
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_RENDER_TARGET_RENDER_RESOURCE_MANAGER_H_
