#ifndef LEGEND_SYSTEM_PHYSICS_FIELD_H_
#define LEGEND_SYSTEM_PHYSICS_FIELD_H_

#include "src/object/desk.h"
#include "src/player/player.h"
#include "src/system/turn_system.h"

namespace legend {
namespace system {

/**
 * @class PhysicsField
 * @brief フィールドの管理クラス
 */
class PhysicsField {
 public:
  /**
   * @brief コンストラクタ
   */
  PhysicsField();
  /**
   * @brief デストラクタ
   */
  ~PhysicsField();
  /**
   * @brief プレイヤーの初期化
   */
  bool PlayerInit(player::Player::InitializeParameter parameter);
  /**
   * @brief エネミーの初期化
   */
  // bool EnemyInit(エネミー::InitializeParameter parameter);
  /**
   * @brief 机の初期化
   */
  bool DeskInit(object::Desk::InitializeParameter parameter);
  /**
   * @brief 更新
   */
  bool Update();
  /**
   * @brief 描画
   */
  void Draw();
  /**
   * @brief エネミーあたり判定の登録
   */
  // void AddEnemy(エネミー enemy);
  /**
   * @brief 机のあたり判定の登録
   */
  void AddDesk(object::Desk desk);
  /**
   * @brief 重力落下
   */
  void UpdateGravity(float gravity);
  /**
   * @brief 減衰処理
   */
  void Deceleration(float deceleration_rate);

 private:
  Turn turn_;
  TurnSystem current_turn_;

  player::Player player_;
  // std::vector<エネミー> enemies_;
  std::vector<object::Desk> desks_;

  //重力
  float gravity_;

  //更新時間(deltaTime)
  float update_time_;
};

}  // namespace system
}  // namespace legend

#endif  //! LEGEND_SYSTEM_PHYSICS_FIELD_H_
