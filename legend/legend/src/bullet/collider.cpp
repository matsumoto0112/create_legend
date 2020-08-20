#include "src/bullet/collider.h"

#include "src/bullet/bullet_helper.h"

namespace legend {
namespace bullet {

Collider::Collider(util::Transform* owner_transform)
    : owner_transform_(owner_transform) {}
Collider::~Collider() {}
bool Collider::Update() { return true; }

btRigidBody* Collider::GetRigidBody() { return rigid_body_.get(); }

void Collider::ApplyCentralImpulse(const math::Vector3& impulse) {
  if (rigid_body_ == nullptr) {
    return;
  }

  rigid_body_->applyCentralImpulse(helper::TobtVector3(impulse));
}

btVector3 Collider::GetVelocity() {
  if (rigid_body_ == nullptr) {
    return btVector3(0, 0, 0);
  }
  return rigid_body_->getLinearVelocity();
}

void Collider::SetVelocity(const math::Vector3& velocity) {
  if (rigid_body_ == nullptr) {
    return;
  }
  rigid_body_->activate();
  rigid_body_->setLinearVelocity(helper::TobtVector3(velocity));
}

btVector3 Collider::GetAngularVelocity() {
  if (rigid_body_ == nullptr) {
    return btVector3(0, 0, 0);
  }
  return rigid_body_->getAngularVelocity();
}

void Collider::SetAngularVelocity(const math::Vector3& velocity) {
  if (rigid_body_ == nullptr) {
    return;
  }
  rigid_body_->setAngularVelocity(helper::TobtVector3(velocity));
}

void Collider::updateAction(btCollisionWorld* collisionWorld,
                            btScalar deltaTimeStep) {
  btTransform tr;
  motion_state_->getWorldTransform(tr);
  owner_transform_->SetPosition(helper::ToVector3(tr.getOrigin()));
  owner_transform_->SetRotation(helper::ToQuaternion(tr.getRotation()));

  // MY_LOG(L"Update:(%f, %f, %f)", tr.getOrigin().x(), tr.getOrigin().y(),
  //       tr.getOrigin().z());
}

void Collider::debugDraw(btIDebugDraw* debugDrawer) {
  btTransform tr;
  motion_state_->getWorldTransform(tr);
  // MY_LOG(L"Draw:(%f, %f, %f)", tr.getOrigin().x(), tr.getOrigin().y(),
  //       tr.getOrigin().z());
}

}  // namespace bullet
}  // namespace legend
