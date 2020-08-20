#include "src/player/player.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace player {

//�R���X�g���N
Player::Player() : Parent(L"Player") {}

//�f�X�g���N�^
Player::~Player() {}

//������
bool Player::Init(actor::IActorMediator* mediator,
                  const InitializeParameter& parameter) {
  if (!Parent::Init(mediator)) {
    return false;
  }
  if (!Parent::InitBuffer()) {
    return false;
  }

  this->transform_ = parameter.transform;
  bullet::BoundingBox::InitializeParameter params;
  params.position = transform_.GetPosition();
  params.rotation = transform_.GetRotation();
  params.scale = parameter.bouding_box_length;
  params.mass = 1.0f;
  params.friction = 0.8f;
  params.restitution = 1.0f;
  box_ = std::make_shared<bullet::BoundingBox>(&this->transform_, params);
  mediator_->AddCollider(box_);

  // this->collision_.SetCollisionCallback(
  //    [&](actor::ActorType type) { Player::OnCollisionHit(type); });
  // this->collision_.SetTriggerCallback(
  //    [&](actor::ActorType type) { Player::OnTriggerHit(type); });
  min_power_ = parameter.min_power;
  max_power_ = parameter.max_power;

  up_power_ = true;
  is_set_power_ = false;
  strength_ = 1.0f;
  max_strength_ = parameter.max_strength;
  min_strength_ = parameter.min_strength;

  // box_ = std::make_shared<bullet::BoundingBox>();

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  model_ =
      resource.GetModel().Get(util::resource::resource_names::model::PLAYER);

  //�X�L���}�l�[�W���[�̏�����
  skill_manager_.Init();

  return true;
}

//�X�V
bool Player::Update() {
  update_time_ =
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();

  //�X�L���̃f�o�b�N�p��GUI
  if (ImGui::Begin("Skill")) {
    if (ImGui::Button("Add Skill")) {
      std::shared_ptr<skill::SkillPencil> skill =
          std::make_shared<skill::SkillPencil>();
      skill->Init(this);
      skill_manager_.AddSkill(skill);
    }
  }
  ImGui::End();
  //�X�L���}�l�[�W���[�̍X�V
  skill_manager_.Update();

  if (game::GameDevice::GetInstance()->GetInput().GetKeyboard()->GetKeyDown(
          input::key_code::A)) {
    box_->ApplyCentralImpulse(math::Vector3(20.0f, 0.0f, 0.0f));
  }

  if (skill_manager_.IsProductionNow()) {
    return true;
  }

  if (change_amount_velocity_.Magnitude() - input_velocity_.Magnitude() >=
      0.5f) {
    is_move_ = true;
    mediator_->PlayerMoveStartEvent();
  }

  if (GetMoveEnd()) {
    ResetParameter();
    mediator_->PlayerMoveEndEvent();
  }

  return true;
}

void Player::Draw() {
  //�v���C���[�̕`��
  actor::Actor::Draw();

  //�X�L���}�l�[�W���[�̕`��
  skill_manager_.Draw();
}

//���W�̐ݒ�
void Player::SetPosition(math::Vector3 position) {
  transform_.SetPosition(position);
}

//��]�̐ݒ�
void Player::SetRotation(math::Quaternion rotation) {
  transform_.SetRotation(rotation);
}

//�X�P�[���̐ݒ�
void Player::SetScale(math::Vector3 scale) { transform_.SetScale(scale); }

//���x�̐ݒ�
void Player::SetVelocity() {
  if (is_move_) return;

  input::InputManager& input = game::GameDevice::GetInstance()->GetInput();
  input_velocity_.x = -input.GetGamepad()->GetStickLeft().x;
  input_velocity_.z = -input.GetGamepad()->GetStickLeft().y;

  velocity_update_time_ += update_time_;
  if (velocity_update_time_ < change_time_) return;

  velocity_ = input_velocity_;
  if (velocity_.Magnitude() >= 0.2f) is_input_ = true;

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
  if (is_set_power_ || !is_input_) return;

  input::InputManager& input = game::GameDevice::GetInstance()->GetInput();
  if (input.GetGamepad()->GetStickLeft().Magnitude() >= 0.2f) {
    if (up_power_) {
      impulse_ += 0.1f * update_time_ * 5;
      if (impulse_ >= max_power_) {
        impulse_ = max_power_;
        up_power_ = false;
      }
    } else {
      impulse_ -= 0.1f * update_time_ * 5;
      if (impulse_ <= min_power_) {
        impulse_ = min_power_;
        up_power_ = true;
      }
    }
  } else {
    is_set_power_ = true;
  }
}

//�ړ��ɕK�v�ȃp�����[�^��������
void Player::ResetParameter() {
  is_input_ = false;
  is_set_power_ = false;
  impulse_ = 0;
  up_power_ = true;
  change_amount_velocity_ = math::Vector3::kZeroVector;
  input_velocity_ = math::Vector3::kZeroVector;
  is_move_ = false;
  velocity_update_time_ = 0;
}

//���W�̎擾
math::Vector3 Player::GetPosition() const { return transform_.GetPosition(); }

//�X�P�[���̎擾
math::Vector3 Player::GetScale() const { return transform_.GetScale(); }

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

//�ړ����I���Ă��邩���擾
bool Player::GetMoveEnd() const {
  bool is_end = (is_move_ && velocity_ == math::Vector3::kZeroVector);
  return is_end;
}

//�ړ�����̎擾
bool Player::GetIsMove() const { return is_move_; }

void Player::OnCollisionHit(actor::ActorType type) {
  // MY_LOG(L"Collision Hit: %d", type);
}

void Player::OnTriggerHit(actor::ActorType type) {
  // MY_LOG(L"Trigger Hit: %d", type);
}

//�����p�����[�^�̍X�V
void Player::UpdateStrength(const float& add_strength) {
  strength_ += add_strength;
  if (strength_ > max_strength_)
    strength_ = max_strength_;
  else if (strength_ < min_strength_)
    strength_ = min_strength_;
}

//�����x�������擾
float Player::GetStrength() const { return strength_; }
}  // namespace player
}  // namespace legend