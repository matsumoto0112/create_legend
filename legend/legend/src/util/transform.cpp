#include "src/util/transform.h"

namespace legend {
namespace util {

//�R���X�g���N�^
Transform::Transform(const math::Vector3& position,
                     const math::Quaternion& rotation,
                     const math::Vector3& scale)
    : position_(position), rotation_(rotation), scale_(scale) {}

//�f�X�g���N�^
Transform::~Transform() {}

//���[���h�s����쐬����
math::Matrix4x4 Transform::CreateWorldMatrix() const {
  return math::Matrix4x4::CreateScale(scale_) * rotation_.ToMatrix() *
         math::Matrix4x4::CreateTranslate(position_);
}
}  // namespace util
}  // namespace legend