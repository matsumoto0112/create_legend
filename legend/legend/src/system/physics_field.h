#ifndef LEGEND_SYSTEM_PHYSICS_FIELD_H_
#define LEGEND_SYSTEM_PHYSICS_FIELD_H_

#include "src/object/desk.h"
#include "src/player/player.h"
#include "src/system/turn_system.h"

namespace legend {
namespace system {

/**
 * @class PhysicsField
 * @brief �t�B�[���h�̊Ǘ��N���X
 */
class PhysicsField {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  PhysicsField();
  /**
   * @brief �f�X�g���N�^
   */
  ~PhysicsField();
  /**
   * @brief �v���C���[�̏�����
   */
  bool PlayerInit(player::Player::InitializeParameter parameter);
  /**
   * @brief �G�l�~�[�̏�����
   */
  // bool EnemyInit(�G�l�~�[::InitializeParameter parameter);
  /**
   * @brief ���̏�����
   */
  bool DeskInit(object::Desk::InitializeParameter parameter);
  /**
   * @brief �X�V
   */
  bool Update();
  /**
   * @brief �`��
   */
  void Draw();
  /**
   * @brief �G�l�~�[�����蔻��̓o�^
   */
  // void AddEnemy(�G�l�~�[ enemy);
  /**
   * @brief ���̂����蔻��̓o�^
   */
  void AddDesk(object::Desk desk);

 private:
  Turn turn_;

  player::Player player_;
  // std::vector<�G�l�~�[> enemies_;
  std::vector<object::Desk> desks_;

  float gravity_;
};

}  // namespace system
}  // namespace legend

#endif  //! LEGEND_SYSTEM_PHYSICS_FIELD_H_
