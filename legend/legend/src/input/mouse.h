/**
 * @file mouse.h
 * @brief マウス入力管理
 */

#pragma once
#include <src/math/vector_2.h>

namespace Framework::Input {
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
class mouse {
  using MouseButton = MouseButton::Enum;
  using MouseInfo = std::unordered_map<MouseButton, bool>;

 public:
  /**
   * @brief コンストラクタ
   * @param hWnd ウィンドウハンドル
   */
  mouse(HWND hWnd);
  /**
   * @brief デストラクタ
   */
  ~mouse();
  /**
   * @brief 更新
   */
  void update();
  /**
   * @brief マウスの座標を取得
   * @return クライアント領域でのマウス座標を返す
   */
  const legend::math::Vector2& getMousePosition() const;
  /**
   * @brief マウスのボタンを押しているか
   * @param button ボタンの種類
   * @return 押している状態ならtrueを返す
   */
  bool getMouse(MouseButton button) const;
  /**
   * @brief マウスのボタンを押したか
   * @param button ボタンの種類
   * @return 押したタイミングならtrueを返す
   */
  bool getMouseDown(MouseButton button) const;
  /**
   * @brief マウスのボタンを離したか
   * @param button ボタンの種類
   * @return 離したタイミングならtrueを返す
   */
  bool getMouseUp(MouseButton button) const;
  /**
   * @brief マウスの移動量を取得
   */
  legend::math::Vector2 getMove() const;
  /**
   * @brief マウスが出現しているかどうか判定する
   */
  bool isMouseVisible() const;

 private:
  HWND mHWnd;                           //!< ウィンドウハンドル
  legend::math::Vector2 mPosition;      //!< 今のマウスの座標
  legend::math::Vector2 mPrevPosition;  //!< 前のマウスの座標
  MouseInfo mPrevMouseInfo;  //!< 前フレームのマウスのボタンの状態
  MouseInfo mCurrentMouseInfo;  //!< 今フレームのマウスのボタンの状態
};
}  // namespace Framework::Input
