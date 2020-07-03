#ifndef LEGEND_INPUT_INPUT_COMMAND_H_
#define LEGEND_INPUT_INPUT_COMMAND_H_

/**
 * @file input_command.h
 * @brief 入力定義
 */

#include <functional>
#include <unordered_map>

#include "src/input/game_pad.h"
#include "src/input/input_code.h"
#include "src/input/keyboard.h"
#include "src/input/mouse.h"

namespace legend {
namespace input {

/**
 * @class InputCommand
 * @brief 入力コマンド
 */
class InputCommand {
  using InputCode = input_code::Enum; //!< 入力コード

 public:
  /**
   * @brief コンストラクタ
   */
  InputCommand() {}
  /**
   * @brief デスクトラクタ
   */
  ~InputCommand() {}

  /**
   * @brief コンストラクタ
   * @param code            入力コード
   * @param input_keyboard_ 入力キーボード
   * @param input_mouse_    入力マウス
   * @param input_game_pad_ 入力ゲームパッド
   */
  bool GetCommand(InputCode code, Keyboard* input_keyboard_,
                  Mouse* input_mouse_, GamePad* input_game_pad_) const {
    if (input_command_.count(code)) {
      auto value = input_command_.at(code);
      return value(input_keyboard_, input_mouse_, input_game_pad_);
    }
    return false;
  }

 private:
  std::unordered_map<i32, std::function<bool(Keyboard*, Mouse*, GamePad*)>>
      input_command_ = {
          {InputCode::End,//!< 終了コマンド(keyboard_Escape_単押し || (GamePad_Start_長押し && GamePad_Select_長押し))
           [&](Keyboard* input_keyboard_, Mouse* input_mouse_,
               GamePad* input_game_pad_) {
             return (input_keyboard_->GetKeyDown(key_code::Escape) ||
                     (input_game_pad_->GetButton(joy_code::START) &&
                      input_game_pad_->GetButton(joy_code::SELECT)));
           }},
          {InputCode::Decide,//!< 決定コマンド(keyboard_Enter_単押し || GamePad_A_単押し)
           [&](Keyboard* input_keyboard_, Mouse* input_mouse_,
               GamePad* input_game_pad_) {
             return (input_keyboard_->GetKeyDown(key_code::Enter) ||
                     input_game_pad_->GetButtonDown(joy_code::A));
           }},
  };  //!< 入力判定
};
}  // namespace input
}  // namespace legend

#endif  // !LEGEND_INPUT_INPUT_COMMAND_H_