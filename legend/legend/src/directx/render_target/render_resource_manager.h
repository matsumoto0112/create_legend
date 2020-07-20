#ifndef LEGEND_DIRECTX_RENDER_TARGET_RENDER_RESOURCE_MANAGER_H_
#define LEGEND_DIRECTX_RENDER_TARGET_RENDER_RESOURCE_MANAGER_H_

/**
 * @brief レンダーターゲットリソース管理クラス定義
 */

#include "src/directx/directx_accessor.h"
#include "src/directx/render_target/depth_stencil.h"
#include "src/directx/render_target/render_target.h"

namespace legend {
namespace directx {
namespace render_target {
class RenderResourceManager {
 public:
  RenderResourceManager();
  ~RenderResourceManager();
  bool Init();
  bool CreateRenderTarget(IDirectXAccessor& accessor, u32 unique_id,
                          DXGI_FORMAT format, u32 width, u32 height,
                          const util::Color4& clear_color,
                          const std::wstring& name);
  bool CreateDepthStencil(IDirectXAccessor& accessor, u32 unique_id,
                          DXGI_FORMAT format, u32 width, u32 height,
                          float depth_clear_value, u8 stencil_clear_value,
                          const std::wstring& name);
  void SetRenderTarget(u32 unique_id);
  void SetDepthStencilTarget(u32 unique_id);

  void ClearCurrentRenderTarget(IDirectXAccessor& accessor);
  void ClearCurrentDepthStencilTarget(IDirectXAccessor& accessor);
  void SetRenderTargetsToCommandList(IDirectXAccessor& accessor);

 private:
  static constexpr u32 SWAP_CHAIN_RENDER_TARGET_ID = 0;
  std::unordered_map<u32, RenderTarget> created_render_targets_;
  std::unordered_map<u32, DepthStencil> created_depth_stencil_targets_;
  u32 current_render_target_id_;
  u32 current_depth_stencil_target_id_;
};

}  // namespace render_target
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_RENDER_TARGET_RENDER_RESOURCE_MANAGER_H_
