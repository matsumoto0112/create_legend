#include "src/system/physics_field.h"

namespace legend {
namespace system {

//重力
const float gravity = -9.8f;

//コンストラクタ
PhysicsField::PhysicsField() { obbs_.clear(); }

//デストラクタ
PhysicsField::~PhysicsField() {}

//更新
bool PhysicsField::Update() { return true; }

//要素の追加
void PhysicsField::AddCollision(id::Object object_id,
                             physics::BoundingBox add_obb) {
  obbs_.emplace(object_id, add_obb);
}
}  // namespace system
}  // namespace legend