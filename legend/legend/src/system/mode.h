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
  PLAYER_MOVE_READY,         //! �v���C���[�̈ړ�����
  PLAYER_MOVING,             //! �v���C���[�̈ړ���
  PLAYER_MOVE_END,           //! �v���C���[�̈ړ��I��
  PLAYER_SKILL_AFTER_MOVED,  //! �v���C���[�̈ړ��I����X�L����������Ak
  ENEMY_MOVING,              //! �G�̈ړ���
  ENEMY_MOVE_END,            //! �G�̈ړ��I��
};

}  // namespace system
}  // namespace legend

#endif  //! LEGEND_SYSTEM_MODE_H
