#include "skill_manager.h"

#include "src/player/player.h"

namespace legend {
namespace skill {

SkillManager::SkillManager() {}

SkillManager::~SkillManager() {}

void SkillManager::Init() { select_ui_.Init(); }

void SkillManager::GetSkill(i32 skill_id, player::Player* player) {
  std::shared_ptr<SkillPencil> skill;
  skill->Init(player);
  this_turn_get_skills_.push_back(skill);
}

void SkillManager::AddSkill(std::shared_ptr<Skill> skill) {
  skills_.push_back(skill);
  select_ui_.AddSkill();
}

// void SkillManager::AddSkill() { select_ui_.AddSkill(); }

void SkillManager::Update() {
  for (auto&& skill : skills_) {
    skill->Update();
  }
}

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

  //�v���C���[�s����ɔ�������X�L���̔���
  for (auto&& skill : skills_) {
    if (!skill->GetUseFlag()) continue;
    if (skill->GetActivetionTiming() != SkillActivationTiming::PLAYER_TURN_END)
      continue;
    skill->Action();
  }

  //���͂Ȃ�
}

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

void SkillManager::Draw() {
  for (auto&& skill : skills_) {
    skill->Draw();
  }
  select_ui_.Draw();
}

bool SkillManager::IsProductionNow() {
  for (auto&& skill : skills_) {
    if (skill->ProductionFlag()) return true;
  }
  return false;
}

}  // namespace skill
}  // namespace legend