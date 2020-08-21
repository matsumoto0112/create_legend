#include "src/skill/explosion_pencil.h"

#include "src/enemy/boss.h"
#include "src/enemy/enemy.h"
#include "src/game/game_device.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace skill {
//コンストラクタ
ExplosionPencil::ExplosionPencil() : Parent(L"ExplosionPencil") {}

//デストラクタ
ExplosionPencil::~ExplosionPencil() {}

//初期化
void ExplosionPencil::Init(util::Transform transform,
                           actor::IActorMediator* mediator) {
  if (!Parent::Init(mediator)) {
    return;
  }

  transform_.SetPosition(transform.GetPosition());
  transform_.SetRotation(transform.GetRotation());
  transform_.SetScale(transform.GetScale());
  explosion_power_ = 1.0f;
  radius_ = 1.0f;

  bullet::BoundingSphere::InitializeParameter parameter;
  parameter.position = transform_.GetPosition();
  parameter.radius = radius_;
  parameter.mass = 0.0f;
  sphere_ = std::make_shared<bullet::BoundingSphere>(this, parameter);
  sphere_->SetCollisionCallBack([&](bullet::Collider* other) { OnHit(other); });
  sphere_->SetFlags(sphere_->GetFlags() |
                    btCollisionObject::CF_NO_CONTACT_RESPONSE);
  mediator->AddCollider(sphere_);

  auto& resource = game::GameDevice::GetInstance()->GetResource();

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();
  //モデルの初期化
  //モデルは適当に入ってる
  model_ = resource.GetModel().Get(
      util::resource::resource_names::model::STATIONERY_01);
}

//更新
bool ExplosionPencil::Update() {
  radius_ +=
      1.0f *
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();
  return true;
}

//描画
void ExplosionPencil::Draw() { actor::Actor::Draw(); }

//衝突判定
void ExplosionPencil::OnHit(bullet::Collider* other) {
  {
    enemy::Enemy* enemy = dynamic_cast<enemy::Enemy*>(other->GetOwner());
    if (enemy) {
      math::Vector3 explosion_position = transform_.GetPosition();
      math::Vector3 enemy_position = enemy->GetTransform().GetPosition();
      math::Vector3 direction =
          (enemy_position - explosion_position).Normalized();

      enemy->GetCollider()->ApplyCentralImpulse(direction * explosion_power_);
    }
  }
  {
    enemy::Boss* boss = dynamic_cast<enemy::Boss*>(other->GetOwner());
    if (boss) {
      math::Vector3 explosion_position = transform_.GetPosition();
      math::Vector3 boss_position = boss->GetTransform().GetPosition();
      math::Vector3 direction =
          (boss_position - explosion_position).Normalized();

      boss->GetCollider()->ApplyCentralImpulse(direction * explosion_power_);
    }
  }
}

//削除
void ExplosionPencil::Destroy(actor::IActorMediator* mediator) {
  mediator->RemoveCollider(sphere_);
  mediator->RemoveActor(this);
}
}  // namespace skill
}  // namespace legend