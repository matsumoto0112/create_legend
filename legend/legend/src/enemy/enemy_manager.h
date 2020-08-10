#ifndef LEGEND_ENEMY_ENEMY_MANAGER_H_
#define LEGEND_ENEMY_ENEMY_MANAGER_H_

#include <vector>

#include "src/enemy/enemy.h"
#include "src/player/player.h"
#include "src/system/physics_field.h"

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
  bool Initilaize();
  /**
   * @brief �X�V
   */
  bool Update();
  /**
   * @brief �`��
   */
  void Draw();
  /**
   * @brief �G�̍s������
   */
  void EnemyAction();
  /**
   * @brief �G��ǉ�
   */
  void Add(const Enemy::InitializeParameter& paramater);
  /**
   * @brief �G���폜
   */
  void Destroy(i32 index);

  void SetPosition(Enemy* enemy);

  /**
   * @brief �Ō�̓G���擾
   */
  Enemy* GetLastEnemy() const;
  /**
   * @brief obb�̍��W����ɍ��W�X�V
   */
  void SetPosition();
  /**
   * @brief �G�̐����擾
   */
  i32 GetEnemiesSize() const;
  /**
   * @brief obb�̑��x����ɑ��x�X�V
   */
  void SetVelocity();
  /**
   * @brief �e�G�̑��x���擾
   */
  std::vector<math::Vector3> GetVelocities();
  /**
   * @brief �Ō�̓G�̈ړ��I������̎擾
   */
  bool LastEnemyMoveEnd() const;

  void DebugDraw(directx::device::CommandList& command_list);

 private:
  std::vector<std::unique_ptr<Enemy>> enemys_;
  i32 action_enemy_index_ = -1;
  i32 enemy_max_count_ = 50;
  float move_timer_ = 0.0f;
  const float move_time_ = 0.25f;

  //! �e�G�̑��x�i�[���X�g
  std::vector<math::Vector3> velocities_;
  std::unique_ptr<player::Player> player_;
  std::unique_ptr<system::PhysicsField> physics_field_;
};
}  // namespace enemy
}  // namespace legend

#endif LEGEND_ENEMY_ENEMY_MANAGER_H_