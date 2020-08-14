#include "enemy_manager.h"

#include "src/game/game_device.h"

namespace legend {
namespace enemy {

EnemyManager::EnemyManager() {}

EnemyManager::~EnemyManager() {}

bool EnemyManager::Initilaize() { return true; }

bool EnemyManager::Update(search::SearchManager* search_manaegr) {
  // 敵追加
  // if (game::GameDevice::GetInstance()->GetInput().GetKeyboard()->GetKeyDown(
  //        input::key_code::A)) {
  //  // Add(physics_field);
  //}
  //// 敵削除
  // else if (game::GameDevice::GetInstance()
  //             ->GetInput()
  //             .GetKeyboard()
  //             ->GetKeyDown(input::key_code::D)) {
  //  if (0 < enemys_.size()) {
  //    // Destroy(game::GameDevice::GetInstance()->GetRandom().Range(
  //    //    0, static_cast<i32>(enemys_.size())));
  //  }
  //}

  // 敵行動
  // if (game::GameDevice::GetInstance()->GetInput().GetCommand(
  //        input::input_code::Pause)) {
  //  if ((action_enemy_index_ < 0) || (0 < enemys_.size())) {
  //    action_enemy_index_ = -1;
  //    move_timer_ = 0.0f;
  //    for (i32 index = 0; index < enemys_.size(); index++) {
  //      auto enemy = enemys_[index].get();
  //      SetPosition(enemy);
  //      enemy->SetVelocity(math::Vector3::kZeroVector);
  //    }
  //  }
  //}
  //// 敵再始動
  // else if (game::GameDevice::GetInstance()->GetInput().GetCommand(
  //             input::input_code::Decide)) {
  if ((action_enemy_index_ < 0) && (0 < enemys_.size())) {
    action_enemy_index_ = 0;
    move_timer_ = 0.0f;
  }
  //}
  // 敵行動
  EnemyAction(search_manaegr);

  for (auto&& enemy : enemys_) {
    enemy->Update();
  }
  return true;
}

void EnemyManager::Draw() {
  for (i32 i = 0; i < enemys_.size(); i++) {
    enemys_[i]->Draw();
  }
}

void EnemyManager::EnemyAction(search::SearchManager* search_manaegr) {
  if ((action_enemy_index_ < 0) || (enemys_.size() <= action_enemy_index_)) {
    return;
  }
  if (move_timer_ <= 0.0f) {
    if (search_manaegr == nullptr) {
      math::Vector3 velocity = (player_obb_.GetPosition() -
                                enemys_[action_enemy_index_]->GetPosition())
                                   .Normalized();

      enemys_[action_enemy_index_]->SetVelocity(velocity);
    } else {
      std::vector<physics::BoundingBox*> collisions;
      for (i32 i = 0; i < enemys_.size(); i++) {
        collisions.emplace_back(&enemys_[i]->GetCollisionRef());
      }
      auto next = search_manaegr->NextSearch(
                      &enemys_[action_enemy_index_].get()->GetCollisionRef(),
                      collisions) -
                  enemys_[action_enemy_index_].get()->GetPosition();
      next.y = 0;
      enemys_[action_enemy_index_]->SetVelocity(next);
    }
  }
  move_timer_ +=
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();
  if (move_time_ <= move_timer_) {
    move_timer_ = 0.0f;
    action_enemy_index_ = (action_enemy_index_ + 1 < enemys_.size())
                              ? (action_enemy_index_ + 1)
                              : -1;
  }
}

void EnemyManager::Add(const Enemy::InitializeParameter& paramater,
                       system::PhysicsField& physics_field) {
  if (enemy_max_count_ <= enemys_.size()) {
    return;
  }

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  auto enemy = std::make_unique<Enemy>();

  enemy->Init(paramater);
  physics_field.AddEnemy(enemy->GetCollisionRef());

  enemys_.emplace_back(std::move(enemy));
  // SetPosition(enemys_[enemys_.size() - 1].get());
}

void EnemyManager::Destroy(i32 index, system::PhysicsField& physics_field) {
  if (index < 0 || enemys_.size() <= 0 || enemys_.size() <= index) {
    return;
  }

  physics_field.RemoveEnemy(index);
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
void EnemyManager::SetPosition(system::PhysicsField& physics_field) {
  for (i32 i = 0; i < enemys_.size(); i++) {
    enemys_[i]->SetPosition(physics_field.GetEnemyOBB(i).GetPosition());
    if (enemys_[i]->GetPosition().y <= -5.0f) Destroy(i, physics_field);
  }
}

//敵の数を取得
i32 EnemyManager::GetEnemiesSize() const {
  return static_cast<i32>(enemys_.size());
}

// obbの速度を基に速度更新
void EnemyManager::SetVelocity(system::PhysicsField& physics_field) {
  for (i32 i = 0; i < enemys_.size(); i++) {
    if (enemys_[i]->GetMoveEnd()) continue;
    enemys_[i]->SetVelocity(physics_field.GetEnemyVelocity(i));
  }
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

  bool end = false;
  if (enemys_[enemys_.size() - 1]->GetMoveEnd()) {
    end = true;
    for (i32 i = 0; i < enemys_.size(); i++) {
      enemys_[i]->ResetMoveEnd();
    }
  }
  return end;
}

void EnemyManager::SetPlayer(const physics::BoundingBox& player_obb) {
  player_obb_ = player_obb;
}

void EnemyManager::DebugDraw(directx::device::CommandList& command_list) {
  for (i32 i = 0; i < enemys_.size(); i++) {
    enemys_[i]->GetCollisionRef().DebugDraw(command_list);
  }
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
