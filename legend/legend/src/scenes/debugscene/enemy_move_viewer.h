#ifndef LEGEND_SCENES_DEBUGSCENE_ENEMY_MOVE_VIEWER_H_
#define LEGEND_SCENES_DEBUGSCENE_ENEMY_MOVE_VIEWER_H_

#include "src/camera/perspective_camera.h"
#include "src/enemy/enemy_manager.h"
#include "src/object/desk.h"
#include "src/object/obstacle.h"
#include "src/player/player.h"
#include "src/scenes/scene.h"
#include "src/search/search_manager.h"
#include "src/system/physics_field.h"
#include "src/system/turn_system.h"

namespace legend {
namespace scenes {
namespace debugscene {

/**
 * @class EnemyMoveViewer
 * @brief �G�����V�[��1
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
   * @brief �I��
   */
  void Finalize() override;
  /**
   * @brief �^�[���ʂ̍X�V����
   */
  bool UpdateTurn();

 private:
  camera::PerspectiveCamera camera_;
  system::Turn turn_;
  system::TurnSystem current_turn_;
  system::PhysicsField physics_field_;
  //! �v���C���[
  player::Player player_;
  //! ��
  std::vector<object::Desk> desks_;
  //! �G�l�~�[
  enemy::EnemyManager enemy_manager_;
  //! �T���Ǘ�
  search::SearchManager search_manager_;
  //! ��Q�����X�g
  std::vector<object::Obstacle> obstacles_;
};

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DEBUGSCENE_ENEMY_MOVE_VIEWER_H_
