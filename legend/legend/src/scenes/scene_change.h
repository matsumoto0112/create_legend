#ifndef LEGEND_SCENES_SCENE_CHANGE_H_
#define LEGEND_SCENES_SCENE_CHANGE_H_

namespace legend {
namespace scenes {
/**
 * @brief �g�p����V�[���̗�
 */
enum class SceneType {
  NONE = 0,
  TITLE,
  GAMEOVER,
};

class ISceneChange {
 public:
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~ISceneChange() = 0;
  /**
   * @brief ���̃V�[���ɐ؂�ւ���
   */
  virtual void ChangeScene(SceneType next_scene) = 0;
};
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_SCENE_CHANGE_H_
