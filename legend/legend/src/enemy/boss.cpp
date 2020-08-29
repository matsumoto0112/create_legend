#include "src/enemy/boss.h"

#include "src/bullet/bullet_helper.h"
#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/enemy/enemy.h"
#include "src/game/game_device.h"
#include "src/object/graffiti.h"
#include "src/player/player.h"
#include "src/util/path.h"
#include "src/util/resource/pixel_shader.h"
#include "src/util/resource/resource_names.h"
#include "src/util/resource/vertex_shader.h"

namespace legend {
namespace enemy {
//コンストラクタ
Boss::Boss() : Parent(L"Boss") {
  is_move_ = false;
  // deceleration_x_ = deceleration_z_ = 0;
}

//デストラクタ
Boss::~Boss() {}

//初期化
bool Boss::Init(actor::IActorMediator* mediator,
                const InitializeParameter& parameter) {
  if (!Parent::Init(mediator)) return false;
  // if (!obb_.Initialize(device)) {
  //  return false;
  //}
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  this->transform_ = parameter.transform;

  bullet::BoundingBox::InitializeParameter params;
  params.position = this->transform_.GetPosition();
  params.rotation = this->transform_.GetRotation();
  params.scale = parameter.bouding_box_length;
  params.mass = parameter.mass;
  params.friction = parameter.friction;
  params.restitution = parameter.restitution;
  box_ = std::make_shared<bullet::BoundingBox>(this, params);
  box_->SetCollisionCallBack([&](bullet::Collider* other) { OnHit(other); });
  mediator_->AddCollider(box_);

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  model_ = resource.GetModel().Get(util::resource::resource_names::model::BOSS);

  move_end_ = false;

  move_type_ = (enemy::enemy_type::MoveType::Straight);
  hit_type_ = (enemy::enemy_type::HitType::Rush);
  effect_type_ = (enemy::enemy_type::EffectType::Rotate);

  return true;
}

void Boss::Remove() { mediator_->RemoveCollider(box_); }

//更新
bool Boss::Update() {
  update_time_ =
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();

  auto velocity = GetVelocity();
  velocity.y = 0;
  if (is_move_ && (velocity.Magnitude() < 0.01f)) {
    move_end_ = true;
    is_move_ = false;
  }

  return true;
}

void Boss::SetPosition(math::Vector3 position) {
  transform_.SetPosition(position);
  // collision_.SetPosition(position);
}

//速度の設定
void Boss::SetVelocity(math::Vector3 velocity) {
  // 加速度の設定
  box_->ApplyCentralImpulse(velocity);
  // 回転の設定
  if (effect_type_ == enemy::enemy_type::EffectType::Rotate) {
    auto angle = math::Vector3::kUpVector * velocity.Magnitude();
    angle *= (game::GameDevice::GetInstance()->GetRandom().Range(-0.5f, 0.5f));
    box_->SetAngularVelocity(angle);
  }
  is_move_ = true;
}

void Boss::SetRotation() {
  math::Quaternion rotation = transform_.GetRotation();
  rotation.y += GetVelocity().x;
  transform_.SetRotation(rotation);
}

//移動に必要なパラメータを初期化
void Boss::ResetParameter() {
  if (GetVelocity().Magnitude() != 0.0f) return;

  // deceleration_x_ = deceleration_z_ = 0;
  is_move_ = false;
  move_end_ = false;
}

//座標の取得
math::Vector3 Boss::GetPosition() const { return transform_.GetPosition(); }

//移動量の取得
math::Vector3 Boss::GetVelocity() const { return box_->GetVelocity(); }

math::Quaternion Boss::GetRotation() const { return transform_.GetRotation(); }

float Boss::GetPower() const { return power_; }

bool Boss::GetMoveEnd() const { return (!is_move_ && move_end_); }

void Boss::ResetMoveEnd() { move_end_ = false; }

void Boss::OnHit(bullet::Collider* other) {
  system::Mode turn_mode = mediator_->GetCurrentTurn();
  if (turn_mode == system::Mode::ENEMY_MOVING) {
    //プレイヤーに触れた
    if (player::Player* p = dynamic_cast<player::Player*>(other->GetOwner())) {
      HitAction(other);
      auto s = math::util::Clamp(strength_ - p->GetStrength(), 0.0f, 1.0f);
      auto trigonometric = (std::sin(30.0f * math::util::PI * s));
      auto strength =
          math::Vector3::kUpVector * GetVelocity().Magnitude() * trigonometric;
      other->ApplyCentralImpulse(strength);
    }
    //敵に触れた
    else if (enemy::Enemy* e = dynamic_cast<enemy::Enemy*>(other->GetOwner())) {
      HitAction(other);
    }
  }
}

void Boss::HitAction(bullet::Collider* other) {
  auto actor = other->GetOwner();
  const math::Vector3 position = transform_.GetPosition();
  const math::Vector3 other_position = actor->GetTransform().GetPosition();
  const math::Vector3 velocity = GetVelocity();
  const math::Vector3 direction =
      ((other_position - position).Normalized() + velocity.Normalized())
          .Normalized();

  auto add_power = ((strength_ + velocity.Magnitude()) / 2.0f);
  switch (hit_type_) {
      // 衝突時、停止する処理
    case enemy::enemy_type::HitType::Stop:
      GetCollider()->ApplyCentralImpulse(velocity * -1.0f);
      other->ApplyCentralImpulse(direction * add_power);
      break;
      // 衝突時、突進する処理
    case enemy::enemy_type::HitType::Rush:
      other->ApplyCentralImpulse(direction * add_power);
      break;
      // 衝突時、跳弾する処理
    case enemy::enemy_type::HitType::Bound:
      GetCollider()->ApplyCentralImpulse(velocity * -2.0f);
      other->ApplyCentralImpulse(direction * add_power);
      break;
  }
}

void Boss::UpdateStrength(const float& weak) {
  strength_ += weak;
  if (strength_ <= min_strength_) strength_ = min_strength_;
}

}  // namespace enemy
}  // namespace legend