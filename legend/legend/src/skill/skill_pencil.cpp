#include "skill_pencil.h"

#include "src/game/game_device.h"
#include "src/player/player.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace skill {

SkillPencil::SkillPencil() {
  //各ステータスの初期値を設定
}

SkillPencil::~SkillPencil() {}

void SkillPencil::Init(player::Player* player) {
  if (!Parent::InitBuffer()) {
    return;
  }
  //! 規定使用可能回数
  usable_count_ = 1;
  //! 残り使用可能回数
  remaining_usable_count_ = usable_count_;
  //! 再使用まで規定のターン数
  recast_turn_ = 1;
  //! 残り再使用までのターン数
  remaining_recast_turn_ = 0;
  //! スキルの発動タイミング
  activetion_timing_ = SkillActivationTiming::NOW;
  //! スキルの効果終了タイミング
  end_timing_ = SkillEffectEndTiming::NOW;
  //! 使用されているかのフラグ
  is_use_ = false;
  is_production_ = false;

  player_ = player;

  transform_.SetPosition(player->GetPosition());
  transform_.SetRotation(player->GetRotation());
  transform_.SetScale(math::Vector3(1, 1, 1));

  collision_.SetPosition(transform_.GetPosition());
  collision_.SetRotation(transform_.GetRotation());
  collision_.SetScale(transform_.GetScale());
  collision_.SetLength(transform_.GetScale());
  collision_.SetIsTrigger(true);

  collision_.SetTriggerCallback(
      [&](actor::ActorType type) { Explosion(type); });

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();
  //モデルの初期化
  model_ = resource.GetModel().Get(
      util::resource::resource_names::model::STATIONERY_01);
}

bool SkillPencil::Update() {
  if (player_ == nullptr) {
    return false;
  }

  transform_.SetPosition(player_->GetPosition());
  transform_.SetRotation(player_->GetRotation());
  // transform_.SetScale(math::Vector3(1, 1, 1));
  collision_.SetPosition(transform_.GetPosition());
  collision_.SetRotation(transform_.GetRotation());
  // collision_.SetScale(transform_.GetScale());
  // collision_.SetLength(transform_.GetScale());

  return true;
}

void SkillPencil::Draw() { actor::Actor::Draw(); }

void SkillPencil::Use() {
  is_use_ = true;
  Action();
}

void SkillPencil::Action() { is_production_ = true; }

void SkillPencil::ProductionUpdate() {}

void SkillPencil::EndAction() {
  remaining_usable_count_--;
  is_production_ = false;
}

void SkillPencil::Explosion(actor::ActorType type) {
  if (type != actor::ActorType::ENEMY && type != actor::ActorType::BOSS &&
      type != actor::ActorType::DESK)
    return;

  //周囲の敵を吹き飛ばす処理

  //パーティクルの再生?

  explosion_timer_.Init(0.5f, [&]() { EndAction(); });
}

}  // namespace skill
}  // namespace legend
