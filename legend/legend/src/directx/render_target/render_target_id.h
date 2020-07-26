#ifndef LEGEND_DIRECTX_RENDER_TARGET_RENDER_TARGET_ID_H_
#define LEGEND_DIRECTX_RENDER_TARGET_RENDER_TARGET_ID_H_

/**
 * @file render_target_id.h
 *@brief �����_�[�^�[�Q�b�g����ӂɓ��肷��ID��`
 */
namespace legend {
namespace directx {
namespace render_target {

/**
 * @enum RenderTargetID
 * @brief �����_�[�^�[�Q�b�g����ӂɓ��肷��ID
 */
enum class RenderTargetID : u32 {
  //�X���b�v�`�F�C��
  BACK_BUFFER = 0,

  MULTI_RENDER_TARGET_TEST,

};
}  // namespace render_target
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_RENDER_TARGET_RENDER_TARGET_ID_H_
