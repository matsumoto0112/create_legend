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
 * @brief メインシーンその1
 */
class MainScene1 : public Scene {
 public:
  /**
   * @brief コンストラクタ
   */
  MainScene1(ISceneChange* scene_change);
  /**
   * @brief デストラクタ
   */
  ~MainScene1();
  /**
   * @brief 初期化
   */
  bool Initialize() override;
  /**
   * @brief 更新
   */
  bool Update() override;
  /**
   * @brief 描画
   */
  void Draw() override;
  /**
   * @brief 終了
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
