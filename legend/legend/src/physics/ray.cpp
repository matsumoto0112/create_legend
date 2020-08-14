#include "src/physics/ray.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/directx/vertex.h"
#include "src/game/game_device.h"

namespace legend {
namespace physics {

//コンストラクタ
Ray::Ray()
    : start_position_(math::Vector3::kZeroVector),
      direction_(math::Vector3::kRightVector),
      max_distance_(1),
      rotation_(math::Quaternion::kIdentity) {}

//コンストラクタ
Ray::Ray(math::Vector3 direction, float max_distance)
    : start_position_(math::Vector3::kZeroVector),
      direction_(direction),
      max_distance_(max_distance),
      rotation_(math::Quaternion::kIdentity) {}

//コンストラクタ
Ray::Ray(math::Vector3 start_position, math::Vector3 direction,
         float max_distance, math::Quaternion rotation)
    : start_position_(start_position),
      direction_(direction),
      max_distance_(max_distance),
      rotation_(rotation) {}

//デストラクタ
Ray::~Ray() {}

bool Ray::Initialize() {
  if (!draw_line_.Init()) {
    return false;
  }

  return true;
}

void Ray::Update() {
  draw_line_.SetTransform(util::Transform(GetStartPosition(), GetRotation(),
                                          math::Vector3::kUnitVector));
}

void Ray::Draw(directx::device::CommandList& command_list) {
  draw_line_.SetTransform(util::Transform(GetStartPosition(), GetRotation(),
                                          math::Vector3::kUnitVector));
  draw_line_.Render(command_list);
}

//始点の取得
math::Vector3 Ray::GetStartPosition() const { return start_position_; }

//方向ベクトルの取得
math::Vector3 Ray::GetDirection() const { return direction_; }

//最大範囲の取得
float Ray::GetDistance() const { return max_distance_; }

math::Quaternion Ray::GetRotation() const { return rotation_; }

void Ray::SetStartPosition(const math::Vector3& start_position) {
  start_position_ = start_position;
}

void Ray::SetRotation(const math::Quaternion& rotation) {
  rotation_ = rotation;
}
}  // namespace physics
}  // namespace legend