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
  using InputCode = input_code::Enum;    //!< 入力コード
  using JoyCode = joy_code::Enum;        //!< 入力コード
  using KeyCode = key_code::Enum;        //!< 入力コード
  using MouseCode = mouse_button::Enum;  //!< 入力コード

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
   * @return 入力されたらTRUE
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
   * @brief 入力探知：キーボード(Down)
   * @param code  入力コード
   * @param i_key 入力キーボード
   * @return 入力されたらTRUE
   */
  bool Down(KeyCode code, Keyboard* i_key) const {
    return (i_key->GetKeyDown(code));
  }
  /**
   * @brief 入力探知：マウス(Down)
   * @param code     入力コード
   * @param i_mouse  入力マウス
   * @return 入力されたらTRUE
   */
  bool Down(MouseCode code, Mouse* i_mouse) const {
    return (i_mouse->GetMouseDown(code));
  }
  /**
   * @brief 入力探知：キーボード(Down)
   * @param code      入力コード
   * @param i_gamePad 入力ゲームパッド
   * @return 入力されたらTRUE
   */
  bool Down(JoyCode code, GamePad* i_gamePad) const {
    return (i_gamePad->GetButtonDown(code));
  }

  /**
   * @brief 入力探知：キーボード(State)
   * @param code  入力コード
   * @param i_key 入力キーボード
   * @return 入力していたらTRUE
   */
  bool State(KeyCode code, Keyboard* i_key) const {
    return (i_key->GetKey(code));
  }
  /**
   * @brief 入力探知：マウス(State)
   * @param code     入力コード
   * @param i_mouse  入力マウス
   * @return 入力していたらTRUE
   */
  bool State(MouseCode code, Mouse* i_mouse) const {
    return (i_mouse->GetMouse(code));
  }
  /**
   * @brief 入力探知：キーボード(State)
   * @param code      入力コード
   * @param i_gamePad 入力ゲームパッド
   * @return 入力していたらTRUE
   */
  bool State(JoyCode code, GamePad* i_gamePad) const {
    return (i_gamePad->GetButton(code));
  }

  /**
   * @brief 入力探知：キーボード(Down)
   * @param code  入力コード
   * @param i_key 入力キーボード
   * @return 離したらTRUE
   */
  bool Up(KeyCode code, Keyboard* i_key) const {
    return (i_key->GetKeyUp(code));
  }
  /**
   * @brief 入力探知：マウス(Down)
   * @param code     入力コード
   * @param i_mouse  入力マウス
   * @return 離したらTRUE
   */
  bool Up(MouseCode code, Mouse* i_mouse) const {
    return (i_mouse->GetMouseUp(code));
  }
  /**
   * @brief 入力探知：キーボード(Down)
   * @param code      入力コード
   * @param i_gamePad 入力ゲームパッド
   * @return 離したらTRUE
   */
  bool Up(JoyCode code, GamePad* i_gamePad) const {
    return (i_gamePad->GetButtonUp(code));
  }

  /**
   * @brief 入力探知：キーボード(OR_Down)
   * @param arrays 入力コード
   * @param i_key  入力キーボード
   * @return 一つでも入力されたらTRUE
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
   * @brief 入力探知：マウス(OR_Down)
   * @param arrays  入力コード
   * @param i_mouse 入力マウス
   * @return 入力されたらTRUE
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
   * @brief 入力探知：ゲームパッド(OR_Down)
   * @param arrays    入力コード
   * @param i_gamePad 入力ゲームパッド
   * @return 一つでも入力されたらTRUE
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
   * @brief 入力探知：キーボード(AND)
   * @param arrays 入力コード
   * @param i_key  入力キーボード
   * @return すべて入力されたらTRUE
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
   * @brief 入力探知：マウス(AND)
   * @param arrays  入力コード
   * @param i_mouse 入力マウス
   * @return すべて入力されたらTRUE
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
   * @brief 入力探知：ゲームパッド(AND)
   * @param arrays    入力コード
   * @param i_gamePad 入力ゲームパッド
   * @return すべて入力されたらTRUE
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
          {InputCode::End,  //!< 終了コマンド
           [&](Keyboard* i_key, Mouse* i_mouse, GamePad* i_gamePad) {
             return (AND({KeyCode::Escape, KeyCode::Enter}, i_key) ||
                     AND({JoyCode::START, JoyCode::SELECT}, i_gamePad));
           }},
          {InputCode::Decide,  //!< 決定コマンド
           [&](Keyboard* i_key, Mouse* i_mouse, GamePad* i_gamePad) {
             return (Down(KeyCode::Enter, i_key) ||
                     Down(JoyCode::A, i_gamePad));
           }},
          {InputCode::Pause,  //!< 待機コマンド
           [&](Keyboard* i_key, Mouse* i_mouse, GamePad* i_gamePad) {
             return (Down(KeyCode::Escape, i_key) ||
                     Down(JoyCode::START, i_gamePad));
           }},
          {InputCode::CAMERA_CHANGE,  //!< カメラ切り替え
           [&](Keyboard* i_key, Mouse* i_mouse, GamePad* i_gamePad) {
             return (Down(KeyCode::Space, i_key) ||
                     Down(JoyCode::Y, i_gamePad));
           }},
          {InputCode::CANCEL,  //!< キャンセルコマンド
           [&](Keyboard* i_key, Mouse* i_mouse, GamePad* i_gamePad) {
             return (Down(JoyCode::B, i_gamePad));
           }},
  };  //!< 入力判定
};
}  // namespace input
}  // namespace legend

#endif  // !LEGEND_INPUT_INPUT_COMMAND_H_