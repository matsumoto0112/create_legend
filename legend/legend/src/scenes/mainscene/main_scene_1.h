#ifndef LEGEND_SCENES_MAINSCENE_MAIN_SCENE_1_H_
#define LEGEND_SCENES_MAINSCENE_MAIN_SCENE_1_H_

#include "src/camera/perspective_camera.h"
#include "src/object/desk.h"
#include "src/player/player.h"
#include "src/scenes/scene.h"
#include "src/system/physics_field.h"
#include "src/system/turn_system.h"

namespace legend {
namespace scenes {
namespace mainscene {

/**
 * @class MainScene1
 * @brief ���C���V�[������1
 */
class MainScene1 : public Scene {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  MainScene1(ISceneChange* scene_change);
  /**
   * @brief �f�X�g���N�^
   */
  ~MainScene1();
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
  object::Desk desk_;
};

}  // namespace mainscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_MAINSCENE_MAIN_SCENE_1_H_
