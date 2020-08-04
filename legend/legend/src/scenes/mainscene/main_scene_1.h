#ifndef LEGEND_SCENES_MAINSCENE_MAIN_SCENE_1_H_
#define LEGEND_SCENES_MAINSCENE_MAIN_SCENE_1_H_

#include "src/camera/perspective_camera.h"
#include "src/scenes/scene.h"
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

 private:
  camera::PerspectiveCamera camera_;
  system::Turn turn_;
};

}  // namespace mainscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_MAINSCENE_MAIN_SCENE_1_H_
