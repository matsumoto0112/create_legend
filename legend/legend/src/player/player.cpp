#include "src/player/player.h"

#include "src/directx/shader/shader_register_id.h"

namespace legend {
namespace player {

//コンストラク
Player::Player() : actor::Actor<physics::BoundingBox>() {}

//デストラクタ
Player::~Player() {}

//初期化
bool Player::Init(const InitializeParameter& parameter) {
  this->transform_ = parameter.transform;
  this->collision_ =
      physics::BoundingBox(transform_.GetPosition(), transform_.GetRotation(),
                           transform_.GetScale() * 0.1f);
  this->collision_.SetLength(parameter.bouding_box_length.x,
                             parameter.bouding_box_length.y,
                             parameter.bouding_box_length.z);
  min_power_ = parameter.min_power;
  max_power_ = parameter.max_power;

  up_power_ = true;
  is_set_power_ = false;
  move_end_ = false;

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  //トランスフォームバッファを作成する
  if (!transform_cb_.Init(
          device, directx::shader::ConstantBufferRegisterID::TRANSFORM,
          device.GetLocalHandle(directx::descriptor_heap::heap_parameter::
                                    LocalHeapID::PLAYER_MOVE_VIEWER),
          L"Transform ConstantBuffer")) {
    return false;
  }

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
      math::Vector3(change_amount_velocity_ - input_velocity_).Magnitude() >=
          1.0f) {
    is_move_ = true;
  }

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  SetVelocity();
  SetImpulse();

  Move();

  return true;
}

//移動
void Player::Move() {
  if (!is_move_) return;

  math::Vector3 velocity = math::Vector3::kZeroVector;
  while (stick_velocities_.size() > 0) {
    if (velocity.Magnitude() < stick_velocities_.front().Magnitude()) {
      velocity = stick_velocities_.front();
      stick_velocities_.erase(stick_velocities_.begin());
      continue;
    }
    stick_velocities_.erase(stick_velocities_.begin());
  }
  //移動速度がゼロだったらreturn
  if (velocity == math::Vector3::kZeroVector) {
    ResetParameter();
    move_end_ = true;
    return;
  }

  //移動距離を求める
  float length =
      math::util::Sqrt(velocity.x * velocity.x + velocity.z * velocity.z);

  //実際に動く距離
  float x = velocity.x / length;
  float z = velocity.z / length;

  //減速計算
  deceleration_x_ = x / (length * length);
  deceleration_z_ = z / (length * length);

  //移動処理
  math::Vector3 v = math::Vector3(x, 0, z);
  math::Vector3 position = GetPosition() + v * impulse_ * power_ * update_time_;
  SetPosition(position);

  //設置していなければここまで
  if (!collision_.GetOnGround()) return;
  Deceleration(2);
}

void Player::SetPosition(math::Vector3 position) {
  transform_.SetPosition(position);
  collision_.SetPosition(position);
}

//速度の設定
void Player::SetVelocity(math::Vector3 velocity) { velocity_ = velocity; }

void Player::SetRotation(math::Quaternion rotation) {
  transform_.SetRotation(rotation);
  collision_.SetRotation(rotation);
}

void Player::SetVelocity() {
  if (is_move_) return;

  input::InputManager& input = game::GameDevice::GetInstance()->GetInput();
  //ゲームパッドが一つだけ接続されている間

  if (input.GetGamepad()->GetCount() != 1) return;
  input_velocity_.x = -input.GetGamepad()->GetStickLeft().x;
  input_velocity_.z = -input.GetGamepad()->GetStickLeft().y;

  velocity_update_time_ += update_time_;
  if (velocity_update_time_ < change_time_) return;

  //左スティックの傾きに合わせて値を入れる
  velocity_.x = input_velocity_.x;
  velocity_.z = input_velocity_.z;
  if (velocity_.Magnitude() >= 0.1f) is_input_ = true;

  change_amount_velocity_ = velocity_;
  stick_velocities_.push_back(velocity_);
  while (stick_velocities_.size() > max_stick_velocity_num_) {
    stick_velocities_.erase(stick_velocities_.begin());
  }
  velocity_update_time_ = 0;
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

//重力による移動
void Player::UpdateGravity(float gravity) {
  math::Vector3 position =
      GetPosition() + math::Vector3(0, gravity, 0) * update_time_;
  SetPosition(position);
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
  stick_velocities_.clear();
  is_move_ = false;
  velocity_update_time_ = 0;
}

//移動終了判定のリセット
void Player::ResetMoveEnd() { move_end_ = false; }

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

  change_amount_velocity_ = velocity_;
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

bool Player::GetMoveEnd() const { return move_end_; }
}  // namespace player
}  // namespace legend