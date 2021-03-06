#include "enemy_manager.h"

#include "src/game/game_device.h"

namespace legend {
namespace enemy {

// コンストラクタ
EnemyManager::EnemyManager() {}

// デスクトラクタ
EnemyManager::~EnemyManager() {
  auto enemyCount = enemys_.size();
  for (i32 i = 0; i < enemyCount; i++) {
    Destroy(0);
  }
  if (boss_) {
    boss_->Remove();
    boss_.reset();
  }
}

// 初期化
bool EnemyManager::Initilaize(actor::IActorMediator* mediator) {
  this->actor_mediator_ = mediator;
  return true;
}

// 更新
bool EnemyManager::Update(search::SearchManager* search_manaegr) {
  if ((action_enemy_index_ < 0) && (0 < enemys_.size() || boss_ != nullptr)) {
    bool isMove = false;
    // 移動しているか判定
    {
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
    }
    if (isMove) {
      action_enemy_index_ = 0;
      move_timer_ = 0.0f;
      std::sort(enemys_.begin(), enemys_.end(), [](auto& e1, auto& e2) {
        return (e1->DistanceWithPlayer() < e2->DistanceWithPlayer());
      });
    }
  }
  // 敵行動
  EnemyAction(search_manaegr);
  return true;
}

// 描画
void EnemyManager::Draw(
    actor::DifferedRenderingRenderCommandList* render_command_list) {
  for (i32 i = 0; i < enemys_.size(); i++) {
    render_command_list->Push(enemys_[i].get());
  }
  if (boss_ != nullptr) {
    render_command_list->Push(boss_.get());
  }
}

// 敵の行動処理
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
      auto next = search_manaegr->NextSearch(_collider, collisions);
      auto pPos = actor_mediator_->GetPlayer()->GetPosition();
      auto ePos = _collider->GetOwner()->GetTransform().GetPosition();
      next.y = 0;
      pPos.y = 0;
      ePos.y = 0;
      auto vector = next - ePos;
      auto length = math::util::Clamp(vector.Magnitude(), move_speed_min_,
                                      move_speed_max_);
      if ((next - pPos).Magnitude() < 1.0f) {
        length = move_speed_max_;
      }
      vector = vector.Normalized() * length;

      if ((boss_ != nullptr) &&
          (_collider->GetOwner() == boss_->GetCollider()->GetOwner())) {
        boss_->SetVelocity(vector);
      } else {
        enemys_[action_enemy_index_]->SetVelocity(vector);
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

// 敵を追加
void EnemyManager::AddEnemy(const EnemyActor::InitializeParameter& paramater) {
  if (enemy_max_count_ <= enemys_.size()) {
    return;
  }

  auto enemy = std::make_unique<Enemy>();
  enemy->Init(actor_mediator_, paramater);
  enemys_.emplace_back(std::move(enemy));
}

// ボスを追加
void EnemyManager::AddBoss(const EnemyActor::InitializeParameter& paramater) {
  if (boss_ != nullptr || is_game_clear_) {
    return;
  }

  auto enemy_count = enemys_.size();
  boss_ = std::make_unique<Boss>();
  boss_->Init(actor_mediator_, paramater);
  boss_->UpdateStrength(std::max(0.0f, static_cast<float>(enemy_count)) *
                        0.05f);
}

// 敵をボスに集合させる
bool EnemyManager::AbsorpEnemies() {
  if (boss_ == nullptr) return false;
  bool isAbsorp = false;
  for (i32 i = 0; i < enemys_.size(); i++) {
    //パーティクルの強制有効化を働かせる
    enemys_[i]->ParticleForceEmitEnable();

    enemys_[i]->RemoveCollider();
    auto pos = enemys_[i]->GetTransformRef().GetPosition();
    auto vector = (boss_->GetPosition() - pos);
    if (5.0f < vector.Magnitude()) {
      auto update_time_ = game::GameDevice::GetInstance()
                              ->GetFPSCounter()
                              .GetDeltaSeconds<float>();
      pos += vector.Normalized() * move_speed_max_ * update_time_;
      enemys_[i]->GetTransformRef().SetPosition(pos);
      isAbsorp = true;
    } else {
      Destroy(i);
      i--;
    }
  }
  if (isAbsorp) {
    boss_->GetCollider()->SetVelocity(math::Vector3::kZeroVector);
  }

  return isAbsorp;
}

// 削除処理
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

// 敵を削除
void EnemyManager::Destroy(i32 index) {
  enemys_[index]->Remove();
  enemys_.erase(enemys_.begin() + index);
  if ((0 <= action_enemy_index_) && (index <= action_enemy_index_)) {
    action_enemy_index_--;
  }
}

// ボスを削除
void EnemyManager::DestroyBoss() {
  if (boss_ == nullptr) return;

  if (-10 <= boss_->GetPosition().y) {
    return;
  }
  boss_->Remove();
  boss_.reset();
  is_game_clear_ = true;
}

// ポジション設定
void EnemyManager::SetPosition(Enemy* enemy) {
  auto x = game::GameDevice::GetInstance()->GetRandom().Range(-1.0f, 1.0f);
  auto z = game::GameDevice::GetInstance()->GetRandom().Range(-1.0f, 1.0f);
  math::Vector3 position(x, 0.0f, z);
  enemy->SetPosition(position);
}

// 最後の敵の取得
Enemy* EnemyManager::GetLastEnemy() const {
  return enemys_.at(enemys_.size() - 1).get();
}

// 敵の数を取得
i32 EnemyManager::GetEnemiesSize() const {
  return static_cast<i32>(enemys_.size());
}

// 敵の行動を初期化
void EnemyManager::ResetEnemyMove() {
  action_enemy_index_ = -1;
  move_timer_ = -1.0f;
}

// 各敵の速度を取得
std::vector<math::Vector3> EnemyManager::GetVelocities() {
  velocities_.resize(enemys_.size());
  for (i32 i = 0; i < velocities_.size(); i++) {
    velocities_[i] = enemys_[i]->GetVelocity();
  }

  return velocities_;
}

// 最後の敵の移動終了判定を取得
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

  // 移動し終わっていたら移動処理をリセット
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

// プレイヤー設定
void EnemyManager::SetPlayer(bullet::Collider* player_obb) {
  player_collider_ = player_obb;
}

// 敵のポインタリスト取得
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
