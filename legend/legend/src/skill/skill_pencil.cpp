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

  SetName(skill_name::SKILLPENCIL);
  transform_.SetPosition(player->GetPosition() + math::Vector3::kUpVector);
  transform_.SetRotation(player->GetRotation());
  transform_.SetScale(math::Vector3(1, 1, 1));

  bullet::BoundingBox::InitializeParameter params;
  params.position = transform_.GetPosition();
  params.rotation = transform_.GetRotation();
  params.scale = math::Vector3(0.5f, 0.5f, 5.0f);
  params.mass = 0.0f;

  box_ = std::make_shared<bullet::BoundingBox>(this, params);
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

  //スキル説明のテクスチャ設定
  skill_explanation_texture_ =
      resource.GetTexture().Get(resource_name::texture::UI_SKILL_EXPLANATION_1);

  //装備用テクスチャ設定
  equipment_texture_ =
      resource.GetTexture().Get(resource_name::texture::UI_PLAYERFORM_SKILL_1);

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

  return true;
}

//描画
void SkillPencil::Draw() {
  if (!is_explosion_) actor::Actor::Draw();
}

//スキルの使用
void SkillPencil::Use() {
  is_use_ = true;
  Action();
  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();
  audio.Start(util::resource::resource_names::audio::SKILL_PENCIL_SHOT, 1.0f);
}

//発動
void SkillPencil::Action() {
  is_production_ = true;
  //スキルを使用したタイミングで射出方向を取得する
  shoot_theta_ =
      mediator_->GetMainCameraThetaAngle() + math::util::DEG_2_RAD * 90.0f;
  mediator_->PlayerSkillActivate();
  math::Vector3 position =
      transform_.GetPosition() + math::Vector3::kUpVector * 2.0f;
  transform_.SetPosition(position);

  //一度コライダーを削除して、新たに設定し追加する
  mediator_->RemoveCollider(box_);
  bullet::BoundingBox::InitializeParameter params;
  params.position = transform_.GetPosition();
  params.rotation = transform_.GetRotation();
  params.scale = math::Vector3(0.5f, 0.5f, 5.0f);
  params.mass = 0.1f;

  box_ = std::make_shared<bullet::BoundingBox>(this, params);
  box_->SetCollisionCallBack([&](bullet::Collider* other) { OnHit(other); });
  box_->SetFlags(box_->GetFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
  mediator_->AddCollider(box_);
  math::Vector3 velocity = math::Matrix4x4::MultiplyCoord(
      math::Vector3::kForwardVector + math::Vector3::kUpVector,
      math::Matrix4x4::CreateRotationY(-shoot_theta_));
  //コライダーを正面上方向に弾く
  box_->ApplyCentralImpulse(velocity);
}

//演出の更新
void SkillPencil::ProductionUpdate() {
  math::Vector3 target = box_->GetVelocity() - transform_.GetPosition();
  transform_.SetRotation(LookAt(target));
  box_->SetTransform(transform_);

  if (transform_.GetPosition().y <= -2.0f) EndAction();
}

//終了
void SkillPencil::EndAction() {
  remaining_usable_count_--;
  is_production_ = false;
  mediator_->PlayerSkillDeactivate();
  if (is_explosion_) explosion_pencil_->Destroy(mediator_);
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
    //衝突点から爆発させる
    util::Transform transform = transform_;
    transform.SetPosition(box_->GetHitPositions().at(other));
    Explosion(transform);
  }
}

//爆発開始
void SkillPencil::Explosion(util::Transform transform) {
  if (is_explosion_) return;
  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();

  explosion_timer_.Init(2.0f, [&]() { EndAction(); });

  //周囲の敵を吹き飛ばす処理
  is_explosion_ = true;
  audio.Start(resource_name::audio::SKILL_PENCIL_HIT, 1.0f);

  explosion_pencil_ = std::make_shared<ExplosionPencil>();
  explosion_pencil_->Init(transform, mediator_);
}

//爆発更新
void SkillPencil::ExplosionUpdate() {
  //爆発中は更新
  if (!explosion_timer_.Update()) explosion_pencil_->Update();
}

math::Quaternion SkillPencil::LookAt(const math::Vector3& target) {
  const float dot =
      math::Vector3::Dot(math::Vector3::kForwardVector, target.Normalized());
  const float theta = math::util::Acos(dot);
  const math::Vector3 cross =
      math::Vector3::Cross(math::Vector3::kForwardVector, target).Normalized();

  return math::Quaternion(cross, theta);
}

}  // namespace skill
}  // namespace legend
