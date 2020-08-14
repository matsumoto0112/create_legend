#ifndef LEGEND_SYSTEM_TURN_SYSTEM_H_
#define LEGEND_SYSTEM_TURN_SYSTEM_H_

/**
 * @brief ターンによるゲーム進行管理クラス定義
 */

#include "src/actor/actor_mediator.h"
#include "src/camera/perspective_camera.h"
#include "src/enemy/enemy_manager.h"
#include "src/object/desk.h"
#include "src/object/fragment.h"
#include "src/object/graffiti.h"
#include "src/player/player.h"
#include "src/stage_generate/stage_generator.h"
#include "src/system/mode.h"
#include "src/util/timer.h"

namespace legend {
namespace system {

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
  bool Init(const std::string& stage_name);
  bool Update();

  bool PlayerMove();
  bool EnemyMove();
  bool EnemyMoveEnd();

  void Draw();
  void DebugDraw();
  virtual void PlayerMoveStartEvent() override;
  virtual void PlayerMoveEndEvent() override;
  virtual void PlayerSkillActivate() override;

 private:
  /**
   * @brief ターン数の増加
   */
  void AddCurrentTurn();
  /**
   * @brief 現在のターン数を取得
   */
  i32 GetCurrentTurn();

 private:
  i32 current_turn_;
  Mode current_mode_;
  PhysicsField physics_field_;
  stage_generate::StageGenerator stage_generator_;

  camera::PerspectiveCamera main_camera_;
  player::Player player_;
  std::vector<object::Desk> desks_;
  std::vector<object::Obstacle> obstacles_;
  std::vector<object::Graffiti> graffities_;
  enemy::EnemyManager enemy_manager_;
  util::CountDownTimer countdown_timer_;
};

}  // namespace system
}  // namespace legend

#endif  //! LEGEND_SYSTEM_TURN_SYSTEM_H_
