#include "skill_pencil.h"

#include "src/enemy/enemy.h"
#include "src/game/game_device.h"
#include "src/player/player.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace skill {

SkillPencil::SkillPencil() {
  //各ステータスの初期値を設定
}

SkillPencil::~SkillPencil() {}

bool SkillPencil::Init(actor::IActorMediator* mediator,
                       player::Player* player) {
  if (!Parent::Init(mediator, player)) {
    return false;
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

  bullet::BoundingBox::InitializeParameter params;
  params.position = transform_.GetPosition();
  params.rotation = transform_.GetRotation();
  params.scale = math::Vector3(1.0f, 1.0f, 1.0f);
  params.mass = 0.0f;

  box_ = std::make_unique<bullet::BoundingBox>(this, params);
  box_->SetCollisionCallBack([&](bullet::Collider* other) { OnHit(other); });
  mediator->AddCollider(box_);

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();
  //モデルの初期化
  model_ = resource.GetModel().Get(
      util::resource::resource_names::model::STATIONERY_01);

  return true;
}

bool SkillPencil::Update() {
  if (player_ == nullptr) {
    return false;
  }

  if (is_production_) {
    ProductionUpdate();
    return true;
  }

  transform_.SetPosition(player_->GetPosition());
  transform_.SetRotation(player_->GetRotation());
  // transform_.SetScale(math::Vector3(1, 1, 1));
  // collision_.SetPosition(transform_.GetPosition());
  // collision_.SetRotation(transform_.GetRotation());
  // collision_.SetScale(transform_.GetScale());
  // collision_.SetLength(transform_.GetScale());

  return true;
}

void SkillPencil::Draw() { actor::Actor::Draw(); }

void SkillPencil::Use() {
  is_use_ = true;
  Action();
  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();
  audio.Start(util::resource::resource_names::audio::SKILL_PENCIL_SHOT, 1.0f);
}

void SkillPencil::Action() { is_production_ = true; }

void SkillPencil::ProductionUpdate() {
  float update_time =
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();
  math::Vector3 forward =
      transform_.GetRotation() * math::Vector3::kForwardVector;
  math::Vector3 velocity =
      forward + (math::Vector3(0, -20.0f, 0) * update_time);
  math::Vector3 position = transform_.GetPosition() + velocity;

  transform_.SetPosition(position);
}

void SkillPencil::EndAction() {
  remaining_usable_count_--;
  is_production_ = false;
}

void SkillPencil::OnHit(bullet::Collider* other) {
  if (dynamic_cast<enemy::Enemy*>(other->GetOwner())) {
    Explosion();
  }
}

void SkillPencil::Explosion() {
  //周囲の敵を吹き飛ばす処理

  //パーティクルの再生?

  explosion_timer_.Init(0.5f, [&]() { EndAction(); });
}

}  // namespace skill
}  // namespace legend
