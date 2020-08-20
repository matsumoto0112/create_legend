#ifndef LEGEND_ENEMY_ENEMY_MANAGER_H_
#define LEGEND_ENEMY_ENEMY_MANAGER_H_

#include <vector>

#include "src/actor/actor_mediator.h"
#include "src/enemy/boss.h"
#include "src/enemy/enemy.h"
#include "src/player/player.h"
#include "src/search/search_manager.h"

namespace legend {
namespace enemy {
/**
 * @class EnemyManager
 * @brief �G�l�~�[�Ǘ��N���X
 */
class EnemyManager {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  EnemyManager();
  /**
   * @�f�X�N�g���N�^
   */
  ~EnemyManager();
  /**
   * @brief ������
   */
  bool Initilaize(actor::IActorMediator* mediator);
  /**
   * @brief �X�V
   */
  bool Update(search::SearchManager* search_manaegr);
  /**
   * @brief �`��
   */
  void Draw();
  /**
   * @brief �G�̍s������
   */
  void EnemyAction(search::SearchManager* search_manaegr);
  /**
   * @brief �G��ǉ�
   */
  void Add(const Enemy::InitializeParameter& paramater);
  /**
   * @brief �{�X��ǉ�
   */
  void Add(const Boss::InitializeParameter& paramater);
  ///**
  // * @brief �G���폜
  // */
  // void Destroy(i32 index, system::PhysicsField& physics_field);
  /**
   * @brief �G���폜
   */
  // void RandomDestroy(system::PhysicsField& physics_field);

  void SetPosition(Enemy* enemy);

  /**
   * @brief �Ō�̓G���擾
   */
  Enemy* GetLastEnemy() const;
  ///**
  // * @brief obb�̍��W����ɍ��W�X�V
  // */
  // void SetPosition(system::PhysicsField& physics_field);
  /**
   * @brief �G�̐����擾
   */
  i32 GetEnemiesSize() const;
  ///**
  // * @brief obb�̑��x����ɑ��x�X�V
  // */
  // void SetVelocity(system::PhysicsField& physics_field);
  /**
   * @brief �G�̍s����������
   */
  void ResetEnemyMove();
  /**
   * @brief �e�G�̑��x���擾
   */
  std::vector<math::Vector3> GetVelocities();
  /**
   * @brief �Ō�̓G�̈ړ��I������̎擾
   */
  bool LastEnemyMoveEnd() const;

  void SetPlayer(bullet::Collider* player_obb);

  void DebugDraw(directx::device::CommandList& command_list);

  std::vector<Enemy*> GetEnemyPointers() const;

 private:
  std::vector<std::unique_ptr<Enemy>> enemys_;
  std::unique_ptr<Boss> boss_;
  i32 action_enemy_index_ = -1;
  i32 enemy_max_count_ = 50;
  bool game_clear_flag_ = false;
  float move_timer_ = 0.0f;
  const float move_time_ = 0.5f;

  //! �e�G�̑��x�i�[���X�g
  std::vector<math::Vector3> velocities_;
  bullet::Collider* player_collider_;

  actor::IActorMediator* actor_mediator_;
};
}  // namespace enemy
}  // namespace legend

#endif LEGEND_ENEMY_ENEMY_MANAGER_H_