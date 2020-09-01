#include "src/bullet/bounding_sphere.h"

#include "src/bullet/bullet_helper.h"

namespace legend {
namespace bullet {
BoundingSphere::BoundingSphere(actor::Actor* owner,
                               const InitializeParameter& parameter)
    : Collider(owner) {
  //���̂ɐݒ�
  shape_ = std::make_shared<btSphereShape>(parameter.radius);

  motion_state_ = std::make_shared<btDefaultMotionState>(
      btTransform(helper::TobtQuaternion(parameter.rotation),
                  helper::TobtVector3(parameter.position)));

  //�������[�����g�̌v�Z
  inertia_ = btVector3(0, 0, 0);
  shape_->calculateLocalInertia(parameter.mass, inertia_);

  //���̃I�u�W�F�N�g����
  rigid_body_ = std::make_shared<btRigidBody>(
      parameter.mass, motion_state_.get(), shape_.get(), inertia_);
  rigid_body_->setUserPointer(this);

  //���C�W����ݒ�
  rigid_body_->setFriction(parameter.friction);
}
BoundingSphere::~BoundingSphere() {
  if (rigid_body_) {
    rigid_body_->setUserPointer(nullptr);
    rigid_body_->setMotionState(nullptr);
    rigid_body_->setCollisionShape(nullptr);
  }
}

bool BoundingSphere::Update() {
  Collider::Update();
  return true;
}

void BoundingSphere::SetScale(float radius, float mass) {
  //���̂ɐݒ�
  shape_ = std::make_shared<btSphereShape>(radius);

  // rigidbody�֌W�ŃG���[��f���ꂽ�̂ŏ���ɒǉ����܂���
  //���̃I�u�W�F�N�g����
  rigid_body_ = std::make_shared<btRigidBody>(mass, motion_state_.get(),
                                              shape_.get(), inertia_);
  rigid_body_->setUserPointer(this);
}

}  // namespace bullet
}  // namespace legend
