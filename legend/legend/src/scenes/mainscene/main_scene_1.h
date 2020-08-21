#ifndef LEGEND_SCENES_MAINSCENE_MAIN_SCENE_1_H_
#define LEGEND_SCENES_MAINSCENE_MAIN_SCENE_1_H_

#include "src/camera/perspective_camera.h"
#include "src/enemy/enemy_manager.h"
#include "src/object/desk.h"
#include "src/object/fragment.h"
#include "src/object/obstacle.h"
#include "src/player/player.h"
#include "src/scenes/scene.h"
#include "src/system/game_data.h"
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
  system::TurnSystem turn_system_;
};

}  // namespace mainscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_MAINSCENE_MAIN_SCENE_1_H_
