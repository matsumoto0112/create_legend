#ifndef LEGEND_ENEMY_MOVE_TYPE_H_
#define LEGEND_ENEMY_MOVE_TYPE_H_

/**
 * @file enemy_move_type.h
 * @brief ˆÚ“®”»•Ê
 */

namespace legend {
namespace enemy {
namespace enemy_type {
/**
 * @enum MoveType
 * @brief ˆÚ“®”Ô†
 */
enum MoveType : i32 {
  Straight = 0,  // ’¼ü
  Detour = 1,    // ‰I‰ñ
  Move_Type_End,		 // ”z—ñ‚ÌÅŒã
};
/**
 * @enum HitType
 * @brief Õ“Ë”Ô†
 */
enum HitType : i32 {
  Stop = 0,		 // ’â~
  Rush = 1,		 // “Ëi
  Bound = 2,     // ’µ’e
  Hit_Type_End,      // ”z—ñ‚ÌÅŒã
};
/**
 * @enum EffectType
 * @brief Œø‰Ê”Ô†
 */
enum EffectType : i32 {
  None = 0,      // ’Êí
  Rotate = 1,       // ‰ñ“]
  Escape = 2,       // “¦‘–
  Effect_Type_End,  // ”z—ñ‚ÌÅŒã
};

}  // namespace move_type
}  // namespace enemy
}  // namespace legend

#endif  // !LEGEND_ENEMY_MOVE_TYPE_H_