#include "enemy_manager.h"
#include "src/game/game_device.h"

namespace legend {
namespace enemy {
EnemyManager::EnemyManager() {}

EnemyManager::~EnemyManager() {}

bool EnemyManager::Initilaize() {
  i32 max = 4;
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  for (i32 i = 0; i < max; i++) {
    Add();
    auto enemy = enemys_[enemys_.size() - 1].get();
    auto x = game::GameDevice::GetInstance()->GetRandom().Range(-5.0f, 5.0f);
    auto z = game::GameDevice::GetInstance()->GetRandom().Range(-5.0f, 5.0f);
    math::Vector3 position(x, 0.0f, z);
    enemy->SetPosition(position);
  }
  return true;
}

bool EnemyManager::Update() {
	// “G’Ç‰Á
  if (game::GameDevice::GetInstance()->GetInput().GetKeyboard()->GetKeyDown(
          input::key_code::A)) {
    Add();
    auto enemy = enemys_[enemys_.size() - 1].get();
    auto x = game::GameDevice::GetInstance()->GetRandom().Range(-5.0f, 5.0f);
    auto z = game::GameDevice::GetInstance()->GetRandom().Range(-5.0f, 5.0f);
    math::Vector3 position(x, 0.0f, z);
    enemy->SetPosition(position);
  } 
  // “Gíœ
  else if (game::GameDevice::GetInstance()->GetInput().GetKeyboard()
               ->GetKeyDown(input::key_code::D)) {
    Destroy(
        game::GameDevice::GetInstance()->GetRandom().Range(0, static_cast<i32>(enemys_.size())));
  }

  // “Gs“®
  if (game::GameDevice::GetInstance()->GetInput().GetCommand(
                 input::input_code::Pause)) {
    if ((action_enemy_index_ < 0) || (0 < enemys_.size())) {
      action_enemy_index_ = -1;
      move_timer_ = 0.0f;
      for (auto&& enemy : enemys_) {
        auto x =
            game::GameDevice::GetInstance()->GetRandom().Range(-5.0f, 5.0f);
        auto z =
            game::GameDevice::GetInstance()->GetRandom().Range(-5.0f, 5.0f);
        math::Vector3 position(x, 0.0f, z);
        enemy->SetPosition(position);
        enemy->SetVelocity(math::Vector3::kZeroVector);
      }
    }
  }
  // “GÄŽn“®
  else if (game::GameDevice::GetInstance()->GetInput().GetCommand(
          input::input_code::Decide)) {
    if ((action_enemy_index_ < 0) || (0 < enemys_.size())) {
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
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  for (i32 i = 0; i < enemys_.size(); i++) {
    enemys_[i]->Draw(device);
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

void EnemyManager::Add() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  auto enemy = std::make_unique<Enemy>();
  enemy->Initilaize(device, resource);
  enemys_.emplace_back(std::move(enemy));
}

void EnemyManager::Destroy(i32 index) {
  if (index < 0 || enemys_.size() <= index) {
    return;
  }

  enemys_.erase(enemys_.begin() + index);
  if ((0 < action_enemy_index_) && (index < action_enemy_index_)) {
    action_enemy_index_--;
  }
}

}  // namespace enemy
}  // namespace legend
