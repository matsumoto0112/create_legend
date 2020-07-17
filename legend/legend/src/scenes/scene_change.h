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
  SOUND_TEST,
  PERSPECTIVE_CAMERA_TEST,
  SPRITE_TEST,
  INPUT_TEST,
  PHYSICS_TEST,
  POST_PROCES_VIEWER,
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
  /**
   * @brief 現在のシーンの取得
   */
  virtual SceneType GetCurrentSceneType() const = 0;
};
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_SCENE_CHANGE_H_
