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

namespace {
using legend::math::Quaternion;
using legend::math::Vector3;
Quaternion CreateLookAt(const Vector3& target) {
  const float dot = Vector3::Dot(Vector3::kForwardVector, target.Normalized());
  const float theta = legend::math::util::Acos(dot);
  const Vector3 cross =
      Vector3::Cross(Vector3::kForwardVector, target).Normalized();

  return Quaternion(cross, theta);
}
}  // namespace

namespace legend {
namespace enemy {
//コンストラクタ
Boss::Boss() { is_move_ = false; }

//デストラクタ
Boss::~Boss() {}

//初期化
bool Boss::Init(actor::IActorMediator* mediator,
                const InitializeParameter& parameter) {
  if (enemy::EnemyActor::Init(mediator, parameter)) {
    auto& resource = game::GameDevice::GetInstance()->GetResource();
    model_ =
        resource.GetModel().Get(util::resource::resource_names::model::BOSS);

	SetType(parameter.type_index);

    strength_ = 1.5f;

    return true;
  }
  return false;
}

bool Boss::Update() {
  enemy::EnemyActor::Update();
  Boss_Rotate_Stand();
  Boss_Rush_Move();
  return true;
}

//速度の設定
void Boss::SetVelocity(math::Vector3 velocity) {
  switch (enemy_ai_.ai_type_) {
    case enemy::EnemyAIType::Boss_Rotate_Stand:
      is_rotate_ = true;
      rotate_timer_ = rotate_time_;
      break;
    case enemy::EnemyAIType::Boss_Rush_Move:
      auto power = velocity.Magnitude();
      velocity = (GetTransform().GetRotation() * math::Vector3::kForwardVector);
      velocity.y = 0.0f;
      velocity = velocity.Normalized() * power;
      is_rush_ = true;
      break;
  }
  enemy_ai_.Action(velocity, box_.get());
  is_move_ = true;
}

void Boss::SetType(i32 type_index) {
  type_index = std::clamp(type_index, 0, (i32)enemy_type::EffectType::Effect_Type_End);
  enemy_ai_.move_type_ = (enemy_type::MoveType::Straight);
  enemy_ai_.hit_type_ = (enemy_type::HitType::Rush);
  enemy_ai_.effect_type_ = (enemy_type::EffectType)type_index;
  if (enemy_ai_.effect_type_ == enemy_type::EffectType::None) {
    enemy_ai_.SetAction(std::vector<EnemyAIType>{
        EnemyAIType::Boss_Rotate_Stand,
        EnemyAIType::Boss_Rush_Move,
    });
  } else if (enemy_ai_.effect_type_ == enemy::enemy_type::EffectType::Rotate) {
    enemy_ai_.SetAction(std::vector<enemy::EnemyAIType>{
        enemy::EnemyAIType::Boss_Rotate_Move,
    });
  }
}

void Boss::OnHit(bullet::Collider* other) {
  enemy::EnemyActor::OnHit(other);
  system::Mode turn_mode = mediator_->GetCurrentTurn();
  if (turn_mode == system::Mode::ENEMY_MOVING) {
    //プレイヤーに触れた
    if (player::Player* p = dynamic_cast<player::Player*>(other->GetOwner())) {
      HitAction(other);
      auto s = math::util::Clamp(strength_ - p->GetStrength(), 0.0f, 1.0f);
      auto trigonometric = (std::sin(30.0f * math::util::DEG_2_RAD * s));
      auto strength =
          math::Vector3::kUpVector * GetVelocity().Magnitude() * trigonometric;
      other->ApplyCentralImpulse(strength);
      CreateFireParticle(GetCollider()->GetHitPositions().at(other));
    }
    //敵に触れた
    if (enemy::Enemy* e = dynamic_cast<enemy::Enemy*>(other->GetOwner())) {
      HitAction(other);
    }
  }
}

void Boss::Boss_Rotate_Stand() {
  if (is_rotate_) {
    auto vector =
        (mediator_->GetPlayer()->GetPosition() - GetPosition()).Normalized();
    vector.y = 0;
    vector.Normalized();

    auto q = CreateLookAt(vector);
    // auto speed = (rotate_speed_ - box_->GetAngularVelocity().Magnitude());
    box_->SetAngularVelocity(math::Vector3::kUpVector /** q.w*/ *
                             rotate_speed_);

    auto f = ((GetTransform().GetRotation() * math::Vector3::kForwardVector)
                  .Normalized());
    f.y = 0;
    f.Normalized();

    is_move_ = true;

    rotate_timer_ =
        math::util::Clamp(rotate_timer_ - update_time_, 0.0f, rotate_time_);
    if (0.0f < rotate_timer_) return;
    if (0.99f < Vector3::Dot(vector, f)) {
      box_->SetAngularVelocity(math::Vector3::kZeroVector);
      is_rotate_ = false;
      is_move_ = false;
    }
  }
}
void Boss::Boss_Rush_Move() {
  if (is_rush_) {
    auto position = GetPosition();
    auto direction = GetVelocity().Normalized();
    direction.y = 0;
    direction = direction.Normalized();

    auto point = position + direction * 30.0f;
    const auto raycast =
        mediator_->RayCast(point, point + math::Vector3::kDownVector * 10.0f);
    auto objs = raycast.m_collisionObjects;

    if (objs.size() <= 0) {
      box_->ApplyCentralImpulse(direction * GetVelocity().Magnitude() * -0.5f);
    }
    is_rush_ = is_move_;
  }
}
}  // namespace enemy
}  // namespace legend