#include "src/skill/skill_manager.h"

#include "src/game/game_device.h"
#include "src/player/player.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace skill {

//�R���X�g���N�^
SkillManager::SkillManager() {}

//�f�X�g���N�^
SkillManager::~SkillManager() {}

//������
void SkillManager::Init(actor::IActorMediator* mediator,
                        player::Player* player) {
  this->mediator_ = mediator;
  player_ = player;
  select_ui_.Init();
  select_move_ = false;
  player_ui_.Init();
  skill_max_count_ = 5;
  current_mode_ = Mode::NONE;
  is_equipment_production_ = false;
  complete_eqquipment_ = false;
  something_skill_use_ = false;
  select_skill_number_ = 0;
  previous_select_number_ = 0;
}

//�X�L���擾��
void SkillManager::GetSkill(std::shared_ptr<Skill> skill) {
  skill->Init(mediator_, player_);
  this_turn_get_skills_.push_back(skill);
}

//�X�L���̊l��
void SkillManager::AddSkill(std::shared_ptr<Skill> skill) {
  //�ő及�����łȂ���Βǉ�
  if (skills_.size() >= skill_max_count_) return;

  skills_.push_back(skill);
  SetPosition(skill, static_cast<i32>(skills_.size() - 1));
  select_ui_.AddSkill(skill.get());
  player_ui_.AddEquipmentUI(skill.get());
}

//�X�V
void SkillManager::Update() {
  i32 i = 0;
  for (auto&& skill : skills_) {
    skill->Update();
    if (skill->ProductionFlag()) {
      i++;
      continue;
    }

    if (!select_ui_.GetIsSelectMode()) SetPosition(skill, i);
    i++;
  }

  player_ui_.Update();
}

void SkillManager::EquipmentProductionUpdate() {
  //���̃^�[���ɉ�������Ă��Ȃ����́A�����X�L�������ő�Ȃ�A�^�[����؂�ւ���
  if (this_turn_get_skills_.empty() ||
      (skills_.size() >= skill_max_count_ && !complete_eqquipment_)) {
    something_skill_use_ = false;
    mediator_->PlayerCompleteEquipment();
    return;
  }

  //�����ǉ����I���ĂȂ���Ώ㏸��Ԃɐ؂�ւ�
  if (!complete_eqquipment_) {
    player_->GetCollider()->ApplyCentralImpulse(math::Vector3(0, 1, 0));
    current_mode_ = Mode::RISE_PLAYER;
  }

  //�㏸
  if (current_mode_ == Mode::RISE_PLAYER) {
    //���̍����܂ōs�����瑕����ǉ�
    if (player_->GetTransform().GetPosition().y >= 30.0f) {
      if (!complete_eqquipment_) {
        for (auto&& skill : this_turn_get_skills_) {
          AddSkill(skill);
        }
        complete_eqquipment_ = true;
      }
      //���~��Ԃɐ؂�ւ�
      current_mode_ = Mode::FALL_PLAYER;
      player_->GetCollider()->ApplyCentralImpulse(
          player_->GetCollider()->GetVelocity() * -1);
    }
  }
  //���~
  if (current_mode_ == Mode::FALL_PLAYER) {
    //���̍����܂ōs������^�[����؂�ւ���
    if (player_->GetTransform().GetPosition().y <= 1.8f) {
      current_mode_ = Mode::NONE;
      something_skill_use_ = false;
      mediator_->PlayerCompleteEquipment();
      complete_eqquipment_ = false;
      this_turn_get_skills_.clear();
    }
  }
}

//�v���C���[�s����̏���
void SkillManager::PlayerTurnEnd() {
  //�v���C���[�̍s����ɏI���X�L���̍X�V����
  for (auto&& skill : skills_) {
    if (!skill->EndSkillProduction()) continue;
    if (skill->GetEndTiming() != SkillEffectEndTiming::PLAYER_TURN_END)
      continue;
    skill->EndAction();
  }

  //�v���C���[�s����ɔ�������X�L���̔���
  for (auto&& skill : skills_) {
    if (!skill->GetUseFlag()) continue;
    if (skill->GetActivetionTiming() != SkillActivationTiming::PLAYER_TURN_END)
      continue;
    skill->Action();
  }
  //���͂Ȃ�
}

//�G�l�~�[�s����̏���
void SkillManager::EnemyTurnEnd() {
  //�G�l�~�[�̍s�����I������ۂɍX�V�������e
  for (auto&& skill : skills_) {
    if (!skill->EndSkillProduction()) continue;
    if (skill->GetEndTiming() != SkillEffectEndTiming::ENEMY_TURN_END) continue;
    skill->EndAction();
  }

  //�G�l�~�[�s����ɔ�������X�L���̔���
  for (auto&& skill : skills_) {
    if (!skill->GetUseFlag()) continue;
    if (skill->GetActivetionTiming() != SkillActivationTiming::ENEMY_TURN_END)
      continue;
    skill->Action();
  }

  //�X�L���̍Ďg�p�^�[���̍X�V
  for (auto&& skill : skills_) {
    skill->RemaingRecastTurnUpdate();
  }
}

//�`��
void SkillManager::Draw() {
  for (auto&& skill : skills_) {
    skill->Draw();
  }
  select_ui_.Draw();
  player_ui_.Draw();
}

//���o����
bool SkillManager::IsProductionNow() {
  for (auto&& skill : skills_) {
    if (skill->ProductionFlag()) return true;
  }
  return false;
}

//�X�L�����X�g�̎擾
std::vector<std::shared_ptr<Skill>> SkillManager::GetSkillList() {
  return skills_;
}

//�X�L���̍폜
void SkillManager::RemoveSkill() {
  for (i32 i = 0; i < skills_.size(); i++) {
    if (skills_[i]->GetRemainingUsableCount() >= 1) continue;

    skills_[i]->RemoveCollider();
    skills_.erase(skills_.begin() + i);
    select_ui_.RemoveSkillUI(i);
    player_ui_.RemoveEquipmentUI(i);
    i--;
    select_ui_.SelectSkillNumber(0);
  }
}

//�X�L���̑I��
bool SkillManager::SelectSkill() {
  if (IsProductionNow()) return true;

  auto& input = game::GameDevice::GetInstance()->GetInput();
  if (input.GetGamepad()->GetButtonDown(input::joy_code::LB)) {
    select_ui_.ChangeIsSelectMode();
  }

  if (select_ui_.GetIsSelectMode()) {
    //�ړ������1�񂾂��ɂ���
    if (input.GetGamepad()->GetStickLeft().x >= 0.8f && !select_move_) {
      select_skill_number_++;
      if (select_skill_number_ >= skills_.size()) select_skill_number_ = 0;
      select_move_ = true;
    } else if (input.GetGamepad()->GetStickLeft().x <= -0.8f && !select_move_) {
      select_skill_number_--;
      if (select_skill_number_ < 0)
        select_skill_number_ = static_cast<i32>(skills_.size() - 1);
      select_move_ = true;
    }
    if (input.GetGamepad()->GetStickLeft().x > -0.8f &&
        input.GetGamepad()->GetStickLeft().x < 0.8f)
      select_move_ = false;
    //�I�𒆂̔ԍ��ɍ��킹�ĕ\���֌W���X�V
    select_ui_.SelectSkillNumber(select_skill_number_);
    SetPositionSelectSkill(select_skill_number_);
    return true;
  } else {
    previous_select_number_ = 0;
    return false;
  }
}

//�X�L���̎g�p
void SkillManager::UseSkill() {
  //�I�𒆁A���o���A���̃^�[���ɃX�L�����g�p�ςłȂ���Ύg�p�ł���
  if (!select_ui_.GetIsSelectMode() || IsProductionNow() ||
      something_skill_use_)
    return;

  auto& input = game::GameDevice::GetInstance()->GetInput();
  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();
  i32 skill_num = select_ui_.GetSkillNumber();
  if (input.GetGamepad()->GetButtonDown(input::joy_code::A)) {
    skills_[skill_num]->Use();
    something_skill_use_ = true;
    select_skill_number_ = 0;
    previous_select_number_ = 0;
    audio.Start(util::resource::resource_names::audio::SKILL_DECISION, 1.0f);
    if (skills_[skill_num]->GetActivetionTiming() !=
        SkillActivationTiming::NOW) {
      select_ui_.ChangeIsSelectMode();
    }
  }
}

//�X�L���I��
void SkillManager::EndSkill() {
  for (auto&& skill : skills_) {
    if (skill->EndSkillProduction()) {
      if (skill->GetActivetionTiming() == SkillActivationTiming::NOW) {
        select_ui_.ChangeIsSelectMode();
      }
      break;
    }
  }
}

void SkillManager::SetPosition(std::shared_ptr<Skill> skill, i32 skill_num) {
  math::Vector3 pos;
  if (skill_num == 0)
    pos = math::Vector3::kUpVector;
  else if (skill_num == 1)
    pos = math::Vector3::kRightVector * 1.5f;
  else if (skill_num == 2)
    pos = math::Vector3::kLeftVector * 1.5f;
  else if (skill_num == 3)
    pos = math::Vector3::kUpVector + math::Vector3::kRightVector +
          math::Vector3::kBackwardVector;
  else
    pos = math::Vector3::kUpVector + math::Vector3::kLeftVector +
          math::Vector3::kBackwardVector;
  pos = math::Matrix4x4::MultiplyCoord(
      pos, player_->GetTransform().GetRotation().ToMatrix());
  skill->AdjustPosition(pos);
}

void SkillManager::SetPositionSelectSkill(i32 skill_num) {
  if (skill_num == previous_select_number_) return;

  const math::Vector3 skill1_pos =
      skills_[previous_select_number_]->GetTransform().GetPosition();
  const math::Vector3 skill2_pos =
      skills_[skill_num]->GetTransform().GetPosition();
  skills_[previous_select_number_]->ChangePosition(skill2_pos);
  skills_[skill_num]->ChangePosition(skill1_pos);

  previous_select_number_ = skill_num;
}
}  // namespace skill
}  // namespace legend