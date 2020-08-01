#include "src/physics/capsule.h"

namespace legend {
namespace physics {
//�R���X�g���N�^
Capsule::Capsule()
    : start_position_(math::Vector3::kZeroVector),
      end_position_(math::Vector3::kForwardVector),
      rotation_(math::Quaternion::kIdentity),
      radius_(2),
      scale_(0.1f) {}

//�R���X�g���N�^
Capsule::Capsule(math::Vector3 end_position, float radius, float scale)
    : start_position_(math::Vector3::kZeroVector),
      end_position_(end_position),
      rotation_(math::Quaternion::kIdentity),
      radius_(radius),
      scale_(scale) {}

//�R���X�g���N�^
Capsule::Capsule(math::Vector3 start_position, math::Vector3 end_position,
                 float radius, float scale)
    : start_position_(start_position),
      end_position_(end_position),
      rotation_(math::Quaternion::kIdentity),
      radius_(radius),
      scale_(scale) {}

//�f�X�g���N�^
Capsule::~Capsule() {}

bool Capsule::Initialize() { return true; }

void Capsule::Update() {}

void Capsule::Draw() {}

//�n�_�̐ݒ�
void Capsule::SetStartPosition(math::Vector3 position) {
  start_position_ = position;
}

//�I�_�̐ݒ�
void Capsule::SetEndPosition(math::Vector3 position) {
  end_position_ = position;
}

//��]�ʂ̐ݒ�
void Capsule::SetRotation(math::Quaternion rotate) { rotation_ = rotate; }

//�X�P�[���̐ݒ�
void Capsule::SetScale(float scale) { scale_ = scale; }

//���a�̐ݒ�
void Capsule::SetRadius(float radius) { radius_ = radius; }

//�n�_�̎擾
math::Vector3 Capsule::GetStartPosition() { return start_position_; }

//�I�_�̎擾
math::Vector3 Capsule::GetEndPosition() { return end_position_; }

//��]�ʂ̎擾
math::Quaternion Capsule::GetRotation() { return rotation_; }

//�X�P�[���̎擾
float Capsule::GetScale() { return scale_; }

//���a�̎擾
float Capsule::GetRadius() { return radius_ * scale_; }

//�����̎擾
float Capsule::GetLength() {
  return math::Vector3(GetEndPosition() - GetStartPosition()).Magnitude();
}
}  // namespace physics
}  // namespace legend