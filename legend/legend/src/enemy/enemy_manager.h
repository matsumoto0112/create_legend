#ifndef LEGEND_ENEMY_ENEMY_MANAGER_H_
#define LEGEND_ENEMY_ENEMY_MANAGER_H_

#include <vector>
#include "src/enemy/enemy.h"

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
  void Add();
  /**
  * @brief �G���폜
  */
  void Destroy(i32 index);

  void SetPosition(Enemy* enemy);

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