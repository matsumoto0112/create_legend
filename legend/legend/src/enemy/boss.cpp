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

    move_type_ = (enemy::enemy_type::MoveType::Straight);
    hit_type_ = (enemy::enemy_type::HitType::Rush);
    effect_type_ = (enemy::enemy_type::EffectType::Rotate);

    return true;
  }
  return false;
}

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