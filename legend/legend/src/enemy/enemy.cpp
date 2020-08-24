#include "src/enemy/enemy.h"

#include "src/bullet/bullet_helper.h"
#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/enemy/boss.h"
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
Enemy::Enemy() : Parent(L"Enemy") /*, velocity_(math::Vector3::kZeroVector)*/ {
  is_move_ = false;
  // deceleration_x_ = deceleration_z_ = 0;
}

//デストラクタ
Enemy::~Enemy() {}

//初期化
bool Enemy::Init(actor::IActorMediator* mediator,
                 const InitializeParameter& parameter) {
  if (!Parent::Init(mediator)) {
    return false;
  }

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  this->transform_ = parameter.transform;

  bullet::BoundingBox::InitializeParameter params;
  params.position = this->transform_.GetPosition();
  params.rotation = this->transform_.GetRotation();
  params.scale = parameter.bouding_box_length;
  params.mass = 1.0f;
  params.friction = 0.6f;
  params.restitution = 0.6f;
  box_ = std::make_shared<bullet::BoundingBox>(this, params);
  box_->SetCollisionCallBack([&](bullet::Collider* other) { OnHit(other); });
  mediator_->AddCollider(box_);
  strength_ = 1.0f;

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  model_ =
      resource.GetModel().Get(util::resource::resource_names::model::ENEMY);

  move_end_ = false;

  return true;
}

void Enemy::Remove() { mediator_->RemoveCollider(box_); }

//更新
bool Enemy::Update() {
  // obb_.Update();

  update_time_ =
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();

  auto velocity = GetVelocity();
  velocity.y = 0;
  const bool is_nearly_zero_vector = velocity.MagnitudeSquared() < 0.01f;
  if (is_move_ && is_nearly_zero_vector) move_end_ = true;
  is_move_ = (0.01f <= velocity.Magnitude());
  // Move();

  // transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  // transform_cb_.UpdateStaging();

  return true;
}

void Enemy::Draw() {
  MY_ASSERTION(model_.get(), L"モデルが存在しません。");
  if (GetPosition().y <= -20) return;

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
  resource.GetPipeline()
      .Get(util::resource::resource_names::pipeline::MODEL_VIEW)
      ->SetCommandList(command_list);

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();
  transform_cb_.RegisterHandle(
      device, directx::shader::ConstantBufferRegisterID::TRANSFORM);

  model_->Draw(command_list);
}

//移動
void Enemy::Move() {
  // if (!is_move_) return;
  // auto velocity = GetVelocity();
  // velocity.y = 0;
  ////移動距離を求める
  // float length =
  //    math::util::Sqrt(velocity.x * velocity.x + velocity.z * velocity.z);

  ////実際に動く距離
  // float x = -velocity.x / length;
  // float z = -velocity.z / length;

  //////減速計算
  //// deceleration_x_ = x / (length * length);
  //// deceleration_z_ = z / (length * length);

  ////移動処理
  // math::Vector3 v = math::Vector3(x, 0, z);
  // math::Vector3 position = GetPosition() + v * power_ * update_time_;
  // SetPosition(position);

  //// Deceleration(2);
}

void Enemy::SetPosition(math::Vector3 position) {
  transform_.SetPosition(position);
  // collision_.SetPosition(position);
}

//速度の設定
void Enemy::SetVelocity(math::Vector3 velocity) {
  box_->ApplyCentralImpulse(velocity);
}

void Enemy::SetRotation() {
  math::Quaternion rotation = transform_.GetRotation();
  rotation.y += GetVelocity().x;
  transform_.SetRotation(rotation);
}

//移動に必要なパラメータを初期化
void Enemy::ResetParameter() {
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
math::Vector3 Enemy::GetPosition() const { return transform_.GetPosition(); }

//移動量の取得
math::Vector3 Enemy::GetVelocity() const { return (box_->GetVelocity()); }

math::Quaternion Enemy::GetRotation() const { return transform_.GetRotation(); }

float Enemy::GetPower() const { return power_; }

bool Enemy::GetMoveEnd() const { return move_end_; }

void Enemy::ResetMoveEnd() { move_end_ = false; }

void Enemy::OnHit(bullet::Collider* other) {
  system::Mode turn_mode = mediator_->GetCurrentTurn();
  if (turn_mode == system::Mode::ENEMY_MOVING) {
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
  }
  //ボスに触れた
  {
    enemy::Boss* b = dynamic_cast<enemy::Boss*>(other->GetOwner());
    if (b) {
      const math::Vector3 enemy_position = transform_.GetPosition();
      const math::Vector3 boss_position = b->GetTransform().GetPosition();
      const math::Vector3 direction =
          (boss_position - enemy_position).Normalized();

      b->GetCollider()->ApplyCentralImpulse(direction * power_);
    }
  }
}

void Enemy::Weaking(const float& weak) {
  strength_ -= weak;
  if (strength_ <= min_strength_) strength_ = min_strength_;
}

}  // namespace enemy
}  // namespace legend