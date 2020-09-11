#ifndef LEGEND_ENEMY_ENEMY_MANAGER_H_
#define LEGEND_ENEMY_ENEMY_MANAGER_H_

#include <vector>

#include "src/actor/actor_mediator.h"
#include "src/actor/differed_rendering_render_command_list.h"
#include "src/enemy/boss.h"
#include "src/enemy/enemy.h"
#include "src/enemy/enemy_actor.h"
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
   * @brief デスクトラクタ
   */
  ~EnemyManager();
  /**
   * @brief 初期化
   * @param mediator 仲介者
   */
  bool Initilaize(actor::IActorMediator* mediator);
  /**
   * @brief 更新
   * @param search_manaegr 探索管理
   * @return 更新したらTRUE
   */
  bool Update(search::SearchManager* search_manaegr);
  /**
   * @brief 描画
   * @param render_command_list 描画コマンドリスト
   */
  void Draw(actor::DifferedRenderingRenderCommandList* render_command_list);
  /**
   * @brief 敵の行動処理
   * @param search_manaegr 探索管理
   */
  void EnemyAction(search::SearchManager* search_manaegr);
  /**
   * @brief 敵を追加
   * @param paramater 初期化パラメータ
   */
  void AddEnemy(const EnemyActor::InitializeParameter& paramater);
  /**
   * @brief ボスを追加
   * @param paramater 初期化パラメータ
   */
  void AddBoss(const EnemyActor::InitializeParameter& paramater);
  /**
   * @brief ボスを追加
   * @return 回収終了
   */
  bool AbsorpEnemies();
  /**
   * @brief 削除処理
   */
  void DestroyUpdate();
  /**
   * @brief 敵を削除
   * @param index 削除番号
   */
  void Destroy(i32 index);
  /**
   * @brief ボスを削除
   */
  void DestroyBoss();
  /**
   * @brief ポジション設定
   * @param enemy 敵
   */
  void SetPosition(Enemy* enemy);
  /**
   * @brief 最後の敵を取得
   * @return 配列最後尾の敵取得
   */
  Enemy* GetLastEnemy() const;
  /**
   * @brief 敵の数を取得
   * @return 敵配列取得
   */
  i32 GetEnemiesSize() const;
  /**
   * @brief 敵の行動を初期化
   */
  void ResetEnemyMove();
  /**
   * @brief 各敵の速度を取得
   * @return 敵の速度配列
   */
  std::vector<math::Vector3> GetVelocities();
  /**
   * @brief 最後の敵の移動終了判定の取得
   * @return 配列最後尾の敵が停止しているか判定
   */
  bool LastEnemyMoveEnd() const;
  /**
   * @brief ボスの取得
   * @return ボスの取得
   */
  enemy::Boss* GetBoss() const { return boss_.get(); };
  /**
   * @brief クリア判定
   * @return クリア判定
   */
  bool IsGameClear() const { return is_game_clear_; };
  /**
   * @brief プレイヤー設定
   * @param player_obb プレイヤー長とつ判定
   */
  void SetPlayer(bullet::Collider* player_obb);
  /**
   * @brief 敵のポインタリスト取得
   * @return 敵のポインタリスト取得
   */
  std::vector<Enemy*> GetEnemyPointers() const;

 private:
  //! 敵リスト
  std::vector<std::unique_ptr<Enemy>> enemys_;
  //! ボス
  std::unique_ptr<Boss> boss_;
  //! 行動的番号
  i32 action_enemy_index_ = -1;
  //! 敵の最大生成数
  i32 enemy_max_count_ = 50;
  //! クリア判定
  bool is_game_clear_ = false;
  //! 移動速度最大値
  float move_speed_max_ = 60.0f;
  //! 移動速度最低値
  float move_speed_min_ = 20.0f;
  //! 稼働移動時間
  float move_timer_ = 0.0f;
  //! 移動時間
  const float move_time_ = 0.5f;

  //! 各敵の速度格納リスト
  std::vector<math::Vector3> velocities_;
  //! プレイヤーの衝突判定
  bullet::Collider* player_collider_;

  //! 仲介者
  actor::IActorMediator* actor_mediator_;
};
}  // namespace enemy
}  // namespace legend

#endif LEGEND_ENEMY_ENEMY_MANAGER_H_