#ifndef LEGEND_ACTOR_ACTOR_RENDER_COMMAND_LIST_H
#define LEGEND_ACTOR_ACTOR_RENDER_COMMAND_LIST_H

/**
 * @file actor_render_command_list.h
 * @brief
 */

#include "src/actor/actor.h"
#include "src/object/back_ground.h"

namespace legend {
namespace actor {
class ActorRenderCommandList {
 public:
  ActorRenderCommandList();
  ~ActorRenderCommandList();

  void Push(Actor* actor);

  void Clear();
  void ShadowPass();
  void RenderPass();

 private:
  std::vector<Actor*> actor_list_;
  object::BackGround back_ground_;
};

}  // namespace actor
}  // namespace legend
#endif  //! LEGEND_ACTOR_ACTOR_RENDER_COMMAND_LIST_H
