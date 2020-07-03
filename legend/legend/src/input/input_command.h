#ifndef LEGEND_INPUT_INPUT_COMMAND_H_
#define LEGEND_INPUT_INPUT_COMMAND_H_

/**
 * @file input_command.h
 * @brief “ü—Í’è‹`
 */

#include <functional>
#include <unordered_map>

#include "src/input/game_pad.h"
#include "src/input/input_code.h"
#include "src/input/keyboard.h"
#include "src/input/mouse.h"

namespace legend {
namespace input {

class InputCommand {
  using InputCode = input_code::Enum;

 public:
  InputCommand() {}
  ~InputCommand() {}

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
          {InputCode::End,
           [&](Keyboard* input_keyboard_, Mouse* input_mouse_,
               GamePad* input_game_pad_) {
             return (input_keyboard_->GetKeyDown(key_code::Escape) ||
                     (input_game_pad_->GetButton(joy_code::START) &&
                      input_game_pad_->GetButton(joy_code::SELECT)));
           }},
          {InputCode::Decide,
           [&](Keyboard* input_keyboard_, Mouse* input_mouse_,
               GamePad* input_game_pad_) {
             return (input_keyboard_->GetKeyDown(key_code::Enter) ||
                     input_game_pad_->GetButtonDown(joy_code::A));
           }},
  };  //!< “ü—Í”»’è
};
}  // namespace input
}  // namespace legend

#endif  // !LEGEND_INPUT_INPUT_COMMAND_H_