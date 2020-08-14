#ifndef LEGEND_PHYSICS_COLLIDER_H_
#define LEGEND_PHYSICS_COLLIDER_H_

/**
 * @file collider.h
 */

#include "src/actor/actor_type.h"
#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/constant_buffer_structure.h"
#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/directx/device/command_list.h"
#include "src/directx/vertex.h"
#include "src/util/transform.h"

namespace legend {
namespace physics {
class Collider {
 public:
  using CollisionCallback = std::function<void(actor::ActorType)>;

 public:
  Collider(const util::Transform& transform, const std::wstring& name);
  virtual ~Collider();
  virtual bool Init();
  virtual void Update();
  virtual void OnCollisionHit(actor::ActorType other_actor_type) {
    if (collision_callback_) {
      collision_callback_(other_actor_type);
    }
  };
  virtual void OnTriggerHit(actor::ActorType other_actor_type) {
    if (trigger_callback_) {
      trigger_callback_(other_actor_type);
    };
  }
  virtual void DebugDraw(directx::device::CommandList& command_list) {}
  virtual void SetCollisionCallback(CollisionCallback callback) {
    this->collision_callback_ = callback;
  }
  virtual void SetTriggerCallback(CollisionCallback callback) {
    this->trigger_callback_ = callback;
  }

 protected:
  std::wstring name_;
  //! トランスフォーム
  util::Transform transform_;
  CollisionCallback collision_callback_;
  CollisionCallback trigger_callback_;
};

}  // namespace physics
}  // namespace legend
#endif  //! LEGEND_PHYSICS_COLLIDER_H_
