#include "src/physics/capsule.h"

namespace legend {
namespace physics {
//コンストラクタ
Capsule::Capsule()
    : start_position_(math::Vector3::kZeroVector),
      end_position_(math::Vector3::kForwardVector),
      rotation_(math::Quaternion::kIdentity),
      radius_(2),
      scale_(0.1f) {}

//コンストラクタ
Capsule::Capsule(math::Vector3 end_position, float radius, float scale)
    : start_position_(math::Vector3::kZeroVector),
      end_position_(end_position),
      rotation_(math::Quaternion::kIdentity),
      radius_(radius),
      scale_(scale) {}

//コンストラクタ
Capsule::Capsule(math::Vector3 start_position, math::Vector3 end_position,
                 float radius, float scale)
    : start_position_(start_position),
      end_position_(end_position),
      rotation_(math::Quaternion::kIdentity),
      radius_(radius),
      scale_(scale) {}

//デストラクタ
Capsule::~Capsule() {}

bool Capsule::Initialize() { return true; }

void Capsule::Update() {}

void Capsule::Draw() {}

//始点の設定
void Capsule::SetStartPosition(math::Vector3 position) {
  start_position_ = position;
}

//終点の設定
void Capsule::SetEndPosition(math::Vector3 position) {
  end_position_ = position;
}

//回転量の設定
void Capsule::SetRotation(math::Quaternion rotate) { rotation_ = rotate; }

//スケールの設定
void Capsule::SetScale(float scale) { scale_ = scale; }

//半径の設定
void Capsule::SetRadius(float radius) { radius_ = radius; }

//始点の取得
math::Vector3 Capsule::GetStartPosition() { return start_position_; }

//終点の取得
math::Vector3 Capsule::GetEndPosition() { return end_position_; }

//回転量の取得
math::Quaternion Capsule::GetRotation() { return rotation_; }

//スケールの取得
float Capsule::GetScale() { return scale_; }

//半径の取得
float Capsule::GetRadius() { return radius_ * scale_; }

//長さの取得
float Capsule::GetLength() {
  return math::Vector3(GetEndPosition() - GetStartPosition()).Magnitude();
}
}  // namespace physics
}  // namespace legend