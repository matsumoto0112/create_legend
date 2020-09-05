#include "src/enemy/enemy_actor.h"

#include "src/bullet/bullet_helper.h"
#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/draw/particle/particle_factory.h"
#include "src/game/game_device.h"
#include "src/object/graffiti.h"
#include "src/object/obstacle.h"
#include "src/player/player.h"
#include "src/util/path.h"
#include "src/util/resource/pixel_shader.h"
#include "src/util/resource/resource_names.h"
#include "src/util/resource/vertex_shader.h"

namespace legend {
namespace enemy {
//コンストラクタ
EnemyActor::EnemyActor() : Parent(L"EnemyActor") {
  is_move_ = false;
  enemy_ai_.Init();
}

//デストラクタ
EnemyActor::~EnemyActor() {}

//初期化
bool EnemyActor::Init(actor::IActorMediator* mediator,
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
  params.mass = parameter.mass;
  params.friction = parameter.friction;
  params.restitution = parameter.restitution;
  box_ = std::make_shared<bullet::BoundingBox>(this, params);
  box_->SetCollisionCallBack([&](bullet::Collider* other) { OnHit(other); });
  mediator_->AddCollider(box_);
  strength_ = 1.0f;

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  move_end_ = false;

  enemy_move_particle_ =
      draw::particle::particle_factory::CreateEnemyMoveParticle();
  enemy_move_particle_->SetEmitEnable(false);

  return true;
}

void EnemyActor::Remove() {
  mediator_->RemoveCollider(box_);
  enemy_move_particle_->Delete();
}

//更新
bool EnemyActor::Update() {
  update_time_ =
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();

  auto velocity = GetVelocity();
  velocity.y = 0;
  auto angluar = box_->GetAngularVelocity();
  if (is_move_ && (velocity.Magnitude() < 0.01f) &&
      (angluar.Magnitude() < 0.01f)) {
    move_end_ = true;
    is_move_ = false;
  }

  auto ParticleUpdate = [&]() {
    const math::Vector3 MOVE_PARTICLE_OFFSET = GetVelocity().Normalized() * -3;
    const math::Vector3 move_particle_position =
        transform_.GetPosition() + MOVE_PARTICLE_OFFSET;
    enemy_move_particle_->GetTransformRef().SetPosition(move_particle_position);
    const math::Vector3 velocity = GetVelocity();
    const math::Vector3 velocity_xz{velocity.x, 0.0f, velocity.z};
    const bool emit_enable = is_move_ ? velocity_xz.Magnitude() > 0.6f
                                      : math::util::Abs(velocity.y) > 1.0f;
    enemy_move_particle_->SetEmitEnable(emit_enable);
  };

  ParticleUpdate();
  obstacle_hit_timer_.Update();

  return true;
}

void EnemyActor::Draw() {
  MY_ASSERTION(model_.get(), L"モデルが存在しません。");
  if (GetPosition().y <= -20) return;

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
  resource.GetPipeline().Get(PIPELINE_NAME)->SetCommandList(command_list);

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();
  transform_cb_.RegisterHandle(
      device, directx::shader::ConstantBufferRegisterID::TRANSFORM);

  model_->Draw(command_list);
}

void EnemyActor::SetPosition(math::Vector3 position) {
  transform_.SetPosition(position);
  // collision_.SetPosition(position);
}

//速度の設定
void EnemyActor::SetVelocity(math::Vector3 velocity) {
  enemy_ai_.Action(velocity, box_.get());
  is_move_ = true;
}

void EnemyActor::SetRotation() {
  math::Quaternion rotation = transform_.GetRotation();
  rotation.y += GetVelocity().x;
  transform_.SetRotation(rotation);
}

//移動に必要なパラメータを初期化
void EnemyActor::ResetParameter() {
  if (GetVelocity().Magnitude() != 0.0f) return;

  // deceleration_x_ = deceleration_z_ = 0;
  is_move_ = false;
  move_end_ = false;
}

//座標の取得
math::Vector3 EnemyActor::GetPosition() const {
  return transform_.GetPosition();
}

//移動量の取得
math::Vector3 EnemyActor::GetVelocity() const { return (box_->GetVelocity()); }

math::Quaternion EnemyActor::GetRotation() const {
  return transform_.GetRotation();
}

float EnemyActor::GetPower() const { return power_; }

float EnemyActor::GetStrength() const { return strength_; }

bool EnemyActor::GetMoveEnd() const { return (!is_move_ && move_end_); }

void EnemyActor::ResetMoveEnd() { move_end_ = false; }

float EnemyActor::DistanceWithPlayer() {
  auto p_pos = mediator_->GetPlayer()->GetPosition();
  auto e_pos = GetPosition();
  p_pos.y = 0;
  e_pos.y = 0;
  return (p_pos - e_pos).Magnitude();
}


void EnemyActor::SetType(i32 type_index) {
  type_index = std::clamp(type_index, 0,
                          (i32)enemy_type::EffectType::Effect_Type_End - 1);
  enemy_ai_.move_type_ = (enemy::enemy_type::MoveType)(
      game::GameDevice::GetInstance()->GetRandom().Range(
          0, enemy::enemy_type::MoveType::Move_Type_End));
  enemy_ai_.hit_type_ = (enemy::enemy_type::HitType)(
      game::GameDevice::GetInstance()->GetRandom().Range(
          0, enemy::enemy_type::HitType::Hit_Type_End));
  enemy_ai_.effect_type_ = (enemy::enemy_type::EffectType)(
      game::GameDevice::GetInstance()->GetRandom().Range(
          0, (i32)enemy_type::EffectType::Effect_Type_End - 1));

  switch (enemy_ai_.effect_type_) {
    case enemy_type::EffectType::Effect_None:
      enemy_ai_.SetAction(std::vector<EnemyAIType>{
          EnemyAIType::None,
      });
      break;
    case enemy_type::EffectType::Effect_Rotate:
      enemy_ai_.SetAction(std::vector<enemy::EnemyAIType>{
          enemy::EnemyAIType::Enemy_Rotate,
      });
      break;
    default:
      enemy_ai_.SetAction(std::vector<EnemyAIType>{
          EnemyAIType::None,
      });
      break;
  }
}

void EnemyActor::OnHit(bullet::Collider* other) {
  // system::Mode turn_mode = mediator_->GetCurrentTurn();
  // if (turn_mode == system::Mode::ENEMY_MOVING) {
  //  //プレイヤーに触れた
  //  if (player::Player* p = dynamic_cast<player::Player*>(other->GetOwner()))
  //  {
  //    HitAction(other);
  //    auto s = math::util::Clamp(strength_ - p->GetStrength(), 0.0f, 1.0f);
  //    auto trigonometric = (std::sin(30.0f * math::util::DEG_2_RAD * s));
  //    auto strength =
  //        math::Vector3::kUpVector * GetVelocity().Magnitude() *
  //        trigonometric;
  //    other->ApplyCentralImpulse(strength);
  //    CreateFireParticle(
  //        GetCollider()->GetHitPositions().at(other));
  //  }
  //}

  //糊に触れた
  {
    skill::SkillPaste* paste =
        dynamic_cast<skill::SkillPaste*>(other->GetOwner());
    if (paste) {
      //現状、止まるように
      GetCollider()->ApplyCentralImpulse(-0.1f * GetVelocity());
    }
  }
  //障害物に触れた
  {
    object::Obstacle* obstacle =
        dynamic_cast<object::Obstacle*>(other->GetOwner());
    if (obstacle) {
      if (!is_hit_obstacle_) {
        obstacle_hit_timer_.Init(1.0f, [&]() { is_hit_obstacle_ = false; });
        is_hit_obstacle_ = true;
        CreateFireParticle(GetCollider()->GetHitPositions().at(other));
      }
    }
  }
}

void EnemyActor::HitAction(bullet::Collider* other) {
  auto actor = other->GetOwner();
  const math::Vector3 position = transform_.GetPosition();
  const math::Vector3 other_position = actor->GetTransform().GetPosition();
  const math::Vector3 velocity = GetVelocity();
  const math::Vector3 direction =
      ((other_position - position).Normalized() + velocity.Normalized())
          .Normalized();

  auto add_power = ((strength_ + velocity.Magnitude()) / 2.0f);
  switch (enemy_ai_.hit_type_) {
      // 衝突時、停止する処理
    case enemy::enemy_type::HitType::Hit_Stop:
      GetCollider()->ApplyCentralImpulse(velocity * -1.0f);
      other->ApplyCentralImpulse(direction * add_power);
      break;
      // 衝突時、突進する処理
    case enemy::enemy_type::HitType::Hit_Rush:
      other->ApplyCentralImpulse(direction * add_power);
      break;
      // 衝突時、跳弾する処理
    case enemy::enemy_type::HitType::Hit_Bound:
      GetCollider()->ApplyCentralImpulse(velocity * -2.0f);
      other->ApplyCentralImpulse(direction * add_power);
      break;
  }
}

void EnemyActor::UpdateStrength(const float& weak) {
  strength_ += weak;
  if (strength_ <= min_strength_) strength_ = min_strength_;
}

void EnemyActor::CreateFireParticle(const util::Transform& transform) {
  auto fire = draw::particle::particle_factory::CreateFireParticle();
  fire->SetTransform(transform);
}
}  // namespace enemy
}  // namespace legend