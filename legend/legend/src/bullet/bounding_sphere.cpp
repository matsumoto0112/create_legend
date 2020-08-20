#include "bounding_sphere.h"

#include "src\\stdafx.h"

namespace legend {
namespace bullet {
BoundingSphere::BoundingSphere(util::Transform* owner_transform,
                               const InitializeParameter& parameter)
    : Collider(owner_transform) {
  //�����̂ɐݒ�
  shape_ = std::make_shared<btSphereShape>(parameter.radius);

  motion_state_ = std::make_shared<btDefaultMotionState>(
      btTransform(parameter.rotation, parameter.position));

  //�������[�����g�̌v�Z
  inertia_ = btVector3(0, 0, 0);
  shape_->calculateLocalInertia(parameter.mass, inertia_);

  //���̃I�u�W�F�N�g����
  rigid_body_ = std::make_shared<btRigidBody>(
      parameter.mass, motion_state_.get(), shape_.get(), inertia_);

  //���C�W����ݒ�
  rigid_body_->setFriction(parameter.friction);
}
BoundingSphere::~BoundingSphere() {}
bool BoundingSphere::Update() { return false; }
}  // namespace bullet
}  // namespace legend
