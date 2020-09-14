#include "enemy_manager.h"

#include "src/game/game_device.h"

namespace legend {
namespace enemy {

// �R���X�g���N�^
EnemyManager::EnemyManager() {}

// �f�X�N�g���N�^
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

// ������
bool EnemyManager::Initilaize(actor::IActorMediator* mediator) {
  this->actor_mediator_ = mediator;
  return true;
}

// �X�V
bool EnemyManager::Update(search::SearchManager* search_manaegr) {
  if ((action_enemy_index_ < 0) && (0 < enemys_.size() || boss_ != nullptr)) {
    bool isMove = false;
    // �ړ����Ă��邩����
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
  // �G�s��
  EnemyAction(search_manaegr);
  return true;
}

// �`��
void EnemyManager::Draw(
    actor::DifferedRenderingRenderCommandList* render_command_list) {
  for (i32 i = 0; i < enemys_.size(); i++) {
    render_command_list->Push(enemys_[i].get());
  }
  if (boss_ != nullptr) {
    render_command_list->Push(boss_.get());
  }
}

// �G�̍s������
void EnemyManager::EnemyAction(search::SearchManager* search_manaegr) {
  //�{�X�̍s��index�͓G�̔z��T�C�Y�ƈ�v�����Ƃ�
  const bool is_index_is_boss = enemys_.size() == action_enemy_index_;
  //�z��͈̔͊O���`�F�b�N
  const bool is_out_of_range =
      action_enemy_index_ < 0 || enemys_.size() <= action_enemy_index_;

  //�͈͊O�Ń{�X�s��index�łȂ��Ƃ��͏I��������
  if (is_out_of_range && !is_index_is_boss) {
    return;
  }

  if (is_index_is_boss) {
    //�{�X�����݂��Ȃ��Ƃ��͂���index�͖����Ȃ̂ŏ��������Ȃ�
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

    {  //�T���Ǘ�������ꍇ
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

// �G��ǉ�
void EnemyManager::AddEnemy(const EnemyActor::InitializeParameter& paramater) {
  if (enemy_max_count_ <= enemys_.size()) {
    return;
  }

  auto enemy = std::make_unique<Enemy>();
  enemy->Init(actor_mediator_, paramater);
  enemys_.emplace_back(std::move(enemy));
}

// �{�X��ǉ�
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

// �G���{�X�ɏW��������
bool EnemyManager::AbsorpEnemies() {
  if (boss_ == nullptr) return false;
  bool isAbsorp = false;
  for (i32 i = 0; i < enemys_.size(); i++) {
    //�p�[�e�B�N���̋����L�����𓭂�����
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

// �폜����
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

// �G���폜
void EnemyManager::Destroy(i32 index) {
  enemys_[index]->Remove();
  enemys_.erase(enemys_.begin() + index);
  if ((0 <= action_enemy_index_) && (index <= action_enemy_index_)) {
    action_enemy_index_--;
  }
}

// �{�X���폜
void EnemyManager::DestroyBoss() {
  if (boss_ == nullptr) return;

  if (-10 <= boss_->GetPosition().y) {
    return;
  }
  boss_->Remove();
  boss_.reset();
  is_game_clear_ = true;
}

// �|�W�V�����ݒ�
void EnemyManager::SetPosition(Enemy* enemy) {
  auto x = game::GameDevice::GetInstance()->GetRandom().Range(-1.0f, 1.0f);
  auto z = game::GameDevice::GetInstance()->GetRandom().Range(-1.0f, 1.0f);
  math::Vector3 position(x, 0.0f, z);
  enemy->SetPosition(position);
}

// �Ō�̓G�̎擾
Enemy* EnemyManager::GetLastEnemy() const {
  return enemys_.at(enemys_.size() - 1).get();
}

// �G�̐����擾
i32 EnemyManager::GetEnemiesSize() const {
  return static_cast<i32>(enemys_.size());
}

// �G�̍s����������
void EnemyManager::ResetEnemyMove() {
  action_enemy_index_ = -1;
  move_timer_ = -1.0f;
}

// �e�G�̑��x���擾
std::vector<math::Vector3> EnemyManager::GetVelocities() {
  velocities_.resize(enemys_.size());
  for (i32 i = 0; i < velocities_.size(); i++) {
    velocities_[i] = enemys_[i]->GetVelocity();
  }

  return velocities_;
}

// �Ō�̓G�̈ړ��I��������擾
bool EnemyManager::LastEnemyMoveEnd() const {
  //�󂩂ǂ����`�F�b�N
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

  // �ړ����I����Ă�����ړ����������Z�b�g
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

// �v���C���[�ݒ�
void EnemyManager::SetPlayer(bullet::Collider* player_obb) {
  player_collider_ = player_obb;
}

// �G�̃|�C���^���X�g�擾
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
