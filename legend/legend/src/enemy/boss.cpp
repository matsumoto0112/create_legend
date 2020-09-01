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

    enemy_ai_.move_type_ = (enemy::enemy_type::MoveType::Straight);
    enemy_ai_.hit_type_ = (enemy::enemy_type::HitType::Rush);
    enemy_ai_.effect_type_ = (enemy::enemy_type::EffectType::Rotate);
    enemy_ai_.ai_type_ =
        (enemy_ai_.effect_type_ == enemy::enemy_type::EffectType::Rotate)
            ? enemy::EnemyAIType::Boss_Rotate_Stand
            : enemy::EnemyAIType::None;

    strength_ = 1.5f;

    return true;
  }
  return false;
}

bool Boss::Update() {
  enemy::EnemyActor::Update();
  if (is_rotate_) {
    auto vector =
        (mediator_->GetPlayer()->GetPosition() - GetPosition()).Normalized();
    vector.y = 0;
    vector.Normalized();

    auto q = CreateLookAt(vector);
    auto speed = (rotate_speed_ - box_->GetAngularVelocity().Magnitude());
    box_->SetAngularVelocity(math::Vector3::kUpVector * q.w * speed);

    auto f = ((GetTransform().GetRotation() * math::Vector3::kForwardVector)
                  .Normalized());
    f.y = 0;
    f.Normalized();

    is_move_ = true;

    if (0.975f < Vector3::Dot(vector, f)) {
          box_->SetAngularVelocity(math::Vector3::kZeroVector);
      is_rotate_ = false;
      is_move_ = false;
    }
  }
  return true;
}

//速度の設定
void Boss::SetVelocity(math::Vector3 velocity) {
  if (enemy_ai_.ai_type_ == enemy::EnemyAIType::Boss_Rotate_Stand) {
    is_rotate_ = true;
  }
  enemy_ai_.Action(velocity, box_.get());
  is_move_ = true;
}

void Boss::OnHit(bullet::Collider* other) {
  enemy::EnemyActor::OnHit(other);
  system::Mode turn_mode = mediator_->GetCurrentTurn();
  if (turn_mode == system::Mode::ENEMY_MOVING) {
    //敵に触れた
    if (enemy::Enemy* e = dynamic_cast<enemy::Enemy*>(other->GetOwner())) {
      HitAction(other);
    }
  }
}
}  // namespace enemy
}  // namespace legend