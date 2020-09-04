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
Enemy::Enemy() { is_move_ = false; }

//デストラクタ
Enemy::~Enemy() {}

//初期化
bool Enemy::Init(actor::IActorMediator* mediator,
                 const InitializeParameter& parameter) {
  if (enemy::EnemyActor::Init(mediator, parameter)) {
    auto& resource = game::GameDevice::GetInstance()->GetResource();
    model_ = resource.GetModel().Get(
        util::resource::resource_names::model::ENEMY_ERASER_01);

    SetType();

    return true;
  }
  return false;
}

void Enemy::OnHit(bullet::Collider* other) {
  enemy::EnemyActor::OnHit(other);
  system::Mode turn_mode = mediator_->GetCurrentTurn();
  if (turn_mode == system::Mode::ENEMY_MOVING) {
    //ボスに触れた
    if (enemy::Boss* b = dynamic_cast<enemy::Boss*>(other->GetOwner())) {
      HitAction(other);
    }
  }
}

}  // namespace enemy
}  // namespace legend