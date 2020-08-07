#ifndef LEGEND_ENEMY_ENEMY_MANAGER_H_
#define LEGEND_ENEMY_ENEMY_MANAGER_H_

#include <vector>

#include "src/enemy/enemy.h"
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
  bool Initilaize(system::PhysicsField* physics_field = nullptr);
  /**
   * @brief 更新
   */
  bool Update(system::PhysicsField* physics_field = nullptr);
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
  void Add(system::PhysicsField* physics_field = nullptr);
  /**
   * @brief 敵を削除
   */
  void Destroy(i32 index, system::PhysicsField* physics_field = nullptr);

  void SetPosition(Enemy* enemy);

  Enemy* GetLastEnemy() const;
  void SetPosition(system::PhysicsField* physics_field);
  float GetEnemyPower() const;
  i32 GetEnemiesSize() const;
  void SetVelocity(system::PhysicsField* physics_field, i32 index_num);
  std::vector<math::Vector3> GetVelocities() const;
  bool LastEnemyMoveEnd() const;

 private:
  std::vector<std::unique_ptr<Enemy>> enemys_;
  i32 action_enemy_index_ = -1;
  i32 enemy_max_count_ = 50;
  float move_timer_ = 0.0f;
  const float move_time_ = 0.25f;
};
}  // namespace enemy
}  // namespace legend

#endif LEGEND_ENEMY_ENEMY_MANAGER_H_