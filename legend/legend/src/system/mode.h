#ifndef LEGEND_SYSTEM_MODE_H
#define LEGEND_SYSTEM_MODE_H

/**
 * @file mode.h
 * @brief
 */

namespace legend {
namespace system {

/**
 * @brief �Q�[�����[�h��
 */
enum class Mode {
  PLAYER_MOVE_READY,  //! �v���C���[�̈ړ�����
  PLAYER_MOVING,      //! �v���C���[�̈ړ���
  PLAYER_MOVE_END,    //! �v���C���[�̈ړ��I��
  PLAYER_SKILL_AFTER_MOVED,  //! �v���C���[�̈ړ��I����X�L����������Ak
  PLAYER_END_ADD_SKILL,  //! �v���C���[�^�[����̃X�L���������o
  TO_ENEMY_TURN_,        //! �G�^�[���ɂ���ύX��
  ENEMY_MOVING,          //! �G�̈ړ���
  ENEMY_MOVE_END,        //! �G�̈ړ��I��
  ENEMY_END_ADD_SKILL,   //! �G�^�[����̃X�L���������o
  ENEMY_GENERATE,        //! �G�̐���
  ENEMY_PRODUCTION,      //! �G�̏o�����o��
  BOSS_PRODUCTION,       //! �{�X�̏o�����o��
  TO_PLAYER_TURN_,       //! �v���C���[�̃^�[���ɂ��鉉�o��
};

}  // namespace system
}  // namespace legend

#endif  //! LEGEND_SYSTEM_MODE_H
