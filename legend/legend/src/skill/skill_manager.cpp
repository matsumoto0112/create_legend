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
  //全て所持しようとした場合にあふれるかどうか
  if (skills_.size() + this_turn_get_skills_.size() <= skill_max_count_) {
    for (auto&& skill : this_turn_get_skills_) {
      AddSkill(skill);
    }
  }
  //溢れる場合
  else {
    //現状無視?
  }

  this_turn_get_skills_.clear();

  //プレイヤーの行動後に終わるスキルの更新処理

  //プレイヤー行動後に発動するスキルの発動
  for (auto&& skill : skills_) {
    if (!skill->GetUseFlag()) continue;
    if (skill->GetActivetionTiming() != SkillActivationTiming::PLAYER_TURN_END)
      continue;
    skill->Action();
  }

  //今はない
}

void SkillManager::EnemyTurnEnd() {
  //エネミーの行動が終わった際に更新される内容

  //エネミー行動後に発動するスキルの発動
  for (auto&& skill : skills_) {
    if (!skill->GetUseFlag()) continue;
    if (skill->GetActivetionTiming() != SkillActivationTiming::ENEMY_TURN_END)
      continue;
    skill->Action();
  }

  //スキルの再使用ターンの更新
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