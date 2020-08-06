#ifndef LEGEND_DIRECTX_RENDER_TARGET_DEPTH_STENCIL_TARGET_ID_H_
#define LEGEND_DIRECTX_RENDER_TARGET_DEPTH_STENCIL_TARGET_ID_H_

/**
 * @file depth_stencil_target_id.h
 * @brief デプス・ステンシルターゲットを一意に特定するID定義
 */

namespace legend {
namespace directx {
namespace render_target {

/**
 * @brief デプス・ステンシルターゲットを一意に特定するID
 */
enum class DepthStencilTargetID {
  //! 使用しないときのデフォルトID
  NONE,

  DEPTH_ONLY,
};

}  // namespace render_target
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_RENDER_TARGET_DEPTH_STENCIL_TARGET_ID_H_
