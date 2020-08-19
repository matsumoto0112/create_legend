#include "src/bullet/bounding_box.h"

namespace legend {

namespace bullet {

BoundingBox::BoundingBox(const InitializeParameter& parameter) {
  //�����̂ɐݒ�
  shape_ = std::make_shared<btBoxShape>(parameter.scale);

  std::shared_ptr<btDefaultMotionState> motion_state =
      std::make_shared<btDefaultMotionState>(
          btTransform(parameter.rotation, parameter.position));

  //�������[�����g�̌v�Z
  btVector3 inertia(0, 0, 0);
  shape_->calculateLocalInertia(parameter.mass, inertia);

  //���̃I�u�W�F�N�g����
  rigid_body_ = std::make_shared<btRigidBody>(
      parameter.mass, motion_state.get(), shape_.get(), inertia);

  //���[���h�ɒǉ�
  parameter.world->addRigidBody(rigid_body_.get());
}

bool BoundingBox::Update() { return true; }
}  // namespace bullet
}  // namespace legend