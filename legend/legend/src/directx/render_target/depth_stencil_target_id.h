#ifndef LEGEND_DIRECTX_RENDER_TARGET_DEPTH_STENCIL_TARGET_ID_H_
#define LEGEND_DIRECTX_RENDER_TARGET_DEPTH_STENCIL_TARGET_ID_H_

/**
 * @file depth_stencil_target_id.h
 * @brief �f�v�X�E�X�e���V���^�[�Q�b�g����ӂɓ��肷��ID��`
 */

namespace legend {
namespace directx {
namespace render_target {

/**
 * @brief �f�v�X�E�X�e���V���^�[�Q�b�g����ӂɓ��肷��ID
 */
enum class DepthStencilTargetID {
  //! �g�p���Ȃ��Ƃ��̃f�t�H���gID
  NONE,

  DEPTH_ONLY,
};

}  // namespace render_target
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_RENDER_TARGET_DEPTH_STENCIL_TARGET_ID_H_
