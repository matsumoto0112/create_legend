#ifndef LEGEND_SCENES_SCENE_MANAGER_H_
#define LEGEND_SCENES_SCENE_MANAGER_H_

#include "src/scenes/scene.h"
#include "src/scenes/scene_change.h"

namespace legend {
namespace scenes {

/**
 * @class SceneManager
 * @brief シーン管理
 */
class SceneManager : public ISceneChange, Task {
 public:
  /**
   * @brief コンストラクタ
   */
  SceneManager();
  /**
   * @brief 初期化
   */
  bool Initialize() override;
  /**
   * @brief 終了
   */
  void Finalize() override;
  /**
   * @brief 更新
   */
  bool Update() override;
  /**
   * @brief 描画
   */
  void Draw() override;
  /**
   * @brief シーン遷移
   */
  void ChangeScene(SceneType next_scene) override;
  /**
   * @brief 現在のシーンの取得
   */
  SceneType GetCurrentSceneType() const override;

 private:
  //! 現在のシーン
  std::unique_ptr<Scene> current_scene_;
  //! 次のシーン
  SceneType next_scene_;
  //! 現在のシーン取得用変数
  SceneType current_scene_type_;
};

}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_SCENE_MANAGER_H_
