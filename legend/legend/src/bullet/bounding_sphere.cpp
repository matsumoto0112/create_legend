#include "bounding_sphere.h"

#include "src\\stdafx.h"

namespace legend {
namespace bullet {
BoundingSphere::BoundingSphere(const InitializeParameter& parameter) {
    //立方体に設定
    shape_ = std::make_shared<btSphereShape>(parameter.radius);

    motion_state_ = std::make_shared<btDefaultMotionState>(
        btTransform(parameter.rotation, parameter.position));

    //慣性モーメントの計算
    inertia_ = btVector3(0, 0, 0);
    shape_->calculateLocalInertia(parameter.mass, inertia_);

    //剛体オブジェクト生成
    rigid_body_ = std::make_shared<btRigidBody>(
        parameter.mass, motion_state_.get(), shape_.get(), inertia_);
}
BoundingSphere::~BoundingSphere() {}
bool BoundingSphere::Update() { return false; }
}  // namespace bullet
}  // namespace legend
