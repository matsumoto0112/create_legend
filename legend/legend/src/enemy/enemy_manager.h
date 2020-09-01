#ifndef LEGEND_ENEMY_ENEMY_MANAGER_H_
#define LEGEND_ENEMY_ENEMY_MANAGER_H_

#include <vector>

#include "src/actor/actor_mediator.h"
#include "src/actor/actor_render_command_list.h"
#include "src/enemy/enemy_actor.h"
#include "src/enemy/boss.h"
#include "src/enemy/enemy.h"
#include "src/player/player.h"
#include "src/search/search_manager.h"

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
  bool Initilaize(actor::IActorMediator* mediator);
  /**
   * @brief 更新
   */
  bool Update(search::SearchManager* search_manaegr);
  /**
   * @brief 描画
   */
  void Draw(actor::ActorRenderCommandList* render_command_list);
  /**
   * @brief 敵の行動処理
   */
  void EnemyAction(search::SearchManager* search_manaegr);
  /**
   * @brief 敵を追加
   */
  void AddEnemy(const EnemyActor::InitializeParameter& paramater);
  /**
   * @brief ボスを追加
   */
  void AddBoss(const EnemyActor::InitializeParameter& paramater);
  /**
   * @brief 削除処理
   */
  void DestroyUpdate();
  /**
   * @brief 敵を削除
   */
  void Destroy(i32 index);
  /**
   * @brief ボスを削除
   */
  void DestroyBoss();
  /**
   * @brief 敵を削除
   */
  // void RandomDestroy(system::PhysicsField& physics_field);

  void SetPosition(Enemy* enemy);

  /**
   * @brief 最後の敵を取得
   */
  Enemy* GetLastEnemy() const;
  /**
   * @brief 敵の数を取得
   */
  i32 GetEnemiesSize() const;
  /**
   * @brief 敵の行動を初期化
   */
  void ResetEnemyMove();
  /**
   * @brief 各敵の速度を取得
   */
  std::vector<math::Vector3> GetVelocities();
  /**
   * @brief 最後の敵の移動終了判定の取得
   */
  bool LastEnemyMoveEnd() const;
  /**
   * @brief ボスの取得
   */
  enemy::Boss* GetBoss() const { return boss_.get(); };
  /**
   * @brief 最後の敵の移動終了判定の取得
   */
  bool IsGameClear() const { return is_game_clear_; };

  void SetPlayer(bullet::Collider* player_obb);

  void DebugDraw(directx::device::CommandList& command_list);

  std::vector<Enemy*> GetEnemyPointers() const;

 private:
  std::vector<std::unique_ptr<Enemy>> enemys_;
  std::unique_ptr<Boss> boss_;
  i32 action_enemy_index_ = -1;
  i32 enemy_max_count_ = 50;
  bool is_game_clear_ = false;
  float move_speed_max_ = 60.0f;
  float move_speed_min_ = 20.0f;
  float move_timer_ = 0.0f;
  const float move_time_ = 0.5f;

  //! 各敵の速度格納リスト
  std::vector<math::Vector3> velocities_;
  bullet::Collider* player_collider_;

  actor::IActorMediator* actor_mediator_;
};
}  // namespace enemy
}  // namespace legend

#endif LEGEND_ENEMY_ENEMY_MANAGER_H_