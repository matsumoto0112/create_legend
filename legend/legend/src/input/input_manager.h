#ifndef LEGEND_INPUT_INPUT_MANAGER_H_
#define LEGEND_INPUT_INPUT_MANAGER_H_

/**
 * @file InputManager.h
 * @brief ゲームにおける入力全般を管理する
 */

#include <functional>
#include <unordered_map>

#include "src/input/keyboard.h"
#include "src/input/mouse.h"
#include "src/input/game_pad.h"
#include "src/input/input_command.h"

namespace legend {
namespace input {
/**
 * @class InputManager
 * @brief 入力を管理する
 */
class InputManager {
  using InputCode = input_code::Enum;

 public:
  /**
   * @brief コンストラクタ
   * @param hWnd ウィンドウハンドル
   */
  InputManager(HWND hWnd);
  /**
   * @brief デストラクタ
   */
  ~InputManager();
  /**
   * @brief 入力情報を更新する
   */
  void Update();
  /**
   * @brief キーボードを取得する
   */
  inline Keyboard* GetKeyboard() const { return input_keyboard_.get(); }
  /**
   * @brief マウスを取得する
   */
  inline Mouse* GetMouse() const { return input_mouse_.get(); }
  /**
   * @brief ゲームパッドを取得する
   */
  inline GamePad* GetGamepad() const { return input_game_pad_.get(); }
  /**
   * @brief ゲームパッドを取得する
   */
  bool GetCommand(InputCode code) const;

  /**
   * @brief 横軸移動入力取得
   */
  float GetHorizontal() const;
  /**
   * @brief 縦軸移動入力取得
   */
  float GetVertical() const;

 private:
  std::unique_ptr<Keyboard> input_keyboard_;  //!< キーボード
  std::unique_ptr<Mouse> input_mouse_;        //!< マウス
  std::unique_ptr<GamePad> input_game_pad_;   //!< ゲームパッド

  std::unique_ptr<InputCommand> input_command_;  //!< コマンド入力
};
}  // namespace input
}  // namespace legend

#endif  // !LEGEND_INPUT_INPUT_MANAGER_H_