#ifndef LEGEND_SCENES_SCENE_CHANGE_H_
#define LEGEND_SCENES_SCENE_CHANGE_H_

namespace legend {
namespace scenes {
/**
 * @brief 使用するシーンの列挙
 */
enum class SceneType {
  NONE = 0,
  TITLE,
  GAMEOVER,

  MODEL_VIEW = 100,
  INPUT_TEST,
  SOUND_TEST,
};

class ISceneChange {
 public:
  /**
   * @brief デストラクタ
   */
  virtual ~ISceneChange() = default;
  /**
   * @brief 次のシーンに切り替える
   */
  virtual void ChangeScene(SceneType next_scene) = 0;
};
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_SCENE_CHANGE_H_
