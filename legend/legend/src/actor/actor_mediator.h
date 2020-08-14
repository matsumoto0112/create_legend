#ifndef LEGEND_ACTOR_ACTOR_MEDIATOR_H
#define LEGEND_ACTOR_ACTOR_MEDIATOR_H

/**
 * @file actor_mediator.h
 * @brief 
 */

namespace legend {
namespace actor {
class IActorMediator {
 public:
  virtual ~IActorMediator() = default;
  virtual void PlayerMoveStartEvent() = 0;
  virtual void PlayerMoveEndEvent() = 0;
  virtual void PlayerSkillActivate() = 0;
};

}  // namespace actor
}  // namespace legend
#endif  //! LEGEND_ACTOR_ACTOR_MEDIATOR_H
