#ifndef LEGEND_INPUT_INPUT_CODE_H_
#define LEGEND_INPUT_INPUT_CODE_H_

/**
 * @file input_code.h
 * @brief 入力定義
 */

namespace legend {
namespace input {
namespace input_code {
/**
 * @enum Enum
 * @brief キー番号
 */
enum Enum : i32 {
  End = -1,       //!< 終了コマンド
  Decide = 0,     //!< 決定コマンド
  Pause = 1,      //!< 待機コマンド
  CANCEL,         //! キャンセルボタン
  CAMERA_CHANGE,  //!< カメラ切り替え
};
}  // namespace input_code
}  // namespace input
}  // namespace legend

#endif  // !LEGEND_INPUT_INPUT_CODE_H_