#ifndef LEGEND_SCENES_DEBUGSCENE_ENEMY_MOVE_VIEWER_H_
#define LEGEND_SCENES_DEBUGSCENE_ENEMY_MOVE_VIEWER_H_

/**
 * @brief モデル描画テストシーン
 * @details モデルを一つ読み込み、アルベドテクスチャを張った状態で描画するシーン
 パラメータとしてモデルの座標、回転、スケーリング、カメラの座標、回転、fovが操作できる
 */

#include "src/camera/perspective_camera.h"
#include "src/scenes/scene.h"
#include "src/enemy/enemy_manager.h"

namespace legend {
namespace scenes {
namespace debugscene {

/**
 * @class ModelView
 * @brief モデル描画テストシーン
 */
class EnemyMoveViewer : public Scene {
 public:
  /**
   * @brief コンストラクタ
   */
  EnemyMoveViewer(ISceneChange* scene_change);
  /**
   * @brief デストラクタ
   */
  ~EnemyMoveViewer();
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
   * @brief 終了処理
   */
  void Finalize() override;

 private:
  //! メインカメラ
  camera::PerspectiveCamera camera_;
  //! 敵管理システム
  enemy::EnemyManager enemy_manager_;
  physics::Plane plane_;
};

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DEBUGSCENE_ENEMY_MOVE_VIEWER_H_
