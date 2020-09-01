#include "src/camera/lookat_follow_target_camera.h"

namespace {
using legend::math::Quaternion;
using legend::math::Vector3;
Quaternion CreateLookAt(const Vector3& target) {
  const float dot = Vector3::Dot(Vector3::kForwardVector, target.Normalized());
  const float theta = legend::math::util::Acos(dot);
  const Vector3 cross =
      Vector3::Cross(Vector3::kForwardVector, target).Normalized();

  return Quaternion(cross, theta);
}
}  // namespace

namespace legend {
namespace camera {

LookatFollowTargetCamera::LookatFollowTargetCamera() {}

LookatFollowTargetCamera::~LookatFollowTargetCamera() {}

bool LookatFollowTargetCamera::Init(const std::wstring& name,
                                    actor::Actor* target, float height,
                                    float distance, float fov,
                                    float aspect_ratio,
                                    const math::Vector3& up_vector,
                                    float near_z, float far_z) {
  this->target_ = target;
  const math::Vector3 position =
      target->GetTransform().GetPosition() + math::Vector3(0, 10, -10);
  const math::Quaternion rotation =
      math::Quaternion::FromEular(math::util::DEG_2_RAD * 45.0f, 0.0f, 0.0f);
  this->height_ = height;
  this->distance_ = distance;
  this->theta_ = math::util::DEG_2_RAD * -90.0f;

  return PerspectiveCamera::Init(name, position, rotation, fov, aspect_ratio,
                                 up_vector, near_z, far_z);
}

void LookatFollowTargetCamera::UpdateConstantBufferStaging() {
  const math::Vector3 target_position = target_->GetTransform().GetPosition();

  SetPosition(target_position +
              math::Vector3(math::util::Cos(theta_) * distance_, height_,
                            math::util::Sin(theta_) * distance_));

  math::Vector3 target = target_position - GetPosition();
  SetRotation(CreateLookAt(target));

  PerspectiveCamera::UpdateConstantBufferStaging();
}

}  // namespace camera
}  // namespace legend