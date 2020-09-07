#include "src/skill/skill.h"

#include "src/player/player.h"

namespace legend {
namespace skill {
Skill::Skill() : Parent(L"Skill") {}
Skill::~Skill() {}
bool Skill::Init(actor::IActorMediator* mediator, player::Player* player) {
  if (!Actor::Init(mediator)) {
    return false;
  }
  return true;
}
bool Skill::Update() { return true; }
// void Skill::Draw() {}
void Skill::Use() { is_use_ = true; }
void Skill::Action() {}
void Skill::ProductionUpdate() { is_production_ = false; }
void Skill::EndAction() {}
void Skill::RemaingRecastTurnUpdate() {
  //カウントを更新
  remaining_recast_turn_--;
}
void Skill::AdjustPosition(math::Vector3 position) {
  transform_.SetPosition(player_->GetPosition() + position);
  transform_.SetRotation(player_->GetRotation());
  box_->SetTransform(transform_);
}
// i32 Skill::GetModelID() { return model_id_; }

void Skill::ChangePosition(math::Vector3 position) {
  transform_.SetPosition(position);
  transform_.SetRotation(player_->GetRotation());
  box_->SetTransform(transform_);
}

i32 Skill::GetRemainingUsableCount() { return remaining_usable_count_; }

i32 Skill::GetRemainingRecastTurn() { return remaining_recast_turn_; }

SkillActivationTiming Skill::GetActivetionTiming() {
  return activetion_timing_;
}

SkillEffectEndTiming Skill::GetEndTiming() { return end_timing_; }

std::shared_ptr<directx::buffer::Texture2D> Skill::GetIconTexture() const {
  return skill_icon_texture_;
}

std::shared_ptr<directx::buffer::Texture2D> Skill::GetIconUsingTexture() const {
  return skill_icon_using_texture_;
}

std::shared_ptr<directx::buffer::Texture2D> Skill::GetEquipmentTexture() const {
  return equipment_texture_;
}

std::shared_ptr<directx::buffer::Texture2D> Skill::GetExplanationTexture()
    const {
  return skill_explanation_texture_;
}

// i32 Skill::GetIconTexture() { return i32(); }

// i32 Skill::GetExplanationTexture() { return i32(); }
bool Skill::GetUseFlag() { return is_use_; }

bool Skill::ProductionFlag() { return is_production_; }

void Skill::RemoveCollider() { mediator_->RemoveCollider(box_); }

//演出終わり判定の取得
bool Skill::EndSkillProduction() const { return (is_use_ && !is_production_); }

//スキル名の設定
void Skill::SetName(const std::wstring name) { Parent::name_ = name; }

//スキル名の取得
std::wstring Skill::GetName() const { return Parent::name_; }

}  // namespace skill
}  // namespace legend