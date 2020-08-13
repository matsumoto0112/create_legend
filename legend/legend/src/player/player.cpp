#include "src/player/player.h"

#include "src/directx/shader/shader_register_id.h"

namespace legend {
namespace player {

//�R���X�g���N
Player::Player() : Parent(L"Player") {}

//�f�X�g���N�^
Player::~Player() {}

//������
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

//�X�V
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

//���W�̐ݒ�
void Player::SetPosition(math::Vector3 position) {
  transform_.SetPosition(position);
  collision_.SetPosition(position);
}

//��]�̐ݒ�
void Player::SetRotation(math::Quaternion rotation) {
  transform_.SetRotation(rotation);
  collision_.SetRotation(rotation);
}

//���x�̐ݒ�
void Player::SetVelocity() {
  if (is_move_) return;

  input::InputManager& input = game::GameDevice::GetInstance()->GetInput();
  //�Q�[���p�b�h��������ڑ�����Ă����

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

//���x�̐ݒ�
void Player::SetVelocity(math::Vector3 velocity) {
  if (!is_move_) return;

  velocity_ = velocity;
}

//�p���[�̐ݒ�
void Player::SetImpulse() {
  input::InputManager& input = game::GameDevice::GetInstance()->GetInput();

  //�p���[�������I������return
  if (is_set_power_ || !is_input_) return;

  //���X�e�B�b�N�̌X����0.1�ȉ����A�������͂��Ă�����p���[�����������ɂ���
  if (input.GetGamepad()->GetStickLeft().Magnitude() <= 0.1f && is_input_) {
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
  change_amount_velocity_ = math::Vector3::kZeroVector;
  deceleration_x_ = deceleration_z_ = 0;
  input_velocity_ = math::Vector3::kZeroVector;
  is_move_ = false;
  velocity_update_time_ = 0;
}

//�ړ��I������̃��Z�b�g
void Player::ResetMoveEnd() { move_end_ = false; }

//���W�̎擾
math::Vector3 Player::GetPosition() const { return transform_.GetPosition(); }

//�ړ��ʂ̎擾
math::Vector3 Player::GetVelocity() const { return velocity_; }

//��]�̎擾
math::Quaternion Player::GetRotation() const {
  return transform_.GetRotation();
}

//������͂̊����̎擾
float Player::GetImpulse() const { return impulse_; }

//������͂̎擾
float Player::GetPower() const { return power_; }

//�ړ��I������̎擾
bool Player::GetMoveEnd() const { return move_end_; }

//�ړ�����̎擾
bool Player::GetIsMove() const { return is_move_; }
}  // namespace player
}  // namespace legend