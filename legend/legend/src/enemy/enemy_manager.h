#ifndef LEGEND_ENEMY_ENEMY_MANAGER_H_
#define LEGEND_ENEMY_ENEMY_MANAGER_H_

#include <vector>

#include "src/enemy/enemy.h"
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
  bool Initilaize(system::PhysicsField* physics_field = nullptr);
  /**
   * @brief �X�V
   */
  bool Update(system::PhysicsField* physics_field = nullptr);
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
  void Add(system::PhysicsField* physics_field = nullptr);
  /**
   * @brief �G���폜
   */
  void Destroy(i32 index, system::PhysicsField* physics_field = nullptr);

  void SetPosition(Enemy* enemy);

  Enemy* GetLastEnemy() const;
  void SetPosition(system::PhysicsField* physics_field);
  float GetEnemyPower() const;
  i32 GetEnemiesSize() const;
  void SetVelocity(system::PhysicsField* physics_field, i32 index_num);
  std::vector<math::Vector3> GetVelocities() const;
  bool LastEnemyMoveEnd() const;

 private:
  std::vector<std::unique_ptr<Enemy>> enemys_;
  i32 action_enemy_index_ = -1;
  i32 enemy_max_count_ = 50;
  float move_timer_ = 0.0f;
  const float move_time_ = 0.25f;
};
}  // namespace enemy
}  // namespace legend

#endif LEGEND_ENEMY_ENEMY_MANAGER_H_