#include "src/player/player.h"

namespace legend {
namespace player {
//�R���X�g���N�^
Player::Player()
    : transfrom_(),
      velocity_(math::Vector3::kZeroVector),
      min_power_(0),
      max_power_(1) {
  transfrom_.SetScale(math::Vector3(0.1f, 0.1f, 0.1f));
  obb_ = physics::BoundingBox();
  obb_.SetLength(2, 1, 2);
  is_move_ = false;
  impulse_ = min_power_;
  deceleration_x_ = deceleration_z_ = 0;
  input_velocity_ = math::Vector3::kZeroVector;
  is_set_power_ = false;
  up_power_ = true;
  is_input_ = false;
}

Player::Player(math::Vector3 position, math::Quaternion rotation,
               math::Vector3 scale, float min_power, float max_power)
    : transfrom_(),
      velocity_(math::Vector3::kZeroVector),
      min_power_(min_power),
      max_power_(max_power) {
  transfrom_.SetPosition(position);
  transfrom_.SetRotation(rotation);
  transfrom_.SetScale(scale);
  obb_ = physics::BoundingBox(transfrom_.GetPosition(),
                              transfrom_.GetRotation().ToEular(),
                              transfrom_.GetScale());
  obb_.SetLength(2, 1, 2);
  is_move_ = false;
  impulse_ = min_power_;
  deceleration_x_ = deceleration_z_ = 0;
  input_velocity_ = math::Vector3::kZeroVector;
  is_set_power_ = false;
  up_power_ = true;
  is_input_ = false;
}

//�f�X�g���N�^
Player::~Player() {}

//������
bool Player::Initilaize(directx::DirectX12Device& device) {
  if (!obb_.Initialize(device)) {
    return false;
  }

  return true;
}

//�X�V
bool Player::Update() {
  obb_.Update();

  update_time_ = static_cast<float>(
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds());

  if (is_input_ && input_velocity_.Magnitude() <= 0.1f) {
    is_move_ = true;
  }

  return true;
}

//�`��
void Player::Draw(directx::DirectX12Device& device) { obb_.Draw(device); }

//�ړ�
void Player::Move() {
  if (!is_move_) return;

  math::Vector3 velocity = math::Vector3::kZeroVector;

  if (velocity.Magnitude() < stick_velocities_.front().Magnitude())
    velocity = stick_velocities_.front();
  stick_velocities_.erase(stick_velocities_.begin());

  //�ړ����x���[����������return
  if (velocity == math::Vector3::kZeroVector) {
    ResetParameter();
    return;
  }

  //�ړ����������߂�
  float length =
      math::util::Sqrt(velocity.x * velocity.x + velocity.z * velocity.z);

  //���ۂɓ�������
  float x = velocity.x / length;
  float z = velocity.z / length;

  //�����v�Z
  deceleration_x_ = x / (length * length) * update_time_;
  deceleration_z_ = z / (length * length) * update_time_;

  //�ړ�����
  math::Vector3 v = math::Vector3(x, 0, z);
  math::Vector3 position =
      transfrom_.GetPosition() + v * impulse_ * power_ * update_time_;
  SetPosition(position);

  Deceleration(2);
}

void Player::SetPosition(math::Vector3 position) {
  transfrom_.SetPosition(position);
  obb_.SetPosition(position);
}

//���x�̐ݒ�
void Player::SetVelocity(math::Vector3 velocity) { velocity_ = velocity; }

void Player::SetVelocity() {
  input::InputManager& input = game::GameDevice::GetInstance()->GetInput();
  //�Q�[���p�b�h��������ڑ�����Ă����

  if (input.GetGamepad()->GetCount() != 1) return;
  input_velocity_.x = -input.GetGamepad()->GetStickLeft().x;
  input_velocity_.z = -input.GetGamepad()->GetStickLeft().y;

  if (is_move_) return;

  //���X�e�B�b�N�̌X���ɍ��킹�Ēl������
  velocity_.x = input_velocity_.x;
  velocity_.z = input_velocity_.z;
  if (velocity_.Magnitude() >= 0.1f) is_input_ = true;

  before_velocity_ = velocity_;
  stick_velocities_.push_back(velocity_);
  while (stick_velocities_.size() > max_stick_velocity_num_) {
    stick_velocities_.erase(stick_velocities_.begin());
  }
}

//�p���[�̐ݒ�
void Player::SetImpulse() {
  input::InputManager& input = game::GameDevice::GetInstance()->GetInput();
  //�Q�[���p�b�h��������ڑ�����Ă����
  if (input.GetGamepad()->GetCount() != 1) return;

  //�p���[�������I������return
  if (is_set_power_ || !is_input_) return;

  //���X�e�B�b�N�̌X����0.1�ȉ����A�������͂��Ă�����p���[�����������ɂ���
  if (math::util::Abs(input.GetGamepad()->GetStickLeft().Magnitude()) <= 0.1f &&
      is_input_) {
    is_set_power_ = true;
    return;
  }

  //�p���[��������
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

//�ړ��ɕK�v�ȃp�����[�^��������
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
}

//����
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

//���W�̎擾
math::Vector3 Player::GetPosition() const { return transfrom_.GetPosition(); }

//�ړ��ʂ̎擾
math::Vector3 Player::GetVelocity() const { return velocity_; }

math::Quaternion Player::GetRotation() const {
  return transfrom_.GetRotation();
}

float Player::GetImpulse() const { return impulse_; }

physics::BoundingBox& Player::GetOBB() {
  physics::BoundingBox& obb = obb_;
  return obb;
}
}  // namespace player
}  // namespace legend