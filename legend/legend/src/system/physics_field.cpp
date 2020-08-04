#include "src/system/physics_field.h"

namespace legend {
namespace system {

//�d��
const float gravity = -9.8f;

//�R���X�g���N�^
PhysicsField::PhysicsField() { obbs_.clear(); }

//�f�X�g���N�^
PhysicsField::~PhysicsField() {}

//�X�V
bool PhysicsField::Update() { return true; }

//�v�f�̒ǉ�
void PhysicsField::AddCollision(id::Object object_id,
                             physics::BoundingBox add_obb) {
  obbs_.emplace(object_id, add_obb);
}
}  // namespace system
}  // namespace legend