#ifndef LEGEND_SCENES_DEBUGSCENE_ENEMY_MOVE_VIEWER_H_
#define LEGEND_SCENES_DEBUGSCENE_ENEMY_MOVE_VIEWER_H_

/**
 * @brief ���f���`��e�X�g�V�[��
 * @details ���f������ǂݍ��݁A�A���x�h�e�N�X�`���𒣂�����Ԃŕ`�悷��V�[��
 �p�����[�^�Ƃ��ă��f���̍��W�A��]�A�X�P�[�����O�A�J�����̍��W�A��]�Afov������ł���
 */

#include "src/camera/perspective_camera.h"
#include "src/scenes/scene.h"
#include "src/enemy/enemy_manager.h"

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
  //! ���C���J����
  camera::PerspectiveCamera camera_;
  //! �G�Ǘ��V�X�e��
  enemy::EnemyManager enemy_manager_;
  physics::Plane plane_;
};

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DEBUGSCENE_ENEMY_MOVE_VIEWER_H_
