#ifndef LEGEND_SYSTEM_TURN_SYSTEM_H_
#define LEGEND_SYSTEM_TURN_SYSTEM_H_

namespace legend {
namespace system {

/**
 * @brief �^�[���̗�
 */
enum class Turn {
  PLAYER_TURN = 0,
  ENEMY_TURN,
};

/**
 * @class TurnSystem
 * @brief �^�[�����Ǘ��N���X
 */
class TurnSystem {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  TurnSystem();
  /**
   * @brief �f�X�g���N�^
   */
  ~TurnSystem();
  /**
   * @brief �^�[�����̑���
   */
  void AddCurrentTurn();
  /**
   * @brief ���݂̃^�[�������擾
   */
  i32 GetCurrentTurn();

 private:
  i32 current_turn_;
};

}  // namespace system
}  // namespace legend

#endif  //! LEGEND_SYSTEM_TURN_SYSTEM_H_
