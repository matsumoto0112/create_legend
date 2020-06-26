/**
 * @file joyCode.h
 * @brief ゲームパッド定義
 */

#pragma once

namespace Framework::Input {
namespace joyCode {
/**
 * @enum Enum
 * @brief キー番号
 */
enum Enum : int {
  A = 1,
  B = 2,
  X = 4,
  Y = 8,
  LB = 16,
  RB = 32,
  SELECT = 64,
  START = 128,
  LEFT_STICK = 256,
  RIGHT_STICK = 512,
};
}  // namespace joyCode
}  // namespace input
