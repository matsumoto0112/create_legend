#include "src/bullet/bounding_box.h"

#include "src/bullet/bullet_helper.h"
#include "src/bullet/physics_field.h"

namespace legend {

namespace bullet {

BoundingBox::BoundingBox(actor::Actor* owner,
                         const InitializeParameter& parameter)
    : Collider(owner) {
  //立方体に設定
  shape_ = std::make_shared<btBoxShape>(helper::TobtVector3(parameter.scale));

  motion_state_ = std::make_shared<btDefaultMotionState>(
      btTransform(helper::TobtQuaternion(parameter.rotation),
                  helper::TobtVector3(parameter.position)));

  //慣性モーメントの計算
  inertia_ = btVector3(0, 0, 0);
  shape_->calculateLocalInertia(parameter.mass, inertia_);
  //剛体オブジェクト生成
  rigid_body_ = std::make_shared<btRigidBody>(
      btScalar(parameter.mass), motion_state_.get(), shape_.get(), inertia_);
  rigid_body_->setUserPointer(this);
  rigid_body_->setFriction(btScalar(parameter.friction));
}

BoundingBox::~BoundingBox() {
  if (rigid_body_) {
    rigid_body_->setUserPointer(nullptr);
    rigid_body_->setMotionState(nullptr);
    rigid_body_->setCollisionShape(nullptr);
  }
}

bool BoundingBox::Update() { return true; }
void BoundingBox::SetScale(math::Vector3 scale) {
  //立方体に設定
  shape_ = std::make_shared<btBoxShape>(helper::TobtVector3(scale));
}
}  // namespace bullet
}  // namespace legend