#ifndef LEGEND_SYSTEM_TURN_SYSTEM_H_
#define LEGEND_SYSTEM_TURN_SYSTEM_H_

/**
 * @file turn_system.h
 * @brief ターンによるゲーム進行管理クラス定義
 */

#include "src/actor/actor_mediator.h"
#include "src/bullet/physics_field.h"
#include "src/camera/follow_camera.h"
#include "src/camera/perspective_camera.h"
#include "src/enemy/enemy_manager.h"
#include "src/object/desk.h"
#include "src/object/fragment.h"
#include "src/object/graffiti.h"
#include "src/player/player.h"
#include "src/search/search_manager.h"
#include "src/stage_generate/stage_generator.h"
#include "src/system/mode.h"
#include "src/ui/gauge.h"
#include "src/ui/image.h"
#include "src/ui/number.h"
#include "src/ui/ui_board.h"
#include "src/util/timer.h"

namespace legend {
namespace system {
namespace camera_mode {
enum Enum { Main, Sub1, Sub2, COUNT };
}  // namespace camera_mode
namespace ui_format {
/**
 * @brief UIのファイルに格納してあるパラメータ
 */
enum Enum { NAME, X, Y, Z, ID, MAX };
}  // namespace ui_format
/**
 * @brief UIファイルに格納しているゲージの順
 */
namespace gauge_id {
enum Enum {
  PLAYER_CHARGE_POWER,
  PLAYER_STRENGTHENED_STATE_0,
  PLAYER_STRENGTHENED_STATE_1,
  PLAYER_STRENGTHENED_STATE_2,
  MAX
};
}  // namespace gauge_id
/**
 * @brief UIファイルに格納している数値UIの順
 */
namespace number_id {
enum Enum { DIGIT_3, DIGIT_2, DIGIT_1, MAX };
}  // namespace number_id

/**
 * @class TurnSystem
 * @brief ターン数管理クラス
 */
class TurnSystem : public actor::IActorMediator {
 public:
  /**
   * @brief コンストラクタ
   */
  TurnSystem();
  /**
   * @brief デストラクタ
   */
  virtual ~TurnSystem();
  /**
   * @brief 初期化
   * @param stage_name ステージ名
   */
  bool Init(const std::string& stage_name);
  /**
   * @brief 更新処理
   */
  bool Update();
  /**
   * @brief 描画処理
   */
  void Draw();
  /**
   * @brief デバッグ描画
   */
  void DebugDraw();
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
   * @brief プレイヤーを取得する
   */
  virtual player::Player* GetPlayer() override;
  /**
   * @brief 敵をシーンから取得する
   */
  virtual std::vector<enemy::Enemy*> GetEnemies() override;
  virtual void AddCollider(std::shared_ptr<bullet::Collider> collider) override;

 private:
  /**
   * @brief ターン数の増加
   */
  void AddCurrentTurn();
  /**
   * @brief 現在のターン数を取得
   */
  i32 GetCurrentTurn();
  /**
   * @brief プレイヤーの移動前準備処理
   */
  bool PlayerMoveReady();
  /**
   * @brief プレイヤーの移動処理
   */
  bool PlayerMoving();
  /**
   * @brief プレイヤーの移動終了後のスキル発動処理
   */
  bool PlayerSkillAfterModed();
  /**
   * @brief 敵の移動処理
   */
  bool EnemyMove();
  /**
   * @brief 敵の移動終了時処理
   */
  bool EnemyMoveEnd();

  /**
   * @brief カメラの初期化
   */
  bool InitCameras();
  /**
   * @brief 落書きの削除
   */
  void RemoveGraffiti();
  /**
   * @brief 落書き更新処理
   */
  void UpdateGraffiti();
  /**
   * @brief 消しカスの削除
   */
  void RemoveFragment();

 private:
  //! 現在ターン数
  i32 current_turn_;
  //! 現在のプレイ状態
  Mode current_mode_;
  ////! 物理フィールド
  // PhysicsField physics_field_;
  //! ステージ生成
  stage_generate::StageGenerator stage_generator_;
  ////! メインカメラ
  // camera::FollowCamera main_camera_;
  //! プレイヤー
  player::Player player_;
  //! 机
  std::vector<object::Desk> desks_;
  //! 障害物
  std::vector<object::Obstacle> obstacles_;
  //! 落書き
  std::vector<object::Graffiti> graffities_;
  //! 消しカス
  std::vector<object::Fragment> fragments_;
  //! 敵管理
  enemy::EnemyManager enemy_manager_;
  //! 分岐管理
  search::SearchManager search_manager_;
  //! タイマー
  util::CountDownTimer countdown_timer_;

  //! 使用するカメラ
  std::array<std::unique_ptr<camera::Camera>, camera_mode::COUNT> cameras_;
  //! 現在使用しているカメラ
  camera_mode::Enum current_camera_;

  //! UI表示ボード
  ui::UIBoard ui_board_;
  //! UIゲージ配列
  std::vector<ui::Gauge*> gauges_;
  //! 数値画像配列
  std::vector<ui::Number*> numbers_;

  bullet::PhysicsField physics_field_;

 private:
  // UIのリアルタイム修正用パラメータ
  std::vector<legend::ui::UIComponent*> components_;
  std::vector<std::vector<std::string>> input_lines_;
};

}  // namespace system
}  // namespace legend

#endif  //! LEGEND_SYSTEM_TURN_SYSTEM_H_
