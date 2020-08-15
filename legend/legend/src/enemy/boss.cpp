#include "src/enemy/boss.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/util/path.h"
#include "src/util/resource/pixel_shader.h"
#include "src/util/resource/vertex_shader.h"

namespace {
constexpr const wchar_t* MODEL_NAME = L"enemy_eraser_01.glb";
}  // namespace

namespace legend {
namespace enemy {
//コンストラクタ
Boss::Boss() : Parent(L"Boss"), velocity_(math::Vector3::kZeroVector) {
  is_move_ = false;
  // deceleration_x_ = deceleration_z_ = 0;
}

//デストラクタ
Boss::~Boss() {}

//初期化
bool Boss::Init(const InitializeParameter& parameter) {
  if (!Parent::InitBuffer()) return false;
  // if (!obb_.Initialize(device)) {
  //  return false;
  //}
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  this->transform_ = parameter.transform;
  this->collision_.SetPosition(transform_.GetPosition());
  this->collision_.SetRotation(transform_.GetRotation());
  this->collision_.SetScale(transform_.GetScale());
  this->collision_.SetLength(parameter.bouding_box_length);

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  model_ = resource.GetModel().Get(MODEL_NAME);

  move_end_ = false;

  return true;
}

//更新
bool Boss::Update() {
  // obb_.Update();

  update_time_ =
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();

  const bool is_nearly_zero_vector = velocity_.MagnitudeSquared() < 0.01f;
  if (is_move_ && is_nearly_zero_vector) move_end_ = true;
  is_move_ = (0.01f < velocity_.Magnitude());
  // Move();

  // transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  // transform_cb_.UpdateStaging();

  return true;
}

//移動
void Boss::Move() {
  if (!is_move_) return;

  //移動距離を求める
  float length =
      math::util::Sqrt(velocity_.x * velocity_.x + velocity_.z * velocity_.z);

  //実際に動く距離
  float x = -velocity_.x / length;
  float z = -velocity_.z / length;

  ////減速計算
  // deceleration_x_ = x / (length * length);
  // deceleration_z_ = z / (length * length);

  //移動処理
  math::Vector3 v = math::Vector3(x, 0, z);
  math::Vector3 position = GetPosition() + v * power_ * update_time_;
  SetPosition(position);

  // Deceleration(2);
}

void Boss::SetPosition(math::Vector3 position) {
  transform_.SetPosition(position);
  collision_.SetPosition(position);
}

//速度の設定
void Boss::SetVelocity(math::Vector3 velocity) { velocity_ = velocity; }

void Boss::SetRotation() {
  math::Quaternion rotation = transform_.GetRotation();
  rotation.y += velocity_.x;
  transform_.SetRotation(rotation);
}

//移動に必要なパラメータを初期化
void Boss::ResetParameter() {
  if (velocity_.Magnitude() != 0.0f) return;

  // deceleration_x_ = deceleration_z_ = 0;
  is_move_ = false;
}

////減速
// void Enemy::Deceleration(float deceleration_rate) {
//  float x = deceleration_x_ * deceleration_rate * update_time_;
//  float z = deceleration_z_ * deceleration_rate * update_time_;
//
//  if ((x <= velocity_.x && velocity_.x <= 0) ||
//      (0 <= velocity_.x && velocity_.x <= x)) {
//    velocity_.x = 0;
//  } else {
//    velocity_.x -= x;
//  }
//  if ((z <= velocity_.z && velocity_.z <= 0) ||
//      (0 <= velocity_.z && velocity_.z <= z)) {
//    velocity_.z = 0;
//  } else {
//    velocity_.z -= z;
//  }
//}

//座標の取得
math::Vector3 Boss::GetPosition() const { return transform_.GetPosition(); }

//移動量の取得
math::Vector3 Boss::GetVelocity() const { return velocity_; }

math::Quaternion Boss::GetRotation() const { return transform_.GetRotation(); }

float Boss::GetPower() const { return power_; }

bool Boss::GetMoveEnd() const { return move_end_; }

void Boss::ResetMoveEnd() { move_end_ = false; }

}  // namespace enemy
}  // namespace legend