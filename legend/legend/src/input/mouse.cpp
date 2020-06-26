#include "src/input/mouse.h"

namespace Framework::Input {
//コンストラクタ
mouse::mouse(HWND hWnd) : mHWnd(hWnd) {
#define CHECK_MOUSE_BUTTON_PRESS(key) \
  { mCurrentMouseInfo[key] = GetKeyState(key) & 0x80; }

  //最初に押下状態を調べ、マップを作成しておく
  CHECK_MOUSE_BUTTON_PRESS(MouseButton::Left);
  CHECK_MOUSE_BUTTON_PRESS(MouseButton::Middle);
  CHECK_MOUSE_BUTTON_PRESS(MouseButton::Right);
}
//デストラクタ
mouse::~mouse() {}
//更新
void mouse::update() {
  //前の座標を更新する
  mPrevPosition = mPosition;

  //スクリーン座標を取得する
  POINT pos;
  GetCursorPos(&pos);
  //クライアント座標に変換する
  ScreenToClient(mHWnd, &pos);
  mPosition.x = (float)pos.x;
  mPosition.y = (float)pos.y;

  mPrevMouseInfo = mCurrentMouseInfo;
  CHECK_MOUSE_BUTTON_PRESS(MouseButton::Left);
  CHECK_MOUSE_BUTTON_PRESS(MouseButton::Middle);
  CHECK_MOUSE_BUTTON_PRESS(MouseButton::Right);
}
//マウスの今の座標を取得
const legend::math::Vector2& mouse::getMousePosition() const { return mPosition; }
//マウスのボタンの押下状態を判定
bool mouse::getMouse(MouseButton button) const {
  return mCurrentMouseInfo.at(button);
}
//マウスのボタンが押されたかどうか判定
bool mouse::getMouseDown(MouseButton button) const {
  return mCurrentMouseInfo.at(button) && mPrevMouseInfo.at(button);
}
//マウスのボタンが離されたかどうか判定
bool mouse::getMouseUp(MouseButton button) const {
  return !mCurrentMouseInfo.at(button) && mPrevMouseInfo.at(button);
}
//マウスの前回からの移動量を取得
legend::math::Vector2 mouse::getMove() const { return mPosition - mPrevPosition; }
//マウスが見えているかどうか判定する
bool mouse::isMouseVisible() const { return false; }
}  // namespace Framework::Input
