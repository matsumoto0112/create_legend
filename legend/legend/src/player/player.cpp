#include "src/player/player.h"

namespace legend {
namespace player {
//コンストラクタ
Player::Player()
    : position_(math::Vector3(math::Vector3::kZeroVector)),
      rotation_(math::Vector3::kZeroVector),
      scale_(math::Vector3(0.1f, 0.1f, 0.1f)),
      velocity_(math::Vector3::kZeroVector),
      min_power_(0),
      max_power_(1) {
  obb_ = physics::BoundingBox();
  obb_.SetLength(2, 1, 2);
  is_move_ = false;
}

Player::Player(math::Vector3 position, math::Vector3 rotation,
               math::Vector3 scale, float min_power, float max_power)
    : position_(position),
      rotation_(rotation),
      scale_(scale),
      velocity_(math::Vector3::kZeroVector),
      min_power_(min_power),
      max_power_(max_power) {
  obb_ = physics::BoundingBox(position_, rotation_, scale_);
}

//デストラクタ
Player::~Player() {}

//初期化
bool Player::Initilaize(directx::DirectX12Device& device) {
  if (!obb_.Initialize(device)) {
    return false;
  }

  return true;
}

//更新
bool Player::Update() {
  obb_.Update();

  input::InputManager& input = game::GameDevice::GetInstance()->GetInput();
  if (is_input_ && velocity_.Magnitude() <= 0.1f) {
    is_move_ = true;
  }

  return true;
}

//描画
void Player::Draw(directx::DirectX12Device& device) { obb_.Draw(device); }

//移動
void Player::Move() {
  if (!is_move_ && before_velocity_ != math::Vector3::kZeroVector) return;

  math::Vector3 velocity = math::Vector3::kZeroVector;
  if (velocity.Magnitude() < stick_velocities_.front().Magnitude())
    velocity = stick_velocities_.front();
  stick_velocities_.clear();

  if (velocity == math::Vector3::kZeroVector) {
    ResetParameter();
    return;
  }

  float length =
      math::util::Sqrt(velocity.x * velocity.x + velocity.z * velocity.z);

  float x = velocity.x / length;
  float z = velocity.z / length;

  deceleration_x_ = x / (length * length);
  deceleration_z_ = z / (length * length);

  math::Vector3 position = GetPosition() + math::Vector3(x, 0, z) *
                                               GetImpulse() * power_ *
                                               update_time_;
  SetPosition(position);

  Deceleration();
}

void Player::SetPosition(math::Vector3 position) {
  position_ = position;
  obb_.SetPosition(position);
}

//速度の設定
void Player::SetVelocity(math::Vector3 velocity) { velocity_ = velocity; }

void Player::SetVelocity() {
  input::InputManager& input = game::GameDevice::GetInstance()->GetInput();
  //ゲームパッドが一つだけ接続されている間
  if (input.GetGamepad()->GetCount() != 1) return;

  if (!is_move_) {
    velocity_.x = -input.GetGamepad()->GetStickLeft().x;
    velocity_.z = -input.GetGamepad()->GetStickLeft().y;
    if (math::util::Abs(velocity_.x) >= 0.1f ||
        math::util::Abs(velocity_.z) >= 0.1f)
      is_input_ = true;
  }
  before_velocity_ = velocity_;
  stick_velocities_.push_back(velocity_);
  if (stick_velocities_.size() > max_stick_velocity_num_) {
    stick_velocities_.erase(stick_velocities_.begin());
  }
}

//パワーの設定
void Player::SetPower() {
  input::InputManager& input = game::GameDevice::GetInstance()->GetInput();
  //ゲームパッドが一つだけ接続されている間
  if (input.GetGamepad()->GetCount() != 1) return;

  //パワー調整を終えたらreturn
  if (is_set_power_ || !is_input_) return;

  //左スティックの傾きが0.1以下かつ、方向入力していたらパワー調整を完了にする
  if (math::util::Abs(input.GetGamepad()->GetStickLeft().Magnitude()) <= 0.1f &&
      is_input_) {
    is_set_power_ = true;
    return;
  }

  //パワー調整処理
  if (up_power_) {
    impulse_ += 0.1f * update_time_;
    if (impulse_ >= max_power_) {
      impulse_ = max_power_;
      up_power_ = false;
    }
  } else {
    impulse_ -= 0.1f * update_time_;
    if (impulse_ <= min_power_) {
      impulse_ = min_power_;
      up_power_ = true;
    }
  }
}

//重力
void Player::UseGravity() { velocity_.y = gravity_; }

//移動に必要なパラメータを初期化
void Player::ResetParameter() {
  if (velocity_.Magnitude() != 0.0f) return;

  is_input_ = false;
  is_set_power_ = false;
  impulse_ = 0;
  up_power_ = true;
  before_velocity_ = math::Vector3::kZeroVector;
  stick_velocities_.clear();
  is_move_ = false;
}

void Player::Deceleration() {
  float x = deceleration_x_ * 2;
  float z = deceleration_z_ * 2;

  if ((x <= velocity_.x && velocity_.x <= 0) ||
      (0 <= velocity_.x && velocity_.x <= x)) {
    velocity_.x = 0;
  } else {
    velocity_.x -= x;
  }
  if ((z <= velocity_.z && velocity_.z <= 0) ||
      (0 <= velocity_.z && velocity_.z <= z)) {
    velocity_.z = 0;
  } else {
    velocity_.z -= z;
  }

  // if (math::util::Abs(velocity_.x) <= 0.1f) velocity_.x = 0;
  // if (math::util::Abs(velocity_.z) <= 0.1f) velocity_.z = 0;

  // if (is_set_power_) {
  //  if (velocity_.x < -0.1f) {
  //    velocity_.x += 0.1f * update_time_;
  //    if (velocity_.x >= -0.1f && velocity_.x < 0) {
  //      velocity_.x = 0;
  //    }
  //  } else if (velocity_.x > 0.1f) {
  //    velocity_.x -= 0.1f * update_time_;
  //    if (velocity_.x <= 0.1f && velocity_.x > 0) {
  //      velocity_.x = 0;
  //    }
  //  }
  //  if (velocity_.z < -0.1f) {
  //    velocity_.z += 0.1f * update_time_;
  //    if (velocity_.z >= -0.1f && velocity_.z < 0) {
  //      velocity_.z = 0;
  //    }
  //  } else if (velocity_.z > 0.1f) {
  //    velocity_.z -= 0.1f * update_time_;
  //    if (velocity_.z <= 0.1f && velocity_.z > 0) {
  //      velocity_.z = 0;
  //    }
  //  }
  //}
  before_velocity_ = velocity_;
  stick_velocities_.push_back(velocity_);
  if (stick_velocities_.size() > max_stick_velocity_num_) {
    stick_velocities_.erase(stick_velocities_.begin());
  }
}

//座標の取得
math::Vector3 Player::GetPosition() const { return position_; }

//移動量の取得
math::Vector3 Player::GetVelocity() const { return velocity_; }

float Player::GetImpulse() const { return impulse_; }

physics::BoundingBox& Player::GetOBB() {
  physics::BoundingBox& obb = obb_;
  return obb;
}
}  // namespace player
}  // namespace legend