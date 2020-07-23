#include "src/util/transform.h"

namespace legend {
namespace util {
Transform::Transform() {}

Transform::Transform(const math::Vector3& position,
                     const math::Quaternion& rotation,
                     const math::Vector3& scale)
    : position_(position), rotation_(rotation), scale_(scale) {}

Transform::~Transform() {}

math::Matrix4x4 Transform::CreateWorldMatrix() const {
  return math::Matrix4x4::CreateScale(scale_) * rotation_.ToMatrix() *
         math::Matrix4x4::CreateTranslate(position_);
}
}  // namespace util
}  // namespace legend