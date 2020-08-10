#ifndef LEGEND_ENEMY_ENEMY_MANAGER_H_
#define LEGEND_ENEMY_ENEMY_MANAGER_H_

#include <vector>

#include "src/enemy/enemy.h"
#include "src/player/player.h"
#include "src/system/physics_field.h"

namespace legend {
namespace enemy {
/**
 * @class EnemyManager
 * @brief エネミー管理クラス
 */
class EnemyManager {
 public:
  /**
   * @brief コンストラクタ
   */
  EnemyManager();
  /**
   * @デスクトラクタ
   */
  ~EnemyManager();
  /**
   * @brief 初期化
   */
  bool Initilaize();
  /**
   * @brief 更新
   */
  bool Update();
  /**
   * @brief 描画
   */
  void Draw();
  /**
   * @brief 敵の行動処理
   */
  void EnemyAction();
  /**
   * @brief 敵を追加
   */
  void Add(const Enemy::InitializeParameter& paramater);
  /**
   * @brief 敵を削除
   */
  void Destroy(i32 index);

  void SetPosition(Enemy* enemy);

  /**
   * @brief 最後の敵を取得
   */
  Enemy* GetLastEnemy() const;
  /**
   * @brief obbの座標を基に座標更新
   */
  void SetPosition();
  /**
   * @brief 敵の数を取得
   */
  i32 GetEnemiesSize() const;
  /**
   * @brief obbの速度を基に速度更新
   */
  void SetVelocity();
  /**
   * @brief 各敵の速度を取得
   */
  std::vector<math::Vector3> GetVelocities();
  /**
   * @brief 最後の敵の移動終了判定の取得
   */
  bool LastEnemyMoveEnd() const;

  void DebugDraw(directx::device::CommandList& command_list);

 private:
  std::vector<std::unique_ptr<Enemy>> enemys_;
  i32 action_enemy_index_ = -1;
  i32 enemy_max_count_ = 50;
  float move_timer_ = 0.0f;
  const float move_time_ = 0.25f;

  //! 各敵の速度格納リスト
  std::vector<math::Vector3> velocities_;
  std::unique_ptr<player::Player> player_;
  std::unique_ptr<system::PhysicsField> physics_field_;
};
}  // namespace enemy
}  // namespace legend

#endif LEGEND_ENEMY_ENEMY_MANAGER_H_