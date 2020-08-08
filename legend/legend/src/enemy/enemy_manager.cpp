#include "enemy_manager.h"

#include "src/game/game_device.h"

namespace legend {
namespace enemy {
EnemyManager::EnemyManager() {}

EnemyManager::~EnemyManager() {}

bool EnemyManager::Initilaize(system::PhysicsField* physics_field) {
  i32 max = 4;
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  for (i32 i = 0; i < max; i++) {
    Add(physics_field);
  }
  return true;
}

bool EnemyManager::Update(system::PhysicsField* physics_field) {
  // “G’Ç‰Á
  if (game::GameDevice::GetInstance()->GetInput().GetKeyboard()->GetKeyDown(
          input::key_code::A)) {
    Add(physics_field);
  }
  // “Gíœ
  else if (game::GameDevice::GetInstance()
               ->GetInput()
               .GetKeyboard()
               ->GetKeyDown(input::key_code::D)) {
    if (0 < enemys_.size()) {
      Destroy(game::GameDevice::GetInstance()->GetRandom().Range(
                  0, static_cast<i32>(enemys_.size())),
              physics_field);
    }
  }

  // “Gs“®
  if (game::GameDevice::GetInstance()->GetInput().GetCommand(
          input::input_code::Pause)) {
    if ((action_enemy_index_ < 0) || (0 < enemys_.size())) {
      action_enemy_index_ = -1;
      move_timer_ = 0.0f;
      for (i32 index = 0; index < enemys_.size(); index++) {
        auto enemy = enemys_[index].get();
        SetPosition(enemy);
        enemy->SetVelocity(math::Vector3::kZeroVector);
      }
    }
  }
  // “GÄn“®
  else if (game::GameDevice::GetInstance()->GetInput().GetCommand(
               input::input_code::Decide)) {
    if ((action_enemy_index_ < 0) && (0 < enemys_.size())) {
      action_enemy_index_ = 0;
      move_timer_ = 0.0f;
    }
  }
  // “Gs“®
  EnemyAction();

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

void EnemyManager::EnemyAction() {
  if ((action_enemy_index_ < 0) || (enemys_.size() <= action_enemy_index_)) {
    return;
  }
  if (move_timer_ <= 0.0f) {
    auto h = game::GameDevice::GetInstance()->GetRandom().Range(-1.0f, 1.0f);
    auto v = game::GameDevice::GetInstance()->GetRandom().Range(-1.0f, 1.0f);
    math::Vector3 velocity = (math::Vector3(v, 0, h)).Normalized();
    enemys_[action_enemy_index_]->SetVelocity(velocity);
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

void EnemyManager::Add(system::PhysicsField* physics_field) {
  if (enemy_max_count_ <= enemys_.size()) {
    return;
  }

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  auto enemy = std::make_unique<Enemy>();

  auto x = game::GameDevice::GetInstance()->GetRandom().Range(-1.0f, 1.0f);
  auto z = game::GameDevice::GetInstance()->GetRandom().Range(-1.0f, 1.0f);
  auto paramater = enemy::Enemy::InitializeParameter();
  paramater.transform =
      util::Transform(math::Vector3(x, 0, z), math::Quaternion::kIdentity,
                      math::Vector3::kUnitVector);
  paramater.bouding_box_length = math::Vector3(1.0f, 0.5f, 2.0f);
  enemy->Init(paramater);
  if (physics_field != nullptr) {
    physics_field->AddEnemy(enemy->GetCollisionRef());
  }

  enemys_.emplace_back(std::move(enemy));
  // SetPosition(enemys_[enemys_.size() - 1].get());
}

void EnemyManager::Destroy(i32 index, system::PhysicsField* physics_field) {
  if (index < 0 || enemys_.size() <= 0 || enemys_.size() <= index) {
    return;
  }

  enemys_.erase(enemys_.begin() + index);
  if ((0 < action_enemy_index_) && (index < action_enemy_index_)) {
    action_enemy_index_--;
  }

  if (physics_field != nullptr) {
    physics_field->RemoveEnemy(index);
  }
}

void EnemyManager::SetPosition(Enemy* enemy) {
  auto x = game::GameDevice::GetInstance()->GetRandom().Range(-1.0f, 1.0f);
  auto z = game::GameDevice::GetInstance()->GetRandom().Range(-1.0f, 1.0f);
  math::Vector3 position(x, 0.0f, z);
  enemy->SetPosition(position);
}

//ÅŒã‚Ì“G‚Ìæ“¾
Enemy* EnemyManager::GetLastEnemy() const {
  return enemys_.at(enemys_.size() - 1).get();
}

// obb‚ÌÀ•W‚ğŠî‚ÉÀ•WXV
void EnemyManager::SetPosition(system::PhysicsField* physics_field) {
  for (i32 i = 0; i < enemys_.size(); i++) {
    enemys_[i]->SetPosition(physics_field->GetEnemyOBB(i).GetPosition());
    if (enemys_[i]->GetPosition().y <= -10.0f) Destroy(i, physics_field);
  }
}

//“G‚Ì”‚ğæ“¾
i32 EnemyManager::GetEnemiesSize() const {
  return static_cast<i32>(enemys_.size());
}

// obb‚Ì‘¬“x‚ğŠî‚É‘¬“xXV
void EnemyManager::SetVelocity(system::PhysicsField* physics_field) {
  for (i32 i = 0; i < enemys_.size(); i++) {
    if (enemys_[i]->GetMoveEnd()) continue;
    enemys_[i]->SetVelocity(physics_field->GetEnemyVelocity(i));
  }
}

//Še“G‚Ì‘¬“x‚ğæ“¾
std::vector<math::Vector3> EnemyManager::GetVelocities() {
  velocities_.resize(enemys_.size());
  for (i32 i = 0; i < velocities_.size(); i++) {
    velocities_[i] = enemys_[i]->GetVelocity();
  }

  return velocities_;
}

//ÅŒã‚Ì“G‚ÌˆÚ“®I—¹”»’è‚ğæ“¾
bool EnemyManager::LastEnemyMoveEnd() const {
  bool end = false;
  if (enemys_[enemys_.size() - 1]->GetMoveEnd()) {
    end = true;
    for (i32 i = 0; i < enemys_.size(); i++) {
      enemys_[i]->ResetMoveEnd();
    }
  }
  return end;
}

}  // namespace enemy
}  // namespace legend
