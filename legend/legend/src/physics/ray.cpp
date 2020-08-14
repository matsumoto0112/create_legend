#include "src/physics/ray.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/directx/vertex.h"
#include "src/game/game_device.h"

namespace legend {
namespace physics {

//コンストラクタ
Ray::Ray()
    : start_position_(math::Vector3::kZeroVector),
      direction_(math::Vector3::kRightVector) {}

//コンストラクタ
Ray::Ray(math::Vector3 start_position, math::Vector3 direction)
    : start_position_(start_position), direction_(direction) {}

//デストラクタ
Ray::~Ray() {}

bool Ray::Initialize() {
  if (!draw_line_.Init()) {
    return false;
  }

  return true;
}

//更新
void Ray::Update() {
  draw_line_.SetTransform(util::Transform(GetStartPosition(),
                                          math::Quaternion::kIdentity,
                                          math::Vector3::kUnitVector));
}

//描画
void Ray::Draw(directx::device::CommandList& command_list) {
  draw_line_.SetTransform(util::Transform(GetStartPosition(),
                                          math::Quaternion::kIdentity,
                                          math::Vector3::kUnitVector));
  draw_line_.Render(command_list);
}

//始点の取得
math::Vector3 Ray::GetStartPosition() const { return start_position_; }

//方向ベクトルの取得
math::Vector3 Ray::GetDirection() const { return direction_; }

//始点の設定
void Ray::SetStartPosition(const math::Vector3& start_position) {
  start_position_ = start_position;
}

//方向ベクトルの設定
void Ray::SetDirection(const math::Vector3& direction) {
  direction_ = direction;
}
}  // namespace physics
}  // namespace legend