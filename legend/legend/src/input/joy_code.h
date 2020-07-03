#ifndef LEGEND_INPUT_JOY_CODE_H_
#define LEGEND_INPUT_JOY_CODE_H_

/**
 * @file joyCode.h
 * @brief ゲームパッド定義
 */

namespace legend {
namespace input {
namespace joy_code {
/**
 * @enum Enum
 * @brief キー番号
 */
enum Enum : i32 {
  A = 1,                 //!< Aボタン
  B = 2,                 //!< Bボタン
  X = 4,                 //!< Xボタン
  Y = 8,                 //!< Yボタン
  LB = 16,               //!< LBボタン
  RB = 32,               //!< RBボタン
  SELECT = 64,           //!< SELECTボタン
  START = 128,           //!< STARTボタン
  LEFT_STICK = 256,      //!< 左スティックボタン
  RIGHT_STICK = 512,     //!< 右スティックボタン
};
}  // namespace joy_code
}  // namespace input
}  // namespace legend

#endif  // !LEGEND_INPUT_JOY_CODE_H_