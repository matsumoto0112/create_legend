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
  explosion_power_ = 3.0f;
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
  is_destroy_ = false;

  auto& resource = game::GameDevice::GetInstance()->GetResource();

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();
}

//更新
bool ExplosionPencil::Update() {
  radius_ +=
      10.0f *
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();
  //再生成しているためか、一度格納したコライダーを取り除いて
  //格納し直さないとうまく出来なかった
  mediator_->RemoveCollider(sphere_);
  float mass = 0.0f;
  sphere_->SetScale(radius_, mass);
  mediator_->AddCollider(sphere_);
  sphere_->SetFlags(sphere_->GetFlags() |
      btCollisionObject::CF_NO_CONTACT_RESPONSE);
  return true;
}

//衝突判定
void ExplosionPencil::OnHit(bullet::Collider* other) {
  {
    enemy::Enemy* enemy = dynamic_cast<enemy::Enemy*>(other->GetOwner());
    if (enemy) {
      math::Vector3 explosion_position = transform_.GetPosition();
      math::Vector3 enemy_position = enemy->GetTransform().GetPosition();
      math::Vector3 direction =
          (enemy_position - explosion_position).Normalized();
      direction = direction * explosion_power_;
      //値は適当
      direction.y = 2.0f;

      enemy->GetCollider()->ApplyCentralImpulse(direction);
    }
  }
  {
    enemy::Boss* boss = dynamic_cast<enemy::Boss*>(other->GetOwner());
    if (boss) {
      math::Vector3 explosion_position = transform_.GetPosition();
      math::Vector3 boss_position = boss->GetTransform().GetPosition();
      math::Vector3 direction =
          (boss_position - explosion_position).Normalized();
      direction = direction * explosion_power_;
      //値は適当
      direction.y = 2.0f;

      boss->GetCollider()->ApplyCentralImpulse(direction * 0.5f);
    }
  }
}

//削除
void ExplosionPencil::Destroy(actor::IActorMediator* mediator) {
  is_destroy_ = true;
  mediator->RemoveCollider(sphere_);
  mediator->RemoveActor(this);
}
}  // namespace skill
}  // namespace legend