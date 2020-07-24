#ifndef LEGEND_DIRECTX_RENDER_TARGET_RENDER_TARGET_ID_H_
#define LEGEND_DIRECTX_RENDER_TARGET_RENDER_TARGET_ID_H_

/**
 * @file render_target_id.h
 */
namespace legend {
namespace directx {
namespace render_target {
enum class RenderTargetID : u32 {
  //スワップチェイン
  BACK_BUFFER = 0,

  POST_PROCESS_PRE = 100,
  MULTI_RENDER_TARGET_TEST,

};
}  // namespace render_target
}  // namespace directx
}  // namespace legend
#endif  //! LEGEND_DIRECTX_RENDER_TARGET_RENDER_TARGET_ID_H_
