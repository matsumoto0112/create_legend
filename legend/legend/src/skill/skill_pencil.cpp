#include "src/skill/skill_pencil.h"

#include "src/enemy/boss.h"
#include "src/enemy/enemy.h"
#include "src/game/game_device.h"
#include "src/object/desk.h"
#include "src/object/obstacle.h"
#include "src/player/player.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace skill {
namespace resource_name = util::resource::resource_names;

//コンストラクタ
SkillPencil::SkillPencil() {
  //各ステータスの初期値を設定
}

//デストラクタ
SkillPencil::~SkillPencil() {}

//初期化
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
  is_explosion_ = false;

  player_ = player;

  transform_.SetPosition(player->GetPosition() + math::Vector3::kUpVector);
  transform_.SetRotation(player->GetRotation());
  transform_.SetScale(math::Vector3(1, 1, 1));

  bullet::BoundingBox::InitializeParameter params;
  params.position = transform_.GetPosition();
  params.rotation = transform_.GetRotation();
  params.scale = math::Vector3(0.5f, 0.5f, 5.0f);
  params.mass = 0.0f;

  box_ = std::make_shared<bullet::BoundingBox>(this, params);
  box_->SetCollisionCallBack([&](bullet::Collider* other) { OnHit(other); });
  box_->SetFlags(box_->GetFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
  mediator_->AddCollider(box_);

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();
  //モデルの初期化
  model_ = resource.GetModel().Get(resource_name::model::STATIONERY_01);

  //スキルアイコンのテクスチャ設定
  skill_icon_texture_ =
      resource.GetTexture().Get(resource_name::texture::UI_SKILL_ICON_1);

  return true;
}

//更新
bool SkillPencil::Update() {
  if (player_ == nullptr) {
    return false;
  }

  if (is_explosion_) {
    ExplosionUpdate();
    return true;
  }

  if (is_production_) {
    ProductionUpdate();
    return true;
  }

  //transform_.SetPosition(player_->GetPosition() + math::Vector3::kUpVector);
  //transform_.SetRotation(player_->GetRotation());
  //box_->SetTransform(this->transform_);

  return true;
}

//描画
void SkillPencil::Draw() {
  if (!is_explosion_)
    actor::Actor::Draw();
}

//スキルの使用
void SkillPencil::Use() {
  is_use_ = true;
  Action();
  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();
  audio.Start(util::resource::resource_names::audio::SKILL_PENCIL_SHOT, 1.0f);
}

//発動
void SkillPencil::Action() { is_production_ = true; }

//演出の更新
void SkillPencil::ProductionUpdate() {
  float update_time =
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();
  //メインカメラの回転角を取得する
  //角度はXY平面上で(1,0)の方角から0としているため、90°回転する必要がある
  float theta =
      mediator_->GetMainCameraThetaAngle() + math::util::DEG_2_RAD * 90.0f;
  //移動する方向ベクトルにカメラの向きに応じた回転をかけることで、カメラの向いている方向に対した入力値に変換する
  math::Vector3 velocity = math::Matrix4x4::MultiplyCoord(
      math::Vector3::kForwardVector, math::Matrix4x4::CreateRotationY(-theta));
  velocity = velocity.Normalized();
  velocity = velocity + math::Vector3(0, -9.8f, 0) * update_time;
  math::Vector3 position = transform_.GetPosition() + velocity;

  transform_.SetPosition(position);
  box_->SetTransform(transform_);

  if (transform_.GetPosition().y <= -2.0f) EndAction();
}

//終了
void SkillPencil::EndAction() {
  remaining_usable_count_--;
  is_production_ = false;
  if (is_explosion_) explosion_pencil_.Destroy(mediator_);
  if (remaining_usable_count_ <= 0) mediator_->RemoveActor(this);
}

//衝突判定
void SkillPencil::OnHit(bullet::Collider* other) {
  if (!is_production_) return;

  //雑魚敵、ボス、机、障害物に当たったら爆発
  if (dynamic_cast<enemy::Enemy*>(other->GetOwner()) ||
      dynamic_cast<enemy::Boss*>(other->GetOwner()) ||
      dynamic_cast<object::Desk*>(other->GetOwner()) ||
      dynamic_cast<object::Obstacle*>(other->GetOwner())) {
    Explosion();
  }
}

//爆発開始
void SkillPencil::Explosion() {
  if (is_explosion_) return;
  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();

  explosion_timer_.Init(2.0f, [&]() { EndAction(); });

  //周囲の敵を吹き飛ばす処理
  is_explosion_ = true;
  audio.Start(resource_name::audio::SKILL_PENCIL_HIT, 1.0f);

  explosion_pencil_.Init(transform_, mediator_);

  //パーティクルの再生?
}

//爆発更新
void SkillPencil::ExplosionUpdate() {
  //爆発中は更新
  if (!explosion_timer_.Update()) explosion_pencil_.Update();
}

}  // namespace skill
}  // namespace legend
