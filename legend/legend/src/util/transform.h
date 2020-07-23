#ifndef LEGEND_UTIL_TRANSFORM_H_
#define LEGEND_UTIL_TRANSFORM_H_

/**
 * @file transform.h
 */

#include "src/math/matrix_4x4.h"
#include "src/math/quaternion.h"
#include "src/math/vector_3.h"

namespace legend {
namespace util {
class Transform {
 public:
  Transform();
  Transform(const math::Vector3& position = math::Vector3::kZeroVector,
            const math::Quaternion& rotation = math::Quaternion::kIdentity,
            const math::Vector3& scale = math::Vector3::kUnitVector);
  ~Transform();

  inline void SetPosition(const math::Vector3& position) {
    this->position_ = position;
  }
  inline math::Vector3 GetPosition() const { return position_; }
  inline void SetRotation(const math::Quaternion& rotation) {
    this->rotation_ = rotation;
  }
  inline math::Quaternion GetRotation() const { return rotation_; }
  inline void SetScale(const math::Vector3& scale) { this->scale_ = scale; }
  inline math::Vector3 GetScale() const { return scale_; }

  math::Matrix4x4 CreateWorldMatrix() const;

 private:
  //! 座標
  math::Vector3 position_;
  //! 回転
  math::Quaternion rotation_;
  //! スケーリング
  math::Vector3 scale_;
};

}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_TRANSFORM_H_
