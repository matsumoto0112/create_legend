#ifndef LEGEND_SCENES_DEBUGSCENE_ENEMY_MOVE_VIEWER_H_
#define LEGEND_SCENES_DEBUGSCENE_ENEMY_MOVE_VIEWER_H_

#include "src/camera/perspective_camera.h"
#include "src/enemy/enemy_manager.h"
#include "src/object/desk.h"
#include "src/object/obstacle.h"
#include "src/player/player.h"
#include "src/scenes/scene.h"
#include "src/search/search_manager.h"
#include "src/system/physics_field.h"
#include "src/system/turn_system.h"

namespace legend {
namespace scenes {
namespace debugscene {

/**
 * @class EnemyMoveViewer
 * @brief 敵挙動シーン1
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
   * @brief 終了
   */
  void Finalize() override;
  /**
   * @brief ターン別の更新処理
   */
  bool UpdateTurn();

 private:
  camera::PerspectiveCamera camera_;
  system::Turn turn_;
  system::TurnSystem current_turn_;
  system::PhysicsField physics_field_;
  //! プレイヤー
  player::Player player_;
  //! 机
  std::vector<object::Desk> desks_;
  //! エネミー
  enemy::EnemyManager enemy_manager_;
  //! 探索管理
  search::SearchManager search_manager_;
  //! 障害物リスト
  std::vector<object::Obstacle> obstacles_;
};

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DEBUGSCENE_ENEMY_MOVE_VIEWER_H_
