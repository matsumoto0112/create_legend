#ifndef LEGEND_INPUT_INPUT_COMMAND_H_
#define LEGEND_INPUT_INPUT_COMMAND_H_

/**
 * @file input_command.h
 * @brief ���͒�`
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
 * @brief ���̓R�}���h
 */
class InputCommand {
  using InputCode = input_code::Enum; //!< ���̓R�[�h

 public:
  /**
   * @brief �R���X�g���N�^
   */
  InputCommand() {}
  /**
   * @brief �f�X�N�g���N�^
   */
  ~InputCommand() {}

  /**
   * @brief �R���X�g���N�^
   * @param code            ���̓R�[�h
   * @param input_keyboard_ ���̓L�[�{�[�h
   * @param input_mouse_    ���̓}�E�X
   * @param input_game_pad_ ���̓Q�[���p�b�h
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
          {InputCode::End,//!< �I���R�}���h(keyboard_Escape_�P���� || (GamePad_Start_������ && GamePad_Select_������))
           [&](Keyboard* input_keyboard_, Mouse* input_mouse_,
               GamePad* input_game_pad_) {
             return (input_keyboard_->GetKeyDown(key_code::Escape) ||
                     (input_game_pad_->GetButton(joy_code::START) &&
                      input_game_pad_->GetButton(joy_code::SELECT)));
           }},
          {InputCode::Decide,//!< ����R�}���h(keyboard_Enter_�P���� || GamePad_A_�P����)
           [&](Keyboard* input_keyboard_, Mouse* input_mouse_,
               GamePad* input_game_pad_) {
             return (input_keyboard_->GetKeyDown(key_code::Enter) ||
                     input_game_pad_->GetButtonDown(joy_code::A));
           }},
  };  //!< ���͔���
};
}  // namespace input
}  // namespace legend

#endif  // !LEGEND_INPUT_INPUT_COMMAND_H_