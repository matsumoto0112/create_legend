#ifndef LEGEND_SYSTEM_MODE_H
#define LEGEND_SYSTEM_MODE_H

/**
 * @file mode.h
 * @brief
 */

namespace legend {
namespace system {

/**
 * @brief ゲームモード列挙
 */
enum class Mode {
  PLAYER_MOVE_READY,         //! プレイヤーの移動準備
  PLAYER_MOVING,             //! プレイヤーの移動中
  PLAYER_MOVE_END,           //! プレイヤーの移動終了
  PLAYER_SKILL_AFTER_MOVED,  //! プレイヤーの移動終了後スキル発動処理Ak
  ENEMY_MOVING,              //! 敵の移動中
  ENEMY_MOVE_END,            //! 敵の移動終了
};

}  // namespace system
}  // namespace legend

#endif  //! LEGEND_SYSTEM_MODE_H
