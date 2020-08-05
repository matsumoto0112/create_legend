#ifndef LEGEND_SCENES_DEBUGSCENE_PLAYER_MOVE_VIEWER_H_
#define LEGEND_SCENES_DEBUGSCENE_PLAYER_MOVE_VIEWER_H_

#include "src/camera/perspective_camera.h"
#include "src/object/desk.h"
#include "src/player/player.h"
#include "src/scenes/scene.h"

namespace legend {
namespace scenes {
namespace debugscene {

class PlayerMoveViewer : public Scene {
 public:
  /**
   * @brief コンストラクタ
   */
  PlayerMoveViewer(ISceneChange* scene_change);
  /**
   * @brief デストラクタ
   */
  ~PlayerMoveViewer();
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
  //! メインカメラ
  camera::PerspectiveCamera camera_;
  //! プレイヤー
  player::Player player_;
  //! 机
  object::Desk desk_;
};

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DEBUGSCENE_PLAYER_MOVE_VIEWER_H_
