#ifndef LEGEND_ACTOR_ACTOR_MANAGER_H_
#define LEGEND_ACTOR_ACTOR_MANAGER_H_

/**
 * @file actor_manager.h
 * @brief アクターの管理者
 */

#include <set>

#include "src/actor/actor_mediator.h"
#include "src/actor/differed_rendering_render_command_list.h"
#include "src/bullet/physics_field.h"
#include "src/camera/camera_manager.h"
#include "src/enemy/enemy_manager.h"
#include "src/object/desk.h"
#include "src/object/fragment.h"
#include "src/object/graffiti.h"
#include "src/player/player.h"
#include "src/search/search_manager.h"
#include "src/skill/skill_item_box.h"
#include "src/stage_generate/stage_generator.h"
#include "src/system/mode.h"

namespace legend {
namespace system {
class TurnSystem;
}  // namespace system
namespace actor {

/**
 * @brief アクターの管理者
 */
class ActorManager : public actor::IActorMediator {
 public:
  /**
   * @brief コンストラクタ
   */
  ActorManager();
  /**
   * @brief デストラクタ
   */
  ~ActorManager();
  /**
   * @brief 初期化
   * @param stage_name ステージ名
   * @param turn_system_ ターン管理オブジェクト
   */
  bool Init(const std::string& stage_name, system::TurnSystem* turn_system_);
  /**
   * @brief 更新処理
   */
  bool Update();
  /**
   * @brief Differed-Renderingによるアクターの描画処理
   */
  void DrawDifferedRenderingObject(directx::device::CommandList& command_list);
  /**
   * @brief Alphaオブジェクトの描画処理
   */
  void DrawAlphaObject(directx::device::CommandList& command_list);
  /**
   * @brief 2Dオブジェクト描画処理
   */
  void Draw2D(directx::device::CommandList& command_list);
  /**
  * @brief 描画終了
  */
  void DrawEnd();

  /**
   * @brief デバッグ描画
   */
  void DebugDraw();
  /**
   * @brief 各アクターの生成
   */
  bool GenerateActors(i32 currnt_turn);
  /**
   * @brief ゲームをクリアしたか
   */
  bool IsGameClear() const;
  /**
   * @brief プレイヤーが止まっているかどうか
   */
  bool IsPlayerStop();
  /**
   * @brief 敵が止まっているかどうか
   */
  bool IsAllEnemeyStop();
  /**
   * @brief ボスが止まっているかどうか
   */
  bool IsBossStop();
  /**
   * @brief 全てのアクターが止まっているかどうか
   */
  bool IsAllActorStop();
  /**
   * @brief エネミーマネージャーの取得
   */
  enemy::EnemyManager* GetEnemyManager();
  /**
   * @brief エネミーマネージャーの更新
   */
  void EnemyManagerUpdate();
  /**
   * @brief 敵の数を取得
   */
  i32 GetEnemiesSize();
  /**
   * @brief ボスが出現したか
   */
  bool IsBossGenerated();
  /**
   * @brief ボスの出現するターンを取得
   */
  i32 GetBossGenerateTurn();

 public:
  /**
   * @brief プレイヤーの移動開始時イベント
   */
  virtual void PlayerMoveStartEvent() override;
  /**
   * @brief プレイヤーの移動終了時イベント
   */
  virtual void PlayerMoveEndEvent() override;
  /**
   * @brief プレイヤーのスキル発動開始時イベント
   */
  virtual void PlayerSkillActivate() override;
  /**
   * @brief プレイヤーのスキル発動終了時イベント
   */
  virtual void PlayerSkillDeactivate() override;
  /**
   * @brief プレイヤーのスキル装備演出終了時イベント
   */
  virtual void PlayerCompleteEquipment() override;
  /**
   * @brief プレイヤーを取得する
   */
  virtual player::Player* GetPlayer() const override;
  /**
   * @brief 敵をシーンから取得する
   */
  virtual std::vector<enemy::Enemy*> GetEnemies() override;
  virtual void AddCollider(std::shared_ptr<bullet::Collider> collider) override;
  virtual void RemoveCollider(
      std::shared_ptr<bullet::Collider> collider) override;
  virtual btCollisionWorld::AllHitsRayResultCallback RayCast(
      const math::Vector3& start, const math::Vector3& end) const override;
  virtual void RemoveActor(actor::Actor* actor) override;
  virtual float GetMainCameraThetaAngle() const override;
  /**
   * @brief 現在のターン状況を取得する
   */
  system::Mode GetCurrentTurn() const override;
  /**
   * @brief 1つ前のターン状況を取得する
   */
  system::Mode GetBeforeTurn() const override;
  /**
   * @brief 停止時間を追加する
   */
  virtual void AddStopTime(float time) override;
  /**
   * @brief 停止時間を指定する
   */
  virtual void SetStopTime(float time) override;

  virtual void AddFragment(std::unique_ptr<object::Fragment> fragment) override;
  virtual camera::CameraManager* GetCameraManager() override {
    return &camera_manager_;
  };

  virtual bool IsHitStopNow() override;

 private:
  /**
   * @brief 消しカスの追加
   */
  // void AddFragment(std::unique_ptr<object::Fragment> fragment);

 private:
  bullet::PhysicsField physics_field_;
  //! ステージ生成
  stage_generate::StageGenerator stage_generator_;
  //! プレイヤー
  std::unique_ptr<player::Player> player_;
  //! 静止オブジェクト
  std::vector<std::unique_ptr<Actor>> static_actors_;
  std::vector<std::unique_ptr<Actor>> alpha_actors_;
  std::vector<std::unique_ptr<Actor>> add_static_actors_;
  std::vector<std::unique_ptr<Actor>> add_alpha_actors_;
  //! 敵管理
  enemy::EnemyManager enemy_manager_;
  //! 分岐管理
  search::SearchManager search_manager_;

  std::set<Actor*> remove_alpha_actors_;
  std::set<Actor*> remove_static_actors_;

  actor::DifferedRenderingRenderCommandList actor_render_command_list_;

  //ターンシステムのポインタ
  system::TurnSystem* turn_system_;

  //! タイマー
  util::CountDownTimer countdown_timer_;
  camera::CameraManager camera_manager_;

  //停止させる時間
  float hit_stop_time_;

  //ボスが生成された後かどうか
  bool is_boss_generated_;
};

}  // namespace actor
}  // namespace legend

#endif  //! LEGEND_ACTOR_ACTOR_MANAGER_H_
