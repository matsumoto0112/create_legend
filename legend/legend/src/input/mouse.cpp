#include "src/input/mouse.h"

namespace legend {
namespace input {
//コンストラクタ
Mouse::Mouse(HWND hWnd) : mHWnd_(hWnd) {
#define CHECK_MOUSE_BUTTON_PRESS(key) \
  { mCurrentMouseInfo_[key] = GetKeyState(key) & 0x80; }

  //最初に押下状態を調べ、マップを作成しておく
  CHECK_MOUSE_BUTTON_PRESS(MouseButton::Left);
  CHECK_MOUSE_BUTTON_PRESS(MouseButton::Middle);
  CHECK_MOUSE_BUTTON_PRESS(MouseButton::Right);
}
//デストラクタ
Mouse::~Mouse() {}
//更新
void Mouse::Update() {
  //前の座標を更新する
  mPrevPosition_ = mPosition_;

  //スクリーン座標を取得する
  POINT pos;
  GetCursorPos(&pos);
  //クライアント座標に変換する
  ScreenToClient(mHWnd_, &pos);
  mPosition_.x = (float)pos.x;
  mPosition_.y = (float)pos.y;

  mPrevMouseInfo_ = mCurrentMouseInfo_;
  CHECK_MOUSE_BUTTON_PRESS(MouseButton::Left);
  CHECK_MOUSE_BUTTON_PRESS(MouseButton::Middle);
  CHECK_MOUSE_BUTTON_PRESS(MouseButton::Right);
}
//マウスの今の座標を取得
const legend::math::Vector2& Mouse::GetMousePosition() const {
  return mPosition_;
}
//マウスのボタンの押下状態を判定
bool Mouse::GetMouse(MouseButton button) const {
  return mCurrentMouseInfo_.at(button);
}
//マウスのボタンが押されたかどうか判定
bool Mouse::GetMouseDown(MouseButton button) const {
  return mCurrentMouseInfo_.at(button) && mPrevMouseInfo_.at(button);
}
//マウスのボタンが離されたかどうか判定
bool Mouse::GetMouseUp(MouseButton button) const {
  return !mCurrentMouseInfo_.at(button) && mPrevMouseInfo_.at(button);
}
//マウスの前回からの移動量を取得
legend::math::Vector2 Mouse::GetMove() const {
  return mPosition_ - mPrevPosition_;
}
//マウスが見えているかどうか判定する
bool Mouse::IsMouseVisible() const { return false; }
}  // namespace input
}  // namespace legend