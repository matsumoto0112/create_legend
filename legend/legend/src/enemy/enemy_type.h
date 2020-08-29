#ifndef LEGEND_ENEMY_MOVE_TYPE_H_
#define LEGEND_ENEMY_MOVE_TYPE_H_

/**
 * @file enemy_move_type.h
 * @brief 移動判別
 */

namespace legend {
namespace enemy {
namespace enemy_type {
/**
 * @enum MoveType
 * @brief 移動番号
 */
enum MoveType : i32 {
  Straight = 0,  // 直線
  Detour = 1,    // 迂回
  Move_Type_End,		 // 配列の最後
};
/**
 * @enum HitType
 * @brief 衝突番号
 */
enum HitType : i32 {
  Stop = 0,		 // 停止
  Rush = 1,		 // 突進
  Bound = 2,     // 跳弾
  Hit_Type_End,      // 配列の最後
};
/**
 * @enum EffectType
 * @brief 効果番号
 */
enum EffectType : i32 {
  None = 0,      // 通常
  Rotate = 1,       // 回転
  Effect_Type_End,  // 配列の最後
};

}  // namespace move_type
}  // namespace enemy
}  // namespace legend

#endif  // !LEGEND_ENEMY_MOVE_TYPE_H_