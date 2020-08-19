#include "src/bullet/bounding_box.h"

namespace legend {

namespace bullet {

BoundingBox::BoundingBox(const InitializeParameter& parameter) {
  //立方体に設定
  shape_ = std::make_shared<btBoxShape>(parameter.scale);

  std::shared_ptr<btDefaultMotionState> motion_state =
      std::make_shared<btDefaultMotionState>(
          btTransform(parameter.rotation, parameter.position));

  //慣性モーメントの計算
  btVector3 inertia(0, 0, 0);
  shape_->calculateLocalInertia(parameter.mass, inertia);

  //剛体オブジェクト生成
  rigid_body_ = std::make_shared<btRigidBody>(
      parameter.mass, motion_state.get(), shape_.get(), inertia);

  //ワールドに追加
  parameter.world->addRigidBody(rigid_body_.get());
}

bool BoundingBox::Update() { return true; }
}  // namespace bullet
}  // namespace legend