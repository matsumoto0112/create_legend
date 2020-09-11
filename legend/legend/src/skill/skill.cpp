#include "src/skill/skill.h"

#include "src/player/player.h"

namespace legend {
namespace skill {
//コンストラクタ
Skill::Skill() : Parent(L"Skill") {}

//デストラクタ
Skill::~Skill() {}

//初期化
bool Skill::Init(actor::IActorMediator* mediator, player::Player* player) {
  if (!Actor::Init(mediator)) {
    return false;
  }
  return true;
}

//更新
bool Skill::Update() { return true; }

//使用
void Skill::Use() { is_use_ = true; }

//発動
void Skill::Action() {}

//スキル演出更新
void Skill::ProductionUpdate() { is_production_ = false; }

//スキル終了
void Skill::EndAction() {}

//再使用ターンの更新
void Skill::RemaingRecastTurnUpdate() {
  //カウントを更新
  remaining_recast_turn_--;
}

//装備スキルの座標調整
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

//選択中のスキルの位置を変更
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

//残り使用回数の取得
i32 Skill::GetRemainingUsableCount() { return remaining_usable_count_; }

//リキャストターンの取得
i32 Skill::GetRemainingRecastTurn() { return remaining_recast_turn_; }

//スキル発動タイミング
SkillActivationTiming Skill::GetActivetionTiming() {
  return activetion_timing_;
}

//スキル終了タイミングの取得
SkillEffectEndTiming Skill::GetEndTiming() { return end_timing_; }

//スキルアイコンの取得
std::shared_ptr<directx::buffer::Texture2D> Skill::GetIconTexture() const {
  return skill_icon_texture_;
}

//スキル使用中の画像の取得
std::shared_ptr<directx::buffer::Texture2D> Skill::GetIconUsingTexture() const {
  return skill_icon_using_texture_;
}

//スキル装備画像の取得
std::shared_ptr<directx::buffer::Texture2D> Skill::GetEquipmentTexture() const {
  return equipment_texture_;
}

//スキル説明画像の取得
std::shared_ptr<directx::buffer::Texture2D> Skill::GetExplanationTexture()
    const {
  return skill_explanation_texture_;
}

//使用フラグの取得
bool Skill::GetUseFlag() { return is_use_; }

//演出中フラグの取得
bool Skill::ProductionFlag() { return is_production_; }

//コライダーの削除
void Skill::RemoveCollider() { mediator_->RemoveCollider(box_); }

//演出終わり判定の取得
bool Skill::EndSkillProduction() const { return (is_use_ && !is_production_); }

//スキル名の設定
void Skill::SetName(const std::wstring name) { Parent::name_ = name; }

//スキル名の取得
std::wstring Skill::GetName() const { return Parent::name_; }

}  // namespace skill
}  // namespace legend