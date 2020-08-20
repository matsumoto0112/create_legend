#include "enemy_manager.h"

#include "src/game/game_device.h"

namespace legend {
namespace enemy {

EnemyManager::EnemyManager() {}

EnemyManager::~EnemyManager() {}

bool EnemyManager::Initilaize(actor::IActorMediator* mediator) {
  this->actor_mediator_ = mediator;
  return true;
}

bool EnemyManager::Update(search::SearchManager* search_manaegr) {
  if ((action_enemy_index_ < 0) && (0 < enemys_.size() || boss_ != nullptr)) {
    bool isMove = false;
    for (auto& e : enemys_) {
      isMove = (isMove || e->GetMoveEnd() ||(0.01f <= e->GetVelocity().Magnitude()));
    }
    if (boss_ != nullptr) {
      isMove = (isMove || (0.01f <= boss_->GetVelocity().Magnitude()));
    }
    if (!isMove) {
      action_enemy_index_ = 0;
      move_timer_ = 0.0f;
    }
  }
  // 敵行動
  EnemyAction(search_manaegr);

  for (i32 index = 0; index < enemys_.size(); index++) {
    if (enemys_[index]->GetPosition().y < -30) {
      Destroy(index);
      index--;
    } else {
      enemys_[index]->Update();
    }
  }
  if (boss_ != nullptr) {
    boss_->Update();
  }
  return true;
}

void EnemyManager::Draw() {
  for (i32 i = 0; i < enemys_.size(); i++) {
    enemys_[i]->Draw();
  }
  if (boss_ != nullptr) {
    boss_->Draw();
  }
}

void EnemyManager::EnemyAction(search::SearchManager* search_manaegr) {
  if ((action_enemy_index_ < 0) || (enemys_.size() < action_enemy_index_)) {
    return;
  }
  if (move_timer_ <= 0.0f) {
    bullet::Collider* _collider;
    if ((boss_ != nullptr) && (enemys_.size() <= action_enemy_index_)) {
      _collider = boss_->GetCollider();
    } else {
      _collider = enemys_[action_enemy_index_]->GetCollider();
    }

    //// 速度設定
    // if (search_manaegr == nullptr) {  //探索管理がない場合
    //  auto pos = _actor->GetTransform().GetPosition();
    //  math::Vector3 velocity = (actor_mediator_->GetPlayer()->GetPosition() -
    //  pos).Normalized();

    //  if (_actor == boss_.get()) {
    //    boss_->SetVelocity(velocity);
    //  } else {
    //    enemys_[action_enemy_index_]->SetVelocity(velocity);
    //  }
    //} else
    {  //探索管理がある場合
      std::vector<bullet::Collider*> collisions;
      for (i32 i = 0; i < enemys_.size(); i++) {
        collisions.emplace_back(enemys_[i]->GetCollider());
      }
      if (boss_ != nullptr) {
        collisions.emplace_back(boss_->GetCollider());
      }
      auto next = search_manaegr->NextSearch(_collider, collisions) -
                  _collider->GetOwner()->GetTransform().GetPosition();
      next.y = 0;

      if ((boss_ != nullptr) &&
          (_collider->GetOwner() == boss_->GetCollider()->GetOwner())) {
        boss_->SetVelocity(next);
      } else {
        enemys_[action_enemy_index_]->SetVelocity(next);
      }
    }
  }

  move_timer_ +=
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();
  if (move_time_ <= move_timer_) {
    move_timer_ = 0.0f;
    action_enemy_index_ = (action_enemy_index_ + 1 < enemys_.size())
                              ? (action_enemy_index_ + 1)
                              : -1;
    if (boss_ != nullptr) {
      action_enemy_index_ = (enemys_.size() <= action_enemy_index_)
                                ? -1
                                : (action_enemy_index_ < 0)
                                      ? static_cast<i32>(enemys_.size())
                                      : action_enemy_index_;
    }
  }
}

void EnemyManager::Add(const Enemy::InitializeParameter& paramater) {
  if (enemy_max_count_ <= enemys_.size()) {
    return;
  }

  auto enemy = std::make_unique<Enemy>();
  enemy->Init(actor_mediator_, paramater);
  // physics_field.AddEnemy(enemy->GetCollisionRef());

  enemys_.emplace_back(std::move(enemy));
  // SetPosition(enemys_[enemys_.size() - 1].get());
}

void EnemyManager::Add(const Boss::InitializeParameter& paramater) {
  if (boss_ != nullptr) {
    return;
  }

  boss_ = std::make_unique<Boss>();
  boss_->Init(actor_mediator_, paramater);
  // physics_field.AddEnemy(boss_->GetCollisionRef());
}

void EnemyManager::Destroy(i32 index) {
  if (index < 0 || enemys_.size() <= 0 || enemys_.size() <= index) {
    return;
  }

  enemys_[index]->Remove();
  enemys_.erase(enemys_.begin() + index);
  if ((0 < action_enemy_index_) && (index < action_enemy_index_)) {
    action_enemy_index_--;
  }
}

void EnemyManager::SetPosition(Enemy* enemy) {
  auto x = game::GameDevice::GetInstance()->GetRandom().Range(-1.0f, 1.0f);
  auto z = game::GameDevice::GetInstance()->GetRandom().Range(-1.0f, 1.0f);
  math::Vector3 position(x, 0.0f, z);
  enemy->SetPosition(position);
}

//最後の敵の取得
Enemy* EnemyManager::GetLastEnemy() const {
  return enemys_.at(enemys_.size() - 1).get();
}

// obbの座標を基に座標更新
// void EnemyManager::SetPosition(system::PhysicsField& physics_field) {
//  //for (i32 i = 0; i < enemys_.size(); i++) {
//  //  enemys_[i]->SetPosition(physics_field.GetEnemyOBB(i).GetPosition());
//  //  if (enemys_[i]->GetPosition().y <= -5.0f) {
//  //    Destroy(i, physics_field);
//  //    i--;
//  //  }
//  //}
//}

//敵の数を取得
i32 EnemyManager::GetEnemiesSize() const {
  return static_cast<i32>(enemys_.size());
}

//// obbの速度を基に速度更新
// void EnemyManager::SetVelocity(system::PhysicsField& physics_field) {
//  for (i32 i = 0; i < enemys_.size(); i++) {
//    if (enemys_[i]->GetMoveEnd()) continue;
//    enemys_[i]->SetVelocity(physics_field.GetEnemyVelocity(i));
//  }
//}

void EnemyManager::ResetEnemyMove() {
  action_enemy_index_ = -1;
  move_timer_ = 0.0f;
}

//各敵の速度を取得
std::vector<math::Vector3> EnemyManager::GetVelocities() {
  velocities_.resize(enemys_.size());
  for (i32 i = 0; i < velocities_.size(); i++) {
    velocities_[i] = enemys_[i]->GetVelocity();
  }

  return velocities_;
}

//最後の敵の移動終了判定を取得
bool EnemyManager::LastEnemyMoveEnd() const {
  //空かどうかチェック
  if (enemys_.empty()) {
    return false;
  }
  if (!(0 <= move_timer_ && action_enemy_index_ < 0)) {
    return false;
  }

  bool end = false;
   if (boss_ == nullptr ? enemys_[enemys_.size() - 1]->GetMoveEnd()
                       : boss_->GetMoveEnd()) {
    end = true;
    for (i32 i = 0; i < enemys_.size(); i++) {
      enemys_[i]->ResetMoveEnd();
    }
    if (boss_ != nullptr) {
      boss_->ResetMoveEnd();
    }
  }
  return end;
}

void EnemyManager::SetPlayer(bullet::Collider* player_obb) {
  player_collider_ = player_obb;
}

void EnemyManager::DebugDraw(directx::device::CommandList& command_list) {
  // for (i32 i = 0; i < enemys_.size(); i++) {
  //  enemys_[i]->GetCollisionRef().DebugDraw(command_list);
  //}
  // if (boss_ != nullptr) {
  //  boss_->GetCollisionRef().DebugDraw(command_list);
  //}
}

std::vector<Enemy*> EnemyManager::GetEnemyPointers() const {
  std::vector<Enemy*> res(enemys_.size());
  const u32 size = static_cast<u32>(enemys_.size());
  for (u32 i = 0; i < size; i++) {
    res[i] = enemys_[i].get();
  }
  return res;
}

}  // namespace enemy
}  // namespace legend
