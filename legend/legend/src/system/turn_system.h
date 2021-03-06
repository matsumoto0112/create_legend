#ifndef LEGEND_SYSTEM_TURN_SYSTEM_H_
#define LEGEND_SYSTEM_TURN_SYSTEM_H_

/**
 * @file turn_system.h
 * @brief ターンによるゲーム進行管理クラス定義
 */

#include <set>

#include "src/actor/actor_manager.h"
#include "src/camera/follow_camera.h"
#include "src/camera/perspective_camera.h"
#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/scenes/fade_in_out.h"
#include "src/system/game_data.h"
#include "src/system/mode.h"
#include "src/turn_change/turn_change.h"
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
class TurnSystem {
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
   * @brief ゲームが終了したかどうか
   */
  bool IsGameEnd() const;
  /**
   * @brief ゲーム結果を取得する
   */
  system::GameDataStorage::ResultData GetResult() const;
  /**
   * @brief ターンのモードを設定する
   */
  void SetTurnMode(Mode mode);
  Mode GetCurrentMode();
  /**
   * @brief 1つ前のターン状態を取得
   */
  Mode GetBeforeMode();
  /**
   * @brief プレイヤーターンになる演出
   */
  bool ToPlayerTurn();
  /**
   * @brief 敵のターンになる演出
   */
  bool ToEnemyTurn();
  /**
   * @brief ボス出現ターンの取得
   */
  i32 GetBossGenerateTurn();
  /**
   * @brief 現在のターン数を取得
   */
  i32 GetCurrentTurn();

 private:
  /**
   * @brief ターン数の増加
   */
  void AddCurrentTurn();
  /**
   * @brief 表示用のターン数を表示
   */
  i32 GetViewTurn();
  /**
   * @brief プレイヤーの移動前準備処理
   */
  bool PlayerMoveReady();
  /**
   * @brief プレイヤーの移動処理
   */
  bool PlayerMoving();

  bool WaitEnemyMoveStart();

  /**
   * @brief プレイヤーの移動終了後のスキル発動処理
   */
  bool PlayerSkillAfterMoved();
  /**
   * @brief スキル追加演出処理
   */
  bool AddSkill();

  /**
   * @brief 敵の移動処理
   */
  bool EnemyMove();
  /**
   * @brief 敵の移動終了時処理
   */
  bool EnemyMoveEnd();
  /**
   * @brief 敵の移動終了時処理
   */
  bool EnemyMoveProducing();
  /**
   * @brief ボスの移動終了時処理
   */
  bool BossMoveProducing();
  /**
   * @brief 敵の生成処理
   */
  bool EnemyGenerate();

  void ChangeUIRenderEnable(bool enabler);

 private:
  //! 現在ターン数
  i32 current_turn_;
  i32 view_turn_;
  //! 現在のプレイ状態
  Mode current_mode_;
  //! 1つ前のプレイ状態
  Mode before_mode_;

  //! アクター管理クラス
  actor::ActorManager actor_manager_;
  //! ターン変更演出用クラス
  turn_change::TurnChange turn_change_;

  //! UI表示ボード
  ui::UIBoard ui_board_;
  //! UIゲージ配列
  std::vector<ui::Gauge*> gauges_;
  //! 数値画像配列
  std::vector<ui::Number*> numbers_;
  std::vector<ui::UIComponent*> no_render_if_enemy_turn_uis_;

  scenes::FadeInOut fade_;
  bool is_scene_all_end_;
  bool is_scene_end_fade_start_;

  /**
   * @brief ライト情報
   */
  struct LightCBStruct {
    legend::math::Vector4 light_position;
    legend::util::Color4 light_color;
  };

  legend::directx::buffer::ConstantBuffer<LightCBStruct> light_cb_;


 private:
  // UIのリアルタイム修正用パラメータ
  std::vector<legend::ui::UIComponent*> components_;
  std::vector<std::vector<std::string>> input_lines_;
};

}  // namespace system
}  // namespace legend

#endif  //! LEGEND_SYSTEM_TURN_SYSTEM_H_
