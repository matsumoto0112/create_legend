#include "skill_manager.h"

#include "src\\stdafx.h"

namespace legend {
namespace skill {

SkillManager::SkillManager() {}

SkillManager::~SkillManager() {}

void SkillManager::Init() { select_ui_.Init(); }

void SkillManager::Update() {}

void SkillManager::GetSkill(i32 skill_id) {
  //Skill skill;
  //this_turn_get_skills_.push_back(skill);
}

void SkillManager::AddSkill(const Skill& skill) {
  skills_.push_back(skill);
  select_ui_.AddSkill();
}

void SkillManager::AddSkill()
{
    select_ui_.AddSkill();
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

  //�v���C���[�̍s�����I������ۂɍX�V�������e

  //�v���C���[�s����ɔ�������X�L���̔���
  for (auto&& skill : skills_) {
      if (!skill.GetUseFlag()) continue;
      if (skill.GetActivetionTiming() != SkillActivationTiming::PLAYER_TURN_END) continue;
      skill.Action();
  }

  //���͂Ȃ�
}

void SkillManager::EnemyTurnEnd() {
  //�G�l�~�[�̍s�����I������ۂɍX�V�������e

  //�G�l�~�[�s����ɔ�������X�L���̔���
  for (auto&& skill : skills_) {
      if (!skill.GetUseFlag()) continue;
      if (skill.GetActivetionTiming() != SkillActivationTiming::ENEMY_TURN_END) continue;
      skill.Action();
  }

  //�X�L���̍Ďg�p�^�[���̍X�V
  for (auto&& skill : skills_) {
    skill.RemaingRecastTurnUpdate();
  }
}

void SkillManager::Draw() { select_ui_.Draw(); }

}  // namespace skill
}  // namespace legend