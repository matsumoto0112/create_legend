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
  using InputCode = input_code::Enum;    //!< ���̓R�[�h
  using JoyCode = joy_code::Enum;        //!< ���̓R�[�h
  using KeyCode = key_code::Enum;        //!< ���̓R�[�h
  using MouseCode = mouse_button::Enum;  //!< ���̓R�[�h

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
   * @return ���͂��ꂽ��TRUE
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
  /**
   * @brief ���͒T�m�F�L�[�{�[�h(Down)
   * @param code  ���̓R�[�h
   * @param i_key ���̓L�[�{�[�h
   * @return ���͂��ꂽ��TRUE
   */
  bool Down(KeyCode code, Keyboard* i_key) const {
    return (i_key->GetKeyDown(code));
  }
  /**
   * @brief ���͒T�m�F�}�E�X(Down)
   * @param code     ���̓R�[�h
   * @param i_mouse  ���̓}�E�X
   * @return ���͂��ꂽ��TRUE
   */
  bool Down(MouseCode code, Mouse* i_mouse) const {
    return (i_mouse->GetMouseDown(code));
  }
  /**
   * @brief ���͒T�m�F�L�[�{�[�h(Down)
   * @param code      ���̓R�[�h
   * @param i_gamePad ���̓Q�[���p�b�h
   * @return ���͂��ꂽ��TRUE
   */
  bool Down(JoyCode code, GamePad* i_gamePad) const {
    return (i_gamePad->GetButtonDown(code));
  }

  /**
   * @brief ���͒T�m�F�L�[�{�[�h(State)
   * @param code  ���̓R�[�h
   * @param i_key ���̓L�[�{�[�h
   * @return ���͂��Ă�����TRUE
   */
  bool State(KeyCode code, Keyboard* i_key) const {
    return (i_key->GetKey(code));
  }
  /**
   * @brief ���͒T�m�F�}�E�X(State)
   * @param code     ���̓R�[�h
   * @param i_mouse  ���̓}�E�X
   * @return ���͂��Ă�����TRUE
   */
  bool State(MouseCode code, Mouse* i_mouse) const {
    return (i_mouse->GetMouse(code));
  }
  /**
   * @brief ���͒T�m�F�L�[�{�[�h(State)
   * @param code      ���̓R�[�h
   * @param i_gamePad ���̓Q�[���p�b�h
   * @return ���͂��Ă�����TRUE
   */
  bool State(JoyCode code, GamePad* i_gamePad) const {
    return (i_gamePad->GetButton(code));
  }

  /**
   * @brief ���͒T�m�F�L�[�{�[�h(Down)
   * @param code  ���̓R�[�h
   * @param i_key ���̓L�[�{�[�h
   * @return ��������TRUE
   */
  bool Up(KeyCode code, Keyboard* i_key) const {
    return (i_key->GetKeyUp(code));
  }
  /**
   * @brief ���͒T�m�F�}�E�X(Down)
   * @param code     ���̓R�[�h
   * @param i_mouse  ���̓}�E�X
   * @return ��������TRUE
   */
  bool Up(MouseCode code, Mouse* i_mouse) const {
    return (i_mouse->GetMouseUp(code));
  }
  /**
   * @brief ���͒T�m�F�L�[�{�[�h(Down)
   * @param code      ���̓R�[�h
   * @param i_gamePad ���̓Q�[���p�b�h
   * @return ��������TRUE
   */
  bool Up(JoyCode code, GamePad* i_gamePad) const {
    return (i_gamePad->GetButtonUp(code));
  }

  /**
   * @brief ���͒T�m�F�L�[�{�[�h(OR_Down)
   * @param arrays ���̓R�[�h
   * @param i_key  ���̓L�[�{�[�h
   * @return ��ł����͂��ꂽ��TRUE
   */
  bool OR(std::vector<KeyCode> arrays, Keyboard* i_key) const {
    for (int i = 0; i < arrays.size(); i++) {
      if (Down(arrays[i], i_key)) {
        return true;
      }
    }
    return false;
  }
  /**
   * @brief ���͒T�m�F�}�E�X(OR_Down)
   * @param arrays  ���̓R�[�h
   * @param i_mouse ���̓}�E�X
   * @return ���͂��ꂽ��TRUE
   */
  bool OR(std::vector<MouseCode> arrays, Mouse* i_mouse) const {
    for (int i = 0; i < arrays.size(); i++) {
      if (Down(arrays[i], i_mouse)) {
        return true;
      }
    }
    return false;
  }
  /**
   * @brief ���͒T�m�F�Q�[���p�b�h(OR_Down)
   * @param arrays    ���̓R�[�h
   * @param i_gamePad ���̓Q�[���p�b�h
   * @return ��ł����͂��ꂽ��TRUE
   */
  bool OR(std::vector<JoyCode> arrays, GamePad* i_gamePad) const {
    for (int i = 0; i < arrays.size(); i++) {
      if (Down(arrays[i], i_gamePad)) {
        return true;
      }
    }
    return false;
  }

  /**
   * @brief ���͒T�m�F�L�[�{�[�h(AND)
   * @param arrays ���̓R�[�h
   * @param i_key  ���̓L�[�{�[�h
   * @return ���ׂē��͂��ꂽ��TRUE
   */
  bool AND(std::vector<KeyCode> arrays, Keyboard* i_key) const {
    if (arrays.size() <= 0) {
      return false;
    }
    for (int i = 0; i < arrays.size(); i++) {
      if (!State(arrays[i], i_key)) {
        return false;
      }
    }
    return true;
  }
  /**
   * @brief ���͒T�m�F�}�E�X(AND)
   * @param arrays  ���̓R�[�h
   * @param i_mouse ���̓}�E�X
   * @return ���ׂē��͂��ꂽ��TRUE
   */
  bool AND(std::vector<MouseCode> arrays, Mouse* i_mouse) const {
    if (arrays.size() <= 0) {
      return false;
    }
    for (int i = 0; i < arrays.size(); i++) {
      if (!State(arrays[i], i_mouse)) {
        return false;
      }
    }
    return true;
  }
  /**
   * @brief ���͒T�m�F�Q�[���p�b�h(AND)
   * @param arrays    ���̓R�[�h
   * @param i_gamePad ���̓Q�[���p�b�h
   * @return ���ׂē��͂��ꂽ��TRUE
   */
  bool AND(std::vector<JoyCode> arrays, GamePad* i_gamePad) const {
    if (arrays.size() <= 0) {
      return false;
    }
    for (int i = 0; i < arrays.size(); i++) {
      if (!State(arrays[i], i_gamePad)) {
        return false;
      }
    }
    return true;
  }

 private:
  std::unordered_map<i32, std::function<bool(Keyboard*, Mouse*, GamePad*)>>
      input_command_ = {
          {InputCode::End,  //!< �I���R�}���h
           [&](Keyboard* i_key, Mouse* i_mouse, GamePad* i_gamePad) {
             return (AND({KeyCode::Escape, KeyCode::Enter}, i_key) ||
                     AND({JoyCode::START, JoyCode::SELECT}, i_gamePad));
           }},
          {InputCode::Decide,  //!< ����R�}���h
           [&](Keyboard* i_key, Mouse* i_mouse, GamePad* i_gamePad) {
             return (Down(KeyCode::Enter, i_key) ||
                     Down(JoyCode::A, i_gamePad));
           }},
          {InputCode::Pause,  //!< �ҋ@�R�}���h
           [&](Keyboard* i_key, Mouse* i_mouse, GamePad* i_gamePad) {
             return (Down(KeyCode::Escape, i_key) ||
                     Down(JoyCode::START, i_gamePad));
           }},
          {InputCode::CAMERA_CHANGE,  //!< �J�����؂�ւ�
           [&](Keyboard* i_key, Mouse* i_mouse, GamePad* i_gamePad) {
             return (Down(KeyCode::Space, i_key) ||
                     Down(JoyCode::Y, i_gamePad));
           }},
          {InputCode::CANCEL,  //!< �L�����Z���R�}���h
           [&](Keyboard* i_key, Mouse* i_mouse, GamePad* i_gamePad) {
             return (Down(JoyCode::B, i_gamePad));
           }},
  };  //!< ���͔���
};
}  // namespace input
}  // namespace legend

#endif  // !LEGEND_INPUT_INPUT_COMMAND_H_