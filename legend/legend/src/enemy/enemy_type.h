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
 * @enum MoveType
 * @brief �ړ��ԍ�
 */
enum MoveType : i32 {
  Straight = 0,  // ����
  Detour = 1,    // �I��
  Move_Type_End,		 // �z��̍Ō�
};
/**
 * @enum HitType
 * @brief �Փ˔ԍ�
 */
enum HitType : i32 {
  Stop = 0,		 // ��~
  Rush = 1,		 // �ːi
  Bound = 2,     // ���e
  Hit_Type_End,      // �z��̍Ō�
};
/**
 * @enum EffectType
 * @brief ���ʔԍ�
 */
enum EffectType : i32 {
  None = 0,      // �ʏ�
  Rotate = 1,       // ��]
  Escape = 2,       // ����
  Effect_Type_End,  // �z��̍Ō�
};

}  // namespace move_type
}  // namespace enemy
}  // namespace legend

#endif  // !LEGEND_ENEMY_MOVE_TYPE_H_