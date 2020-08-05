#ifndef LEGEND_SYSTEM_TURN_SYSTEM_H_
#define LEGEND_SYSTEM_TURN_SYSTEM_H_

namespace legend {
namespace system {

/**
 * @brief ターンの列挙
 */
enum class Turn {
  PLAYER_TURN = 0,
  ENEMY_TURN,
};

/**
 * @class TurnSystem
 * @brief ターン数管理クラス
 */
class TurnSystem {
 public:
  /**
   * @brief コンストラクタ
   */
  TurnSystem();
  /**
   * @brief デストラクタ
   */
  ~TurnSystem();
  /**
   * @brief ターン数の増加
   */
  void AddCurrentTurn();
  /**
   * @brief 現在のターン数を取得
   */
  i32 GetCurrentTurn();

 private:
  i32 current_turn_;
};

}  // namespace system
}  // namespace legend

#endif  //! LEGEND_SYSTEM_TURN_SYSTEM_H_
