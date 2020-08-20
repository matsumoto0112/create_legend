#include "src/bullet/bounding_box.h"

#include "src/bullet/bullet_helper.h"
#include "src/bullet/physics_field.h"

namespace legend {

namespace bullet {

BoundingBox::BoundingBox(util::Transform* owner_transform,
                         const InitializeParameter& parameter)
    : Collider(owner_transform) {
  //立方体に設定
  shape_ = std::make_shared<btBoxShape>(parameter.scale);

  motion_state_ = std::make_shared<btDefaultMotionState>(
      btTransform(parameter.rotation, parameter.position));

  //慣性モーメントの計算
  inertia_ = btVector3(0, 0, 0);
  shape_->calculateLocalInertia(parameter.mass, inertia_);

  //剛体オブジェクト生成
  rigid_body_ = std::make_shared<btRigidBody>(
      parameter.mass, motion_state_.get(), shape_.get(), inertia_);

  rigid_body_->setFriction(parameter.friction);
}

BoundingBox::~BoundingBox() {}

bool BoundingBox::Update() { return true; }
}  // namespace bullet
}  // namespace legend