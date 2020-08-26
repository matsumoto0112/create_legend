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

  model_ =
      resource.GetModel().Get(util::resource::resource_names::model::BOSS);

  move_end_ = false;

  return true;
}

void Boss::Remove() { mediator_->RemoveCollider(box_); }

//更新
bool Boss::Update() {
  // obb_.Update();

  update_time_ =
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();

  const bool is_nearly_zero_vector = GetVelocity().MagnitudeSquared() < 0.01f;
  if (is_move_ && is_nearly_zero_vector) move_end_ = true;
  is_move_ = (0.01f < GetVelocity().Magnitude());
  // Move();

  // transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  // transform_cb_.UpdateStaging();

  return true;
}

//移動
void Boss::Move() {
  if (!is_move_) return;
  auto velocity = GetVelocity();

  //移動距離を求める
  float length =
      math::util::Sqrt(velocity.x * velocity.x + velocity.z * velocity.z);

  //実際に動く距離
  float x = -velocity.x / length;
  float z = -velocity.z / length;

  ////減速計算
  // deceleration_x_ = x / (length * length);
  // deceleration_z_ = z / (length * length);

  //移動処理
  math::Vector3 v = math::Vector3(x, 0, z);
  math::Vector3 position = GetPosition() + v * power_ * update_time_;
  SetPosition(position);

  // Deceleration(2);
}

void Boss::SetPosition(math::Vector3 position) {
  transform_.SetPosition(position);
  // collision_.SetPosition(position);
}

//速度の設定
void Boss::SetVelocity(math::Vector3 velocity) {
  box_->ApplyCentralImpulse(velocity);
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
}

////減速
// void Enemy::Deceleration(float deceleration_rate) {
//  float x = deceleration_x_ * deceleration_rate * update_time_;
//  float z = deceleration_z_ * deceleration_rate * update_time_;
//
//  if ((x <= velocity_.x && velocity_.x <= 0) ||
//      (0 <= velocity_.x && velocity_.x <= x)) {
//    velocity_.x = 0;
//  } else {
//    velocity_.x -= x;
//  }
//  if ((z <= velocity_.z && velocity_.z <= 0) ||
//      (0 <= velocity_.z && velocity_.z <= z)) {
//    velocity_.z = 0;
//  } else {
//    velocity_.z -= z;
//  }
//}

//座標の取得
math::Vector3 Boss::GetPosition() const { return transform_.GetPosition(); }

//移動量の取得
math::Vector3 Boss::GetVelocity() const { return box_->GetVelocity(); }

math::Quaternion Boss::GetRotation() const { return transform_.GetRotation(); }

float Boss::GetPower() const { return power_; }

bool Boss::GetMoveEnd() const { return move_end_; }

void Boss::ResetMoveEnd() { move_end_ = false; }

void Boss::OnHit(bullet::Collider* other) {
  //プレイヤーに触れた
  {
    player::Player* p = dynamic_cast<player::Player*>(other->GetOwner());
    if (p) {
      const math::Vector3 enemy_position = transform_.GetPosition();
      const math::Vector3 player_position = p->GetTransform().GetPosition();
      const math::Vector3 direction =
          (player_position - enemy_position).Normalized();

      p->GetCollider()->ApplyCentralImpulse(direction * power_);
    }
  }
  //敵に触れた
  {
    enemy::Enemy* e = dynamic_cast<enemy::Enemy*>(other->GetOwner());
    if (e) {
      const math::Vector3 boss_position = transform_.GetPosition();
      const math::Vector3 enemy_position = e->GetTransform().GetPosition();
      const math::Vector3 direction =
          (enemy_position - boss_position).Normalized();

      e->GetCollider()->ApplyCentralImpulse(direction * power_);
    }
  }
}

}  // namespace enemy
}  // namespace legend