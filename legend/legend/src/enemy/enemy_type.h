#ifndef LEGEND_ENEMY_MOVE_TYPE_H_
#define LEGEND_ENEMY_MOVE_TYPE_H_

/**
 * @file enemy_move_type.h
 * @brief �ړ�����
 */

namespace legend {
namespace enemy {
namespace enemy_type {
/**
 * @enum Enum
 * @brief �L�[�ԍ�
 */
enum MoveType : i32 {
  Straight = 0,  // ����
  Detour = 1,    // �I��
  Move_Type_End,		 // �z��̍Ō�
};
/**
 * @enum Enum
 * @brief �L�[�ԍ�
 */
enum HitType : i32 {
  Stop = 0,		 // ��~
  Rush = 1,		 // �ːi
  Bound = 2,     // ���e
  Hit_Type_End,      // �z��̍Ō�
};

}  // namespace move_type
}  // namespace enemy
}  // namespace legend

#endif  // !LEGEND_ENEMY_MOVE_TYPE_H_