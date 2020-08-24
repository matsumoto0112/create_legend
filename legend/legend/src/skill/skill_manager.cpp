#include "skill_manager.h"

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
void SkillManager::Init(actor::IActorMediator* mediator) {
  this->mediator_ = mediator;
  select_ui_.Init();
  select_move_ = false;
}

//�X�L���擾��
void SkillManager::GetSkill(i32 skill_id, player::Player* player) {
  std::shared_ptr<SkillPencil> skill;
  skill->Init(mediator_, player);
  this_turn_get_skills_.push_back(skill);
}

//�X�L���̊l��
void SkillManager::AddSkill(std::shared_ptr<Skill> skill) {
  skills_.push_back(skill);
  select_ui_.AddSkill();
}

// void SkillManager::AddSkill() { select_ui_.AddSkill(); }

//�X�V
void SkillManager::Update() {
  for (auto&& skill : skills_) {
    skill->Update();
  }
}

//�v���C���[�s����̏���
void SkillManager::PlayerTurnEnd() {
  //�S�ď������悤�Ƃ����ꍇ�ɂ��ӂ�邩�ǂ���
  if (skills_.size() + this_turn_get_skills_.size() <= skill_max_count_) {
    for (auto&& skill : this_turn_get_skills_) {
      AddSkill(skill);
    }
  }
  //����ꍇ
  else {
    //���󖳎�?
  }

  this_turn_get_skills_.clear();

  //�v���C���[�̍s����ɏI���X�L���̍X�V����
  for (auto&& skill : skills_) {
    if (!skill->GetUseFlag()) continue;
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
    if (skill->GetUseFlag()) continue;
    skill->Draw();
  }
  select_ui_.Draw();
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
    i--;
    select_ui_.SelectSkillNumber(0);
  }
}

bool SkillManager::SelectSkill() {
  auto& input = game::GameDevice::GetInstance()->GetInput();
  if (input.GetGamepad()->GetButtonDown(input::joy_code::LB)) {
    select_ui_.ChangeIsSelectMode();
  }

  if (select_ui_.GetIsSelectMode()) {
    i32 number = select_ui_.GetSkillNumber();
    //�ړ������1�񂾂��ɂ���
    if (input.GetGamepad()->GetStickLeft().x >= 0.8f && !select_move_) {
      number++;
      select_move_ = true;
    } else if (input.GetGamepad()->GetStickLeft().x <= -0.8f && !select_move_) {
      number--;
      select_move_ = true;
    }
    if (input.GetGamepad()->GetStickLeft().x > -0.8f &&
        input.GetGamepad()->GetStickLeft().x < 0.8f)
      select_move_ = false;
    select_ui_.SelectSkillNumber(number);
    return true;
  } else {
    return false;
  }
}

void SkillManager::UseSkill() {
  if (!select_ui_.GetIsSelectMode()) return;

  auto& input = game::GameDevice::GetInstance()->GetInput();
  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();
  i32 skill_num = select_ui_.GetSkillNumber();
  if (input.GetGamepad()->GetButtonDown(input::joy_code::A)) {
    skills_[skill_num]->Use();
    audio.Start(util::resource::resource_names::audio::SKILL_DECISION, 1.0f);
  }
}

void SkillManager::SelectUpdate() {
  UseSkill();
  RemoveSkill();
}

}  // namespace skill
}  // namespace legend