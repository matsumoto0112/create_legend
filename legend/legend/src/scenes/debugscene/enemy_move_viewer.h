#ifndef LEGEND_SCENES_DEBUGSCENE_ENEMY_MOVE_VIEWER_H_
#define LEGEND_SCENES_DEBUGSCENE_ENEMY_MOVE_VIEWER_H_

/**
 * @brief ���f���`��e�X�g�V�[��
 * @details ���f������ǂݍ��݁A�A���x�h�e�N�X�`���𒣂�����Ԃŕ`�悷��V�[��
 �p�����[�^�Ƃ��ă��f���̍��W�A��]�A�X�P�[�����O�A�J�����̍��W�A��]�Afov������ł���
 */

#include "src/camera/perspective_camera.h"
#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/shader/graphics_pipeline_state.h"
#include "src/draw/model.h"
#include "src/scenes/scene.h"
#include "src/util/transform.h"

namespace legend {
namespace scenes {
namespace debugscene {

/**
 * @class ModelView
 * @brief ���f���`��e�X�g�V�[��
 */
class EnemyMoveViewer : public Scene {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  EnemyMoveViewer(ISceneChange* scene_change);
  /**
   * @brief �f�X�g���N�^
   */
  ~EnemyMoveViewer();
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
  //! �g�����X�t�H�[���]���p�R���X�^���g�o�b�t�@
  directx::buffer::ConstantBuffer<directx::constant_buffer_structure::Transform>
      transform_cb_;
  //! ���f���̃g�����X�t�H�[��
  util::Transform transform_;
  //! ���C���J����
  camera::PerspectiveCamera camera_;
};

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DEBUGSCENE_ENEMY_MOVE_VIEWER_H_