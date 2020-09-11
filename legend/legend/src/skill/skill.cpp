#include "src/skill/skill.h"

#include "src/player/player.h"

namespace legend {
namespace skill {
//�R���X�g���N�^
Skill::Skill() : Parent(L"Skill") {}

//�f�X�g���N�^
Skill::~Skill() {}

//������
bool Skill::Init(actor::IActorMediator* mediator, player::Player* player) {
  if (!Actor::Init(mediator)) {
    return false;
  }
  return true;
}

//�X�V
bool Skill::Update() { return true; }

//�g�p
void Skill::Use() { is_use_ = true; }

//����
void Skill::Action() {}

//�X�L�����o�X�V
void Skill::ProductionUpdate() { is_production_ = false; }

//�X�L���I��
void Skill::EndAction() {}

//�Ďg�p�^�[���̍X�V
void Skill::RemaingRecastTurnUpdate() {
  //�J�E���g���X�V
  remaining_recast_turn_--;
}

//�����X�L���̍��W����
void Skill::AdjustPosition(math::Vector3 position) {
  transform_.SetPosition(player_->GetPosition() + position);
  if (GetName() != skill_name::SKILLCOMPASS) {
    transform_.SetRotation(player_->GetRotation());
  } else {
    const math::Vector3 angle(0.0f, 0.0f, 90.0f);
    math::Quaternion rotation =
        math::Quaternion::FromEular(angle * math::util::DEG_2_RAD);
    transform_.SetRotation(player_->GetRotation() * rotation);
  }
  box_->SetTransform(transform_);
}

//�I�𒆂̃X�L���̈ʒu��ύX
void Skill::ChangePosition(math::Vector3 position) {
  transform_.SetPosition(position);
  if (GetName() != skill_name::SKILLCOMPASS) {
    transform_.SetRotation(player_->GetRotation());
  } else {
    const math::Vector3 angle(0.0f, 0.0f, 90.0f);
    math::Quaternion rotation =
        math::Quaternion::FromEular(angle * math::util::DEG_2_RAD);
    transform_.SetRotation(player_->GetRotation() * rotation);
  }
  box_->SetTransform(transform_);
}

//�c��g�p�񐔂̎擾
i32 Skill::GetRemainingUsableCount() { return remaining_usable_count_; }

//���L���X�g�^�[���̎擾
i32 Skill::GetRemainingRecastTurn() { return remaining_recast_turn_; }

//�X�L�������^�C�~���O
SkillActivationTiming Skill::GetActivetionTiming() {
  return activetion_timing_;
}

//�X�L���I���^�C�~���O�̎擾
SkillEffectEndTiming Skill::GetEndTiming() { return end_timing_; }

//�X�L���A�C�R���̎擾
std::shared_ptr<directx::buffer::Texture2D> Skill::GetIconTexture() const {
  return skill_icon_texture_;
}

//�X�L���g�p���̉摜�̎擾
std::shared_ptr<directx::buffer::Texture2D> Skill::GetIconUsingTexture() const {
  return skill_icon_using_texture_;
}

//�X�L�������摜�̎擾
std::shared_ptr<directx::buffer::Texture2D> Skill::GetEquipmentTexture() const {
  return equipment_texture_;
}

//�X�L�������摜�̎擾
std::shared_ptr<directx::buffer::Texture2D> Skill::GetExplanationTexture()
    const {
  return skill_explanation_texture_;
}

//�g�p�t���O�̎擾
bool Skill::GetUseFlag() { return is_use_; }

//���o���t���O�̎擾
bool Skill::ProductionFlag() { return is_production_; }

//�R���C�_�[�̍폜
void Skill::RemoveCollider() { mediator_->RemoveCollider(box_); }

//���o�I��蔻��̎擾
bool Skill::EndSkillProduction() const { return (is_use_ && !is_production_); }

//�X�L�����̐ݒ�
void Skill::SetName(const std::wstring name) { Parent::name_ = name; }

//�X�L�����̎擾
std::wstring Skill::GetName() const { return Parent::name_; }

}  // namespace skill
}  // namespace legend