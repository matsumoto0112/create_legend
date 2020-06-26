/**
 * @file keyboard.h
 * @brief キーボード入力管理
 */

#pragma once
#include "src/input/keycode.h"

namespace Framework::Input {
/**
 * @class keyboard
 * @brief キーボード入力管理クラス
 */
class keyboard {
  static constexpr int KEY_MAX = 256;  //!< キー最大数
  using KeyInfo = std::array<BYTE, KEY_MAX>;
  using KeyCode = keyCode::Enum;

 public:
  /**
   * @brief コンストラクタ
   * @param hWnd ウィンドウハンドル
   */
  keyboard(HWND hWnd);
  /**
   * @brief デストラクタ
   */
  ~keyboard();
  /**
   * @brief 更新
   */
  void update();
  /**
  * @brief キーを押しているか
  * @param key 判定するキー
  * @return 今updateで押していたらtrue
  押されていない、コントローラが存在しない場合はfalse
  */
  bool getKey(KeyCode key) const;
  /**
  * @brief キーを押したか（トリガー）
  * @param key 判定するキー
  * @return 前updateで押されていないかつ今updateで押されていたらtrue
  それ以外の場合はfalse
  */
  bool getKeyDown(KeyCode key) const;
  /**
  * @brief キーを離したか（トリガー）
  * @param key 判定するキー
  * @return 前updateで押されていてかつ今updateで離されたらtrue
  それ以外の場合はfalse
  */
  bool getKeyUp(KeyCode key) const;

 private:
  KeyInfo mCurrentKeys;  //!< 今updateで取得したキー情報
  KeyInfo mPrevKeys;     //!< 前updateで取得したキー情報
 private:
  /**
   * @brief キーを押しているか
   * @param keys 判定に使うキー配列
   * @param key 判定するキー
   */
  bool checkKeyDown(const KeyInfo& keys, KeyCode key) const;
};

}  // namespace Framework::Input
