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
void Skill::AdjustPosition(math::Vector3 adjust_position) {
  transform_.SetPosition(player_->GetPosition() + adjust_position);
  transform_.SetRotation(player_->GetRotation());
  box_->SetTransform(transform_);
}
// i32 Skill::GetModelID() { return model_id_; }

i32 Skill::GetRemainingUsableCount() { return remaining_usable_count_; }

i32 Skill::GetRemainingRecastTurn() { return remaining_recast_turn_; }

SkillActivationTiming Skill::GetActivetionTiming() {
  return activetion_timing_;
}

SkillEffectEndTiming Skill::GetEndTiming() { return end_timing_; }

std::shared_ptr<directx::buffer::Texture2D> Skill::GetIconTexture() const {
  return skill_icon_texture_;
}

// i32 Skill::GetIconTexture() { return i32(); }

// i32 Skill::GetExplanationTexture() { return i32(); }
bool Skill::GetUseFlag() { return is_use_; }

bool Skill::ProductionFlag() { return is_production_; }

void Skill::RemoveCollider() { mediator_->RemoveCollider(box_); }

bool Skill::EndSkillProduction() const { return (is_use_ && !is_production_); }

}  // namespace skill
}  // namespace legend