#include "src/physics/bounding_box.h"

namespace legend {
namespace physics {

//�R���X�g���N�^
BoundingBox::BoundingBox()
    : position_(math::Vector3::kZeroVector), directions_(3), lengthes_(3) {
  directions_[0] = math::Vector3::kRightVector;
  directions_[1] = math::Vector3::kUpVector;
  directions_[2] = math::Vector3::kForwardVector;

  lengthes_[0] = 1.0f;
  lengthes_[1] = 1.0f;
  lengthes_[2] = 1.0f;
}

//�����x�N�g�����擾
math::Vector3 BoundingBox::GetDirection(i32 direction_num) {
  return directions_[direction_num];
}

//�������擾
float BoundingBox::GetLength(i32 length_num) { return lengthes_[length_num]; }

//���݂̈ʒu���擾
math::Vector3 BoundingBox::GetPosition() { return position_; }

//�e�����x�N�g���̐ݒ�
void BoundingBox::SetDirection(math::Vector3 direction_x,
                               math::Vector3 direction_y,
                               math::Vector3 direction_z) {
  directions_[0] = direction_x;
  directions_[1] = direction_y;
  directions_[2] = direction_z;
}

//�e�����̒����̐ݒ�
void BoundingBox::SetLength(float length_x, float length_y, float length_z) {
  lengthes_[0] = length_x;
  lengthes_[1] = length_y;
  lengthes_[2] = length_z;
}

}  // namespace physics
}  // namespace legend