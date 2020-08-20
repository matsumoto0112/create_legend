#include "src/bullet/bounding_box.h"

#include "src/bullet/bullet_helper.h"
#include "src/bullet/physics_field.h"

namespace legend {

namespace bullet {

BoundingBox::BoundingBox(util::Transform* owner_transform,
                         const InitializeParameter& parameter)
    : Collider(owner_transform) {
  //�����̂ɐݒ�
  shape_ = std::make_shared<btBoxShape>(helper::TobtVector3(parameter.scale));

  motion_state_ = std::make_shared<btDefaultMotionState>(
      btTransform(helper::TobtQuaternion(parameter.rotation),
                  helper::TobtVector3(parameter.position)));

  //�������[�����g�̌v�Z
  inertia_ = btVector3(0, 0, 0);
  shape_->calculateLocalInertia(parameter.mass, inertia_);

  //���̃I�u�W�F�N�g����
  rigid_body_ = std::make_shared<btRigidBody>(
      btScalar(parameter.mass), motion_state_.get(), shape_.get(), inertia_);

  rigid_body_->setFriction(btScalar(parameter.friction));
}

BoundingBox::~BoundingBox() {}

bool BoundingBox::Update() { return true; }
}  // namespace bullet
}  // namespace legend