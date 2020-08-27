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
      if (isMove) break;
      auto velocity = e->GetVelocity();
      velocity.y = 0;
      isMove = (!e->GetMoveEnd() || (0.01f <= velocity.Magnitude()));
    }
    if (boss_ != nullptr) {
      auto velocity = boss_->GetVelocity();
      velocity.y = 0;
      isMove |= (!boss_->GetMoveEnd() || (0.01f <= velocity.Magnitude()));
    }
    if (isMove) {
      action_enemy_index_ = 0;
      move_timer_ = 0.0f;
    }
  }
  // 敵行動
  EnemyAction(search_manaegr);
  return true;
}

void EnemyManager::Draw(actor::ActorRenderCommandList* render_command_list) {
  for (i32 i = 0; i < enemys_.size(); i++) {
    render_command_list->Push(enemys_[i].get());
  }
  if (boss_ != nullptr) {
    render_command_list->Push(boss_.get());
  }
}

void EnemyManager::EnemyAction(search::SearchManager* search_manaegr) {
  //ボスの行動indexは敵の配列サイズと一致したとき
  const bool is_index_is_boss = enemys_.size() == action_enemy_index_;
  //配列の範囲外かチェック
  const bool is_out_of_range =
      action_enemy_index_ < 0 || enemys_.size() <= action_enemy_index_;

  //範囲外でボス行動indexでないときは終了させる
  if (is_out_of_range && !is_index_is_boss) {
    return;
  }

  if (is_index_is_boss) {
    //ボスが存在しないときはそのindexは無効なので処理をしない
    if (!boss_) {
      return;
    }
  }

  if (move_timer_ <= 0.0f) {
    bullet::Collider* _collider;
    if ((boss_ != nullptr) && (enemys_.size() == action_enemy_index_)) {
      _collider = boss_->GetCollider();
    } else {
      _collider = enemys_[action_enemy_index_]->GetCollider();
    }

    {  //探索管理がある場合
      std::vector<bullet::Collider*> collisions;
      for (auto&& e : enemys_) {
        collisions.emplace_back(e->GetCollider());
      }
      if (boss_ != nullptr) {
        collisions.emplace_back(boss_->GetCollider());
      }
      auto next = search_manaegr->NextSearch(_collider, collisions) -
                  _collider->GetOwner()->GetTransform().GetPosition();
      next.y = 0;

      if ((boss_ != nullptr) &&
          (_collider->GetOwner() == boss_->GetCollider()->GetOwner())) {
        boss_->SetVelocity(next.Normalized() * move_speed_ * 1.5f);
      } else {
        enemys_[action_enemy_index_]->SetVelocity(next.Normalized() *
                                                  move_speed_);
      }
    }
  }

  move_timer_ +=
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();
  if (move_time_ <= move_timer_) {
    move_timer_ = 0.0f;
    action_enemy_index_++;
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
  if (boss_ != nullptr || is_game_clear_) {
    return;
  }

  boss_ = std::make_unique<Boss>();
  boss_->Init(actor_mediator_, paramater);
  // physics_field.AddEnemy(boss_->GetCollisionRef());
}

void EnemyManager::DestroyUpdate() {
  for (i32 index = 0; index < enemys_.size(); index++) {
    enemys_[index]->Update();
    if (enemys_[index]->GetPosition().y < -30) {
      Destroy(index);
      index--;
    }
  }
  if (boss_ != nullptr) {
    boss_->Update();
    DestroyBoss();
  }
}

void EnemyManager::Destroy(i32 index) {
  // if (index < 0 || enemys_.size() <= 0 || enemys_.size() <= index) {
  //  return;
  //}

  enemys_[index]->Remove();
  enemys_.erase(enemys_.begin() + index);
  if ((0 <= action_enemy_index_) && (index <= action_enemy_index_)) {
    action_enemy_index_--;
  }
}

void EnemyManager::DestroyBoss() {
  if (boss_ == nullptr) return;

  if (-10 <= boss_->GetPosition().y) {
    return;
  }
  boss_->Remove();
  boss_.reset();
  is_game_clear_ = true;
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
  move_timer_ = -1.0f;
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
  if (enemys_.size() <= 0 && boss_ == nullptr) {
    return true;
  }
  if (action_enemy_index_ < 0) {
    return true;
  }

  bool end = true;
  for (i32 i = 0; i < enemys_.size(); i++) {
    if (!enemys_[i]->GetMoveEnd()) {
      end = false;
    }
  }
  if ((boss_ != nullptr) && (!boss_->GetMoveEnd())) {
    end = false;
  }

  if (end) {
    for (i32 i = 0; i < enemys_.size(); i++) {
      if (enemys_[i]->GetVelocity().y < -0.1f) {
        return false;
      }
    }
    if ((boss_ != nullptr) && (boss_->GetVelocity().y < -0.1f)) {
      return false;
    }

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
