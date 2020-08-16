#include "enemy_manager.h"

#include "src/game/game_device.h"

namespace legend {
namespace enemy {

EnemyManager::EnemyManager() {}

EnemyManager::~EnemyManager() {}

bool EnemyManager::Initilaize() { return true; }

bool EnemyManager::Update(search::SearchManager* search_manaegr) {
  // “G’Ç‰Á
  // if (game::GameDevice::GetInstance()->GetInput().GetKeyboard()->GetKeyDown(
  //        input::key_code::A)) {
  //  // Add(physics_field);
  //}
  //// “Gíœ
  // else if (game::GameDevice::GetInstance()
  //             ->GetInput()
  //             .GetKeyboard()
  //             ->GetKeyDown(input::key_code::D)) {
  //  if (0 < enemys_.size()) {
  //    // Destroy(game::GameDevice::GetInstance()->GetRandom().Range(
  //    //    0, static_cast<i32>(enemys_.size())));
  //  }
  //}

  // “Gs“®
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
  //// “GÄn“®
  // else if (game::GameDevice::GetInstance()->GetInput().GetCommand(
  //             input::input_code::Decide)) {
  if ((action_enemy_index_ < 0) && (0 < enemys_.size())) {
    bool isMove = false;
    for (auto& e : enemys_) {
      isMove = (isMove || (0.01f <= e->GetVelocity().Magnitude()));
    }
    if (!isMove) {
      action_enemy_index_ = 0;
      move_timer_ = 0.0f;
    }
  }
  //}
  // “Gs“®
  EnemyAction(search_manaegr);

  for (auto&& enemy : enemys_) {
    enemy->Update();
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
    actor::Actor* _actor;
    if ((boss_ != nullptr) && (enemys_.size() <= action_enemy_index_)) {
      _actor = boss_.get();
    } else {
      _actor = enemys_[action_enemy_index_].get();
    }

    // ‘¬“xİ’è
    if (search_manaegr == nullptr) {  //’TõŠÇ—‚ª‚È‚¢ê‡
      auto pos = _actor->GetCollisionRef().GetPosition();
      math::Vector3 velocity = (player_obb_.GetPosition() - pos).Normalized();

      if (_actor == boss_.get()) {
        boss_->SetVelocity(velocity);
      } else {
        enemys_[action_enemy_index_]->SetVelocity(velocity);
      }
    } else {  //’TõŠÇ—‚ª‚ ‚éê‡
      std::vector<physics::BoundingBox*> collisions;
      for (i32 i = 0; i < enemys_.size(); i++) {
        collisions.emplace_back(&enemys_[i]->GetCollisionRef());
      }
      if (boss_ != nullptr) {
        collisions.emplace_back(&boss_->GetCollisionRef());
      }
      auto next =
          search_manaegr->NextSearch(&_actor->GetCollisionRef(), collisions) -
          _actor->GetCollisionRef().GetPosition();
      next.y = 0;

      {  //-------- “G‚ÌˆÚ“®—Êİ’è ---------------------
        std::string str;
        str += (action_enemy_index_<=0?"\n-------------------------------------------\n":"\n|\n");
        str += (action_enemy_index_ < enemys_.size()
                    ? "Enemy_" + std::to_string(action_enemy_index_)
                    : "Boss");
        str += ": (";
        str += std::to_string(next.x) + ", ";
        str += std::to_string(next.y) + ", ";
        str += std::to_string(next.z) + ")\n";

        if (boss_ == nullptr ? enemys_.size() - 1 <= action_enemy_index_
                             : enemys_.size() <= action_enemy_index_) {
          str += "\n-------------------------------------------\n";
        }
        std::wstring wstr = std::wstring(str.begin(), str.end());
        MY_LOG(wstr.c_str());
      }  //-------- “G‚ÌˆÚ“®—Êİ’è ---------------------

      if (_actor == boss_.get()) {
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

void EnemyManager::Add(const Enemy::InitializeParameter& paramater,
                       system::PhysicsField& physics_field) {
  if (enemy_max_count_ <= enemys_.size()) {
    return;
  }

  auto enemy = std::make_unique<Enemy>();
  enemy->Init(paramater);
  physics_field.AddEnemy(enemy->GetCollisionRef());

  enemys_.emplace_back(std::move(enemy));
  // SetPosition(enemys_[enemys_.size() - 1].get());
}

void EnemyManager::Add(const Boss::InitializeParameter& paramater,
                       system::PhysicsField& physics_field) {
  if (boss_ != nullptr) {
    return;
  }

  boss_ = std::make_unique<Boss>();
  boss_->Init(paramater);
  physics_field.AddEnemy(boss_->GetCollisionRef());
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

//ÅŒã‚Ì“G‚Ìæ“¾
Enemy* EnemyManager::GetLastEnemy() const {
  return enemys_.at(enemys_.size() - 1).get();
}

// obb‚ÌÀ•W‚ğŠî‚ÉÀ•WXV
void EnemyManager::SetPosition(system::PhysicsField& physics_field) {
  i32 remove_count = 0;
  for (i32 i = 0 - remove_count; i < enemys_.size(); i++) {
    enemys_[i]->SetPosition(physics_field.GetEnemyOBB(i).GetPosition());
    if (enemys_[i]->GetPosition().y <= -5.0f) {
      Destroy(i, physics_field);
      remove_count++;
    }
  }
}

//“G‚Ì”‚ğæ“¾
i32 EnemyManager::GetEnemiesSize() const {
  return static_cast<i32>(enemys_.size());
}

// obb‚Ì‘¬“x‚ğŠî‚É‘¬“xXV
void EnemyManager::SetVelocity(system::PhysicsField& physics_field) {
  for (i32 i = 0; i < enemys_.size(); i++) {
    if (enemys_[i]->GetMoveEnd()) continue;
    enemys_[i]->SetVelocity(physics_field.GetEnemyVelocity(i));
  }
}

void EnemyManager::ResetEnemyMove() {
  action_enemy_index_ = -1;
  move_timer_ = 0.0f;
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
  //‹ó‚©‚Ç‚¤‚©ƒ`ƒFƒbƒN
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

void EnemyManager::SetPlayer(const physics::BoundingBox& player_obb) {
  player_obb_ = player_obb;
}

void EnemyManager::DebugDraw(directx::device::CommandList& command_list) {
  for (i32 i = 0; i < enemys_.size(); i++) {
    enemys_[i]->GetCollisionRef().DebugDraw(command_list);
  }
  if (boss_ != nullptr) {
    boss_->GetCollisionRef().DebugDraw(command_list);
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
