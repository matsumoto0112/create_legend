#include "src/bullet/collider.h"

#include "src/bullet/bullet_helper.h"

namespace legend {
namespace bullet {

Collider::Collider(util::Transform* owner_transform)
    : owner_transform_(owner_transform) {}
Collider::~Collider() {}
bool Collider::Update() { return true; }

btRigidBody* Collider::GetRigidBody() { return rigid_body_.get(); }

void Collider::ApplyCentralImpulse(btVector3 impulse) {
  if (rigid_body_ == nullptr) {
    return;
  }

  rigid_body_->applyCentralImpulse(impulse);
}

btVector3 Collider::GetVelocity() {
  if (rigid_body_ == nullptr) {
    return btVector3(0, 0, 0);
  }
  return rigid_body_->getLinearVelocity();
}

void Collider::SetVelocity(btVector3 velocity) {
  if (rigid_body_ == nullptr) {
    return;
  }
  rigid_body_->setLinearVelocity(velocity);
}

btVector3 Collider::GetAngularVelocity() {
  if (rigid_body_ == nullptr) {
    return btVector3(0, 0, 0);
  }
  return rigid_body_->getAngularVelocity();
}

void Collider::SetAngularVelocity(btVector3 velocity) {
  if (rigid_body_ == nullptr) {
    return;
  }
  rigid_body_->setAngularVelocity(velocity);
}

void Collider::updateAction(btCollisionWorld* collisionWorld,
                            btScalar deltaTimeStep) {
  btTransform tr;
  motion_state_->getWorldTransform(tr);
  owner_transform_->SetPosition(helper::ToVector3(tr.getOrigin()));
  owner_transform_->SetRotation(helper::ToQuaternion(tr.getRotation()));
}

void Collider::debugDraw(btIDebugDraw* debugDrawer) {}

}  // namespace bullet
}  // namespace legend
