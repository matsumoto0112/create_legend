#ifndef LEGEND_ACTOR_ACTOR_MEDIATOR_H
#define LEGEND_ACTOR_ACTOR_MEDIATOR_H

/**
 * @file actor_mediator.h
 * @brief
 */

namespace legend {
namespace player {
class Player;
}  // namespace player
namespace enemy {
class Enemy;
}  // namespace enemy
}  // namespace legend

namespace legend {
namespace actor {
class IActorMediator {
 public:
  virtual ~IActorMediator() = default;
  /**
   * @brief プレイヤーの移動開始時イベント
   */
  virtual void PlayerMoveStartEvent() = 0;
  /**
   * @brief プレイヤーの移動終了時イベント
   */
  virtual void PlayerMoveEndEvent() = 0;
  /**
   * @brief プレイヤーのスキル発動開始時イベント
   */
  virtual void PlayerSkillActivate() = 0;
  /**
   * @brief プレイヤーのスキル発動終了時イベント
   */
  virtual void PlayerSkillDeactivate() = 0;
  /**
   * @brief プレイヤーを取得する
   */
  virtual player::Player* GetPlayer() = 0;
  /**
   * @brief 敵をシーンから取得する
   */
  virtual std::vector<enemy::Enemy*> GetEnemies() = 0;
};

}  // namespace actor
}  // namespace legend
#endif  //! LEGEND_ACTOR_ACTOR_MEDIATOR_H
