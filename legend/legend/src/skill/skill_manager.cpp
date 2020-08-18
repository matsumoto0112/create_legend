#include "skill_manager.h"

#include "src/player/player.h"

namespace legend {
namespace skill {

//コンストラクタ
SkillManager::SkillManager() {}

//デストラクタ
SkillManager::~SkillManager() {}

//初期化
void SkillManager::Init() { select_ui_.Init(); }

//スキル取得時
void SkillManager::GetSkill(i32 skill_id, player::Player* player) {
  std::shared_ptr<SkillPencil> skill;
  skill->Init(player);
  this_turn_get_skills_.push_back(skill);
}

//スキルの獲得
void SkillManager::AddSkill(std::shared_ptr<Skill> skill) {
  skills_.push_back(skill);
  select_ui_.AddSkill();
}

// void SkillManager::AddSkill() { select_ui_.AddSkill(); }

//更新
void SkillManager::Update() {
  for (auto&& skill : skills_) {
    skill->Update();
  }

  RemoveSkill();
}

//プレイヤー行動後の処理
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
  for (auto&& skill : skills_) {
    if (!skill->GetUseFlag()) continue;
    if (skill->GetEndTiming() != SkillEffectEndTiming::PLAYER_TURN_END)
      continue;
    skill->EndAction();
  }

  //プレイヤー行動後に発動するスキルの発動
  for (auto&& skill : skills_) {
    if (!skill->GetUseFlag()) continue;
    if (skill->GetActivetionTiming() != SkillActivationTiming::PLAYER_TURN_END)
      continue;
    skill->Action();
  }

  //今はない
}

//エネミー行動後の処理
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

//描画
void SkillManager::Draw() {
  for (auto&& skill : skills_) {
    skill->Draw();
  }
  select_ui_.Draw();
}

//演出中か
bool SkillManager::IsProductionNow() {
  for (auto&& skill : skills_) {
    if (skill->ProductionFlag()) return true;
  }
  return false;
}

//スキルリストの取得
std::vector<std::shared_ptr<Skill>> SkillManager::GetSkillList() {
  return skills_;
}

//スキルの削除
void SkillManager::RemoveSkill() {
  for (i32 i = 0; i < skills_.size(); i++) {
    if (skills_[i]->GetRemainingUsableCount() >= 1) continue;

    skills_.erase(skills_.begin() + i);
    select_ui_.RemoveSkillUI(i);
    i--;
  }
}

}  // namespace skill
}  // namespace legend