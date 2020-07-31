#include "src/player/player.h"

namespace legend {
namespace player {
//コンストラクタ
Player::Player()
    : transform_(),
      velocity_(math::Vector3::kZeroVector),
      min_power_(0),
      max_power_(1) {
  transform_.SetScale(math::Vector3(0.1f, 0.1f, 0.1f));
  obb_ = physics::BoundingBox();
  obb_.SetLength(2, 1, 2);
  is_move_ = false;
  impulse_ = min_power_;
  deceleration_x_ = deceleration_z_ = 0;
  input_velocity_ = math::Vector3::kZeroVector;
  is_set_power_ = false;
  up_power_ = true;
  is_input_ = false;
  change_time_ = 0;
}

Player::Player(math::Vector3 position, math::Quaternion rotation,
               math::Vector3 scale, float min_power, float max_power)
    : transform_(),
      velocity_(math::Vector3::kZeroVector),
      min_power_(min_power),
      max_power_(max_power) {
  transform_.SetPosition(position);
  transform_.SetRotation(rotation);
  transform_.SetScale(scale);
  obb_ = physics::BoundingBox(transform_.GetPosition(),
                              transform_.GetRotation().ToEular(),
                              transform_.GetScale());
  obb_.SetLength(2, 1, 2);
  is_move_ = false;
  impulse_ = min_power_;
  deceleration_x_ = deceleration_z_ = 0;
  input_velocity_ = math::Vector3::kZeroVector;
  is_set_power_ = false;
  up_power_ = true;
  is_input_ = false;
  change_time_ = 0;
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

  update_time_ =
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();

  if (is_input_ && input_velocity_.Magnitude() <= 0.1f) {
    is_move_ = true;
  }

  return true;
}

//描画
void Player::Draw(directx::DirectX12Device& device) { obb_.Draw(device); }

//移動
void Player::Move() {
  if (!is_move_) return;

  math::Vector3 velocity = math::Vector3::kZeroVector;

  if (velocity.Magnitude() < stick_velocities_.front().Magnitude())
    velocity = stick_velocities_.front();
  stick_velocities_.erase(stick_velocities_.begin());

  //移動速度がゼロだったらreturn
  if (velocity == math::Vector3::kZeroVector) {
    ResetParameter();
    return;
  }

  //移動距離を求める
  float length =
      math::util::Sqrt(velocity.x * velocity.x + velocity.z * velocity.z);

  //実際に動く距離
  float x = velocity.x / length;
  float z = velocity.z / length;

  //減速計算
  deceleration_x_ = x / (length * length) * update_time_;
  deceleration_z_ = z / (length * length) * update_time_;

  //移動処理
  math::Vector3 v = math::Vector3(x, 0, z);
  math::Vector3 position =
      transform_.GetPosition() + v * impulse_ * power_ * update_time_;
  SetPosition(position);

  Deceleration(10);
}

void Player::SetPosition(math::Vector3 position) {
  transform_.SetPosition(position);
  obb_.SetPosition(position);
}

//速度の設定
void Player::SetVelocity(math::Vector3 velocity) { velocity_ = velocity; }

void Player::SetVelocity() {
  input::InputManager& input = game::GameDevice::GetInstance()->GetInput();
  //ゲームパッドが一つだけ接続されている間

  if (input.GetGamepad()->GetCount() != 1) return;
  input_velocity_.x = -input.GetGamepad()->GetStickLeft().x;
  input_velocity_.z = -input.GetGamepad()->GetStickLeft().y;

  change_time_ += update_time_;
  if (change_time_ < time || is_move_) return;

  //左スティックの傾きに合わせて値を入れる
  velocity_.x = input_velocity_.x;
  velocity_.z = input_velocity_.z;
  if (velocity_.Magnitude() >= 0.1f) is_input_ = true;

  before_velocity_ = velocity_;
  stick_velocities_.push_back(velocity_);
  while (stick_velocities_.size() > max_stick_velocity_num_) {
    stick_velocities_.erase(stick_velocities_.begin());
  }
  change_time_ = 0;
}

//パワーの設定
void Player::SetImpulse() {
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
  before_velocity_ = math::Vector3::kZeroVector;
  deceleration_x_ = deceleration_z_ = 0;
  input_velocity_ = math::Vector3::kZeroVector;
  stick_velocities_.clear();
  is_move_ = false;
  change_time_ = 0;
}

//減速
void Player::Deceleration(float deceleration_rate) {
  float x = deceleration_x_ * deceleration_rate * update_time_;
  float z = deceleration_z_ * deceleration_rate * update_time_;

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

  before_velocity_ = velocity_;
  stick_velocities_.push_back(velocity_);
  while (stick_velocities_.size() > max_stick_velocity_num_) {
    stick_velocities_.erase(stick_velocities_.begin());
  }
}

//座標の取得
math::Vector3 Player::GetPosition() const { return transform_.GetPosition(); }

//移動量の取得
math::Vector3 Player::GetVelocity() const { return velocity_; }

math::Quaternion Player::GetRotation() const {
  return transform_.GetRotation();
}

float Player::GetImpulse() const { return impulse_; }

physics::BoundingBox& Player::GetOBB() {
  physics::BoundingBox& obb = obb_;
  return obb;
}
}  // namespace player
}  // namespace legend