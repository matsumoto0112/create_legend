#include "src/bullet/bounding_sphere.h"

#include "src/bullet/bullet_helper.h"

namespace legend {
namespace bullet {
BoundingSphere::BoundingSphere(actor::Actor* owner,
                               const InitializeParameter& parameter)
    : Collider(owner) {
  //立方体に設定
  shape_ = std::make_shared<btSphereShape>(parameter.radius);

  motion_state_ = std::make_shared<btDefaultMotionState>(
      btTransform(helper::TobtQuaternion(parameter.rotation),
                  helper::TobtVector3(parameter.position)));

  //慣性モーメントの計算
  inertia_ = btVector3(0, 0, 0);
  shape_->calculateLocalInertia(parameter.mass, inertia_);

  //剛体オブジェクト生成
  rigid_body_ = std::make_shared<btRigidBody>(
      parameter.mass, motion_state_.get(), shape_.get(), inertia_);
  rigid_body_->setUserPointer(this);

  //摩擦係数を設定
  rigid_body_->setFriction(parameter.friction);
}
BoundingSphere::~BoundingSphere() {
  if (rigid_body_) {
    rigid_body_->setUserPointer(nullptr);
    rigid_body_->setMotionState(nullptr);
    rigid_body_->setCollisionShape(nullptr);
  }
}

bool BoundingSphere::Update() { return true; }

}  // namespace bullet
}  // namespace legend
