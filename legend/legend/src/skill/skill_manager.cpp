#include "src/skill/skill_manager.h"

#include "src/game/game_device.h"
#include "src/player/player.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace skill {

//コンストラクタ
SkillManager::SkillManager() {}

//デストラクタ
SkillManager::~SkillManager() {}

//初期化
void SkillManager::Init(actor::IActorMediator* mediator,
                        player::Player* player) {
  this->mediator_ = mediator;
  player_ = player;
  select_ui_.Init();
  select_move_ = false;
  player_ui_.Init();
}

//スキル取得時
void SkillManager::GetSkill(i32 skill_id) {
  std::shared_ptr<SkillPencil> skill;
  skill->Init(mediator_, player_);
  this_turn_get_skills_.push_back(skill);
}

//スキルの獲得
void SkillManager::AddSkill(std::shared_ptr<Skill> skill) {
  skills_.push_back(skill);
  select_ui_.AddSkill(skill.get());
  player_ui_.AddEquipmentUI(skill.get());
}

// void SkillManager::AddSkill() { select_ui_.AddSkill(); }

//更新
void SkillManager::Update() {
  i32 i = 0;
  for (auto&& skill : skills_) {
    skill->Update();
    if (skill->ProductionFlag()) {
      i++;
      continue;
    }

    SetPosition(skill, i);
    i++;
  }

  player_ui_.Update();
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
    if (!skill->EndSkillProduction()) continue;
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
  for (auto&& skill : skills_) {
    if (!skill->EndSkillProduction()) continue;
    if (skill->GetEndTiming() != SkillEffectEndTiming::ENEMY_TURN_END) continue;
    skill->EndAction();
  }

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
  player_ui_.Draw();
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

    skills_[i]->RemoveCollider();
    skills_.erase(skills_.begin() + i);
    select_ui_.RemoveSkillUI(i);
    player_ui_.RemoveEquipmentUI(i);
    i--;
    select_ui_.SelectSkillNumber(0);
  }
}

//スキルの選択
bool SkillManager::SelectSkill() {
  if (IsProductionNow()) return true;

  auto& input = game::GameDevice::GetInstance()->GetInput();
  if (input.GetGamepad()->GetButtonDown(input::joy_code::LB)) {
    select_ui_.ChangeIsSelectMode();
  }

  if (select_ui_.GetIsSelectMode()) {
    i32 number = select_ui_.GetSkillNumber();
    //移動判定を1回だけにする
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

//スキルの使用
void SkillManager::UseSkill() {
  if (!select_ui_.GetIsSelectMode() || IsProductionNow()) return;

  auto& input = game::GameDevice::GetInstance()->GetInput();
  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();
  i32 skill_num = select_ui_.GetSkillNumber();
  if (input.GetGamepad()->GetButtonDown(input::joy_code::A)) {
    skills_[skill_num]->Use();
    audio.Start(util::resource::resource_names::audio::SKILL_DECISION, 1.0f);
    if (skills_[skill_num]->GetActivetionTiming() !=
        SkillActivationTiming::NOW) {
      select_ui_.ChangeIsSelectMode();
    }
  }
}

//スキル終了
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
    pos = math::Vector3::kUpVector + math::Vector3::kRightVector;
  else
    pos = math::Vector3::kUpVector + math::Vector3::kLeftVector;
  pos = math::Matrix4x4::MultiplyCoord(
      pos, player_->GetTransform().GetRotation().ToMatrix());
  skill->AdjustPosition(pos);
}

}  // namespace skill
}  // namespace legend