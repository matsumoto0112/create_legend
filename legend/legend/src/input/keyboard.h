#ifndef LEGEND_INPUT_KEYBOARD_H_
#define LEGEND_INPUT_KEYBOARD_H_

/**
 * @file keyboard.h
 * @brief キーボード入力管理
 */

#include "src/input/key_code.h"

namespace legend {
namespace input {
/**
 * @class keyboard
 * @brief キーボード入力管理クラス
 */
class Keyboard {
  static constexpr i32 KEY_MAX = 256;  //!< キー最大数
  using KeyInfo = std::array<BYTE, KEY_MAX>;
  using KeyCode = key_code::Enum;

 public:
  /**
   * @brief コンストラクタ
   * @param hWnd ウィンドウハンドル
   */
  Keyboard(HWND hWnd);
  /**
   * @brief デストラクタ
   */
  ~Keyboard();
  /**
   * @brief 更新
   */
  void Update();
  /**
  * @brief キーを押しているか
  * @param key 判定するキー
  * @return 今updateで押していたらtrue
  押されていない、コントローラが存在しない場合はfalse
  */
  bool GetKey(KeyCode key) const;
  /**
  * @brief キーを押したか（トリガー）
  * @param key 判定するキー
  * @return 前updateで押されていないかつ今updateで押されていたらtrue
  それ以外の場合はfalse
  */
  bool GetKeyDown(KeyCode key) const;
  /**
  * @brief キーを離したか（トリガー）
  * @param key 判定するキー
  * @return 前updateで押されていてかつ今updateで離されたらtrue
  それ以外の場合はfalse
  */
  bool GetKeyUp(KeyCode key) const;

 private:
  KeyInfo current_keys_;  //!< 今updateで取得したキー情報
  KeyInfo previous_keys_;     //!< 前updateで取得したキー情報
 private:
  /**
   * @brief キーを押しているか
   * @param keys 判定に使うキー配列
   * @param key 判定するキー
   */
  bool CheckKeyDown(const KeyInfo& keys, KeyCode key) const;
};

}  // namespace input
}  // namespace legend

#endif  // !LEGEND_INPUT_KEYBOARD_H_