#ifndef LEGEND_SCENES_DEBUGSCENE_MODEL_VIEW_H_
#define LEGEND_SCENES_DEBUGSCENE_MODEL_VIEW_H_

/**
 * @brief ���f���`��e�X�g�V�[��
 * @details ���f������ǂݍ��݁A�A���x�h�e�N�X�`���𒣂�����Ԃŕ`�悷��V�[��
 �p�����[�^�Ƃ��ă��f���̍��W�A��]�A�X�P�[�����O�A�J�����̍��W�A��]�Afov������ł���
 */

#include "src/camera/perspective_camera.h"
#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/texture_2d.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/directx/render_target/multi_render_target_texture.h"
#include "src/directx/shader/graphics_pipeline_state.h"
#include "src/directx/shader/root_signature.h"
#include "src/draw/model.h"
#include "src/math/matrix_4x4.h"
#include "src/math/quaternion.h"
#include "src/scenes/scene.h"
#include "src/util/transform.h"

namespace legend {
namespace scenes {
namespace debugscene {

/**
 * @class ModelView
 * @brief ���f���`��e�X�g�V�[��
 */
class ModelView : public Scene {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  ModelView(ISceneChange* scene_change);
  /**
   * @brief �f�X�g���N�^
   */
  ~ModelView();
  /**
   * @brief ������
   */
  bool Initialize() override;
  /**
   * @brief �X�V
   */
  bool Update() override;
  /**
   * @brief �`��
   */
  void Draw() override;

  /**
   * @brief �I������
   */
  void Finalize() override;

 private:
  //! ���f����
  static const std::wstring MODEL_NAME;
  //! �ǂݍ��񂾃��f���f�[�^
  draw::Model model_;
  //! �g�����X�t�H�[���]���p�R���X�^���g�o�b�t�@
  directx::buffer::ConstantBuffer<directx::constant_buffer_structure::Transform>
      transform_cb_;
  util::Transform transform_;
  //�`��}�e���A��
  directx::shader::GraphicsPipelineState pipeline_state_;

  camera::PerspectiveCamera camera_;
};

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DEBUGSCENE_MODEL_VIEW_H_
