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
  PLAYER_MOVE_READY,  //! プレイヤーの移動準備
  PLAYER_MOVING,      //! プレイヤーの移動中
  PLAYER_MOVE_END,    //! プレイヤーの移動終了
  PLAYER_SKILL_AFTER_MOVED,  //! プレイヤーの移動終了後スキル発動処理Ak
  PLAYER_END_ADD_SKILL,  //! プレイヤーターン後のスキル装備演出
  TO_ENEMY_TURN_,        //! 敵ターンにする変更中
  ENEMY_MOVING,          //! 敵の移動中
  ENEMY_MOVE_END,        //! 敵の移動終了
  ENEMY_END_ADD_SKILL,   //! 敵ターン後のスキル装備演出
  ENEMY_GENERATE,        //! 敵の生成
  ENEMY_PRODUCTION,      //! 敵の出現演出中
  BOSS_PRODUCTION,       //! ボスの出現演出中
  TO_PLAYER_TURN_,       //! プレイヤーのターンにする演出中
};

}  // namespace system
}  // namespace legend

#endif  //! LEGEND_SYSTEM_MODE_H
