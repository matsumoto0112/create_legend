#ifndef LEGEND_BULLET_TRIGGER_H
#define LEGEND_BULLET_TRIGGER_H

/**
 * @file trigger.h
 * @brief 
 */

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <btBulletDynamicsCommon.h>

#include "src/math/quaternion.h"
#include "src/util/transform.h"

namespace legend {
namespace actor {
class Actor;
}  // namespace actor

namespace bullet {
class Trigger : public btActionInterface {
  using TriggerCallback = std::function<void(Trigger* other)>;

 public:
  Trigger(actor::Actor* owner);
  virtual ~Trigger();
  virtual bool Update();
  virtual void updateAction(btCollisionWorld* collisionWorld,
                            btScalar deltaTimeStep) override;
  virtual void debugDraw(btIDebugDraw* debugDrawer) override;
  void OnTrigger(Trigger* other);

  btGhostObject* GetGhostObject() const { return ghost_.get(); }
  void SetTriggerCallback(TriggerCallback callback) {
    this->callback_ = callback;
  }

 protected:
  actor::Actor* owner_;
  std::shared_ptr<btCollisionShape> shape_;
  std::shared_ptr<btGhostObject> ghost_;
  TriggerCallback callback_;
};

}  // namespace bullet
}  // namespace legend
#endif  //! LEGEND_BULLET_TRIGGER_H
