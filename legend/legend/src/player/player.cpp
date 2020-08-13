#include "src/player/player.h"

#include "src/directx/shader/shader_register_id.h"

namespace legend {
namespace player {

//コンストラク
Player::Player() : Parent(L"Player") {}

//デストラクタ
Player::~Player() {}

//初期化
bool Player::Init(const InitializeParameter& parameter) {
  if (!Parent::InitBuffer()) {
    return false;
  }

  this->transform_ = parameter.transform;
  this->collision_.SetPosition(transform_.GetPosition());
  this->collision_.SetRotation(transform_.GetRotation());
  this->collision_.SetScale(transform_.GetScale());
  //= physics::BoundingBox(transform_.GetPosition(), transform_.GetRotation(),
  //                       transform_.GetScale() * 0.1f);
  this->collision_.SetLength(parameter.bouding_box_length);
  min_power_ = parameter.min_power;
  max_power_ = parameter.max_power;

  up_power_ = true;
  is_set_power_ = false;
  move_end_ = false;

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  model_ = resource.GetModel().Get(util::resource::id::Model::ERASER);

  return true;
}

//更新
bool Player::Update() {
  update_time_ =
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();

  if (is_input_ &&
      change_amount_velocity_.Magnitude() - input_velocity_.Magnitude() >=
          0.5f) {
    is_move_ = true;
  }

  // transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  // transform_cb_.UpdateStaging();

  if (is_move_ && velocity_ == math::Vector3::kZeroVector) {
    ResetParameter();
    move_end_ = true;
  }

  SetVelocity();
  SetImpulse();

  return true;
}

//座標の設定
void Player::SetPosition(math::Vector3 position) {
  transform_.SetPosition(position);
  collision_.SetPosition(position);
}

//回転の設定
void Player::SetRotation(math::Quaternion rotation) {
  transform_.SetRotation(rotation);
  collision_.SetRotation(rotation);
}

//速度の設定
void Player::SetVelocity() {
  if (is_move_) return;

  input::InputManager& input = game::GameDevice::GetInstance()->GetInput();
  //ゲームパッドが一つだけ接続されている間

  if (input.GetGamepad()->GetCount() != 1) return;
  input_velocity_.x = -input.GetGamepad()->GetStickLeft().x;
  input_velocity_.z = -input.GetGamepad()->GetStickLeft().y;

  velocity_update_time_ += update_time_;
  if (velocity_update_time_ < change_time_) return;

  velocity_ = input_velocity_;
  if (velocity_.Magnitude() >= 0.1f) is_input_ = true;

  change_amount_velocity_ = velocity_;
  velocity_update_time_ = 0;
}

//速度の設定
void Player::SetVelocity(math::Vector3 velocity) {
  if (!is_move_) return;

  velocity_ = velocity;
}

//パワーの設定
void Player::SetImpulse() {
  input::InputManager& input = game::GameDevice::GetInstance()->GetInput();

  //パワー調整を終えたらreturn
  if (is_set_power_ || !is_input_) return;

  //左スティックの傾きが0.1以下かつ、方向入力していたらパワー調整を完了にする
  if (input.GetGamepad()->GetStickLeft().Magnitude() <= 0.1f && is_input_) {
    is_set_power_ = true;
    return;
  }

  //パワー調整処理
  if (up_power_) {
    impulse_ += 0.1f * update_time_ * 2;
    if (impulse_ >= max_power_) {
      impulse_ = max_power_;
      up_power_ = false;
    }
  } else {
    impulse_ -= 0.1f * update_time_ * 2;
    if (impulse_ <= min_power_) {
      impulse_ = min_power_;
      up_power_ = true;
    }
  }
}

//移動に必要なパラメータを初期化
void Player::ResetParameter() {
  if (velocity_.Magnitude() != 0.0f) return;

  is_input_ = false;
  is_set_power_ = false;
  impulse_ = 0;
  up_power_ = true;
  change_amount_velocity_ = math::Vector3::kZeroVector;
  deceleration_x_ = deceleration_z_ = 0;
  input_velocity_ = math::Vector3::kZeroVector;
  is_move_ = false;
  velocity_update_time_ = 0;
}

//移動終了判定のリセット
void Player::ResetMoveEnd() { move_end_ = false; }

//座標の取得
math::Vector3 Player::GetPosition() const { return transform_.GetPosition(); }

//移動量の取得
math::Vector3 Player::GetVelocity() const { return velocity_; }

//回転の取得
math::Quaternion Player::GetRotation() const {
  return transform_.GetRotation();
}

//加える力の割合の取得
float Player::GetImpulse() const { return impulse_; }

//加える力の取得
float Player::GetPower() const { return power_; }

//移動終了判定の取得
bool Player::GetMoveEnd() const { return move_end_; }

//移動判定の取得
bool Player::GetIsMove() const { return is_move_; }
}  // namespace player
}  // namespace legend