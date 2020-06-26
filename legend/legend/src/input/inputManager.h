/**
 * @file InputManager.h
 * @brief ゲームにおける入力全般を管理する
 */

#pragma once
#include "src/input/keyboard.h"
#include "src/input/mouse.h"
#include "src/input/gamePad.h"

namespace Framework::Input {
/**
 * @class InputManager
 * @brief 入力を管理する
 */
class inputManager {
 public:
  /**
   * @brief コンストラクタ
   * @param hWnd ウィンドウハンドル
   */
  inputManager(HWND hWnd);
  /**
   * @brief デストラクタ
   */
  ~inputManager();
  /**
   * @brief 入力情報を更新する
   */
  void update();
  /**
   * @brief キーボードを取得する
   */
  inline keyboard* getKeyboard() const { return mKeyboard.get(); }
  /**
   * @brief マウスを取得する
   */
  inline mouse* getMouse() const { return mMouse.get(); }
  /**
   * @brief ゲームパッドを取得する
   */
  inline gamePad* getGamepad() const { return mGamepad.get(); }

 private:
  std::unique_ptr<keyboard> mKeyboard;  //!< キーボード
  std::unique_ptr<mouse> mMouse;        //!< マウス
  std::unique_ptr<gamePad> mGamepad;          //!< ゲームパッド
};
}  // namespace Framework::Input
