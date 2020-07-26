#ifndef LEGEND_DIRECTX_RENDER_TARGET_RENDER_TARGET_ID_H_
#define LEGEND_DIRECTX_RENDER_TARGET_RENDER_TARGET_ID_H_

/**
 * @file render_target_id.h
 *@brief レンダーターゲットを一意に特定するID定義
 */
namespace legend {
namespace directx {
namespace render_target {

/**
 * @enum RenderTargetID
 * @brief レンダーターゲットを一意に特定するID
 */
enum class RenderTargetID : u32 {
  //スワップチェイン
  BACK_BUFFER = 0,

  MULTI_RENDER_TARGET_TEST,

};
}  // namespace render_target
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_RENDER_TARGET_RENDER_TARGET_ID_H_
