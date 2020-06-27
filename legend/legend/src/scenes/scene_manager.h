#ifndef LEGEND_SCENES_SCENE_MANAGER_H_
#define LEGEND_SCENES_SCENE_MANAGER_H_

#include "src/scenes/scene.h"
#include "src/scenes/scene_change.h"

namespace legend {
namespace scenes {

/**
 * @class SceneManager
 * @brief �V�[���Ǘ�
 */
class SceneManager : public ISceneChange, Task {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  SceneManager();
  /**
   * @brief ������
   */
  void Initialize() override;
  /**
   * @brief �I��
   */
  void Finalize() override;
  /**
   * @brief �X�V
   */
  void Update() override;
  /**
   * @brief �`��
   */
  void Draw() override;
  /**
   * @brief �V�[���J��
   */
  void ChangeScene(SceneType next_scene) override;

 private:
  //! ���݂̃V�[��
  Scene* current_scene_;
  //! ���̃V�[��
  SceneType next_scene_;
};

}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_SCENE_MANAGER_H_
