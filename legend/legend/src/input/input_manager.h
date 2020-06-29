#ifndef LEGEND_INPUT_INPUT_MANAGER_H_
#define LEGEND_INPUT_INPUT_MANAGER_H_

/**
 * @file InputManager.h
 * @brief ゲームにおける入力全般を管理する
 */

#include "src/input/game_pad.h"
#include "src/input/keyboard.h"
#include "src/input/mouse.h"

namespace legend {
namespace input {
/**
 * @class InputManager
 * @brief 入力を管理する
 */
class InputManager {
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

 private:
  std::unique_ptr<Keyboard> input_keyboard_;  //!< キーボード
  std::unique_ptr<Mouse> input_mouse_;        //!< マウス
  std::unique_ptr<GamePad> input_game_pad_;    //!< ゲームパッド
};
}  // namespace input
}  // namespace legend

#endif  // !LEGEND_INPUT_INPUT_MANAGER_H_