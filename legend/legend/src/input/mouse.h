#ifndef LEGEND_INPUT_MOUSE_H_
#define LEGEND_INPUT_MOUSE_H_

/**
 * @file mouse.h
 * @brief マウス入力管理
 */

#include <src/math/vector_2.h>

namespace legend {
namespace input {
namespace MouseButton {
/**
 * @enum Enum
 * @brief マウスのボタン定義
 */
enum Enum {
  Left = VK_LBUTTON,
  Right = VK_RBUTTON,
  Middle = VK_MBUTTON,
};
};  // namespace MouseButton

/**
 * @class Mouse
 * @brief マウス情報管理クラス
 */
class Mouse {
  using MouseButton = MouseButton::Enum;
  using MouseInfo = std::unordered_map<MouseButton, bool>;

 public:
  /**
   * @brief コンストラクタ
   * @param hWnd ウィンドウハンドル
   */
  Mouse(HWND hWnd);
  /**
   * @brief デストラクタ
   */
  ~Mouse();
  /**
   * @brief 更新
   */
  void Update();
  /**
   * @brief マウスの座標を取得
   * @return クライアント領域でのマウス座標を返す
   */
  const legend::math::Vector2& GetMousePosition() const;
  /**
   * @brief マウスのボタンを押しているか
   * @param button ボタンの種類
   * @return 押している状態ならtrueを返す
   */
  bool GetMouse(MouseButton button) const;
  /**
   * @brief マウスのボタンを押したか
   * @param button ボタンの種類
   * @return 押したタイミングならtrueを返す
   */
  bool GetMouseDown(MouseButton button) const;
  /**
   * @brief マウスのボタンを離したか
   * @param button ボタンの種類
   * @return 離したタイミングならtrueを返す
   */
  bool GetMouseUp(MouseButton button) const;
  /**
   * @brief マウスの移動量を取得
   */
  legend::math::Vector2 GetMove() const;
  /**
   * @brief マウスが出現しているかどうか判定する
   */
  bool IsMouseVisible() const;

 private:
  HWND mHWnd_;                           //!< ウィンドウハンドル
  legend::math::Vector2 mPosition_;      //!< 今のマウスの座標
  legend::math::Vector2 mPrevPosition_;  //!< 前のマウスの座標
  MouseInfo mPrevMouseInfo_;  //!< 前フレームのマウスのボタンの状態
  MouseInfo mCurrentMouseInfo_;  //!< 今フレームのマウスのボタンの状態
};
}  // namespace input
}  // namespace legend

#endif  // !LEGEND_INPUT_MOUSE_H_