#include "src/input/mouse.h"

namespace legend {
namespace input {
//コンストラクタ
Mouse::Mouse(HWND hWnd) : hwnd_(hWnd) {
#define CHECK_MOUSE_BUTTON_PRESS(key) \
  { current_mouse_info_[key] = GetKeyState(key) & 0x80; }

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
  previous_position_ = current_position_;

  //スクリーン座標を取得する
  POINT pos;
  GetCursorPos(&pos);
  //クライアント座標に変換する
  ScreenToClient(hwnd_, &pos);
  current_position_.x = (float)pos.x;
  current_position_.y = (float)pos.y;

  previous_mouse_info_ = current_mouse_info_;
  CHECK_MOUSE_BUTTON_PRESS(MouseButton::Left);
  CHECK_MOUSE_BUTTON_PRESS(MouseButton::Middle);
  CHECK_MOUSE_BUTTON_PRESS(MouseButton::Right);
}
//マウスの今の座標を取得
const legend::math::Vector2& Mouse::GetMousePosition() const {
  return current_position_;
}
//マウスのボタンの押下状態を判定
bool Mouse::GetMouse(MouseButton button) const {
  return current_mouse_info_.at(button);
}
//マウスのボタンが押されたかどうか判定
bool Mouse::GetMouseDown(MouseButton button) const {
  return current_mouse_info_.at(button) && previous_mouse_info_.at(button);
}
//マウスのボタンが離されたかどうか判定
bool Mouse::GetMouseUp(MouseButton button) const {
  return !current_mouse_info_.at(button) && previous_mouse_info_.at(button);
}
//マウスの前回からの移動量を取得
legend::math::Vector2 Mouse::GetMove() const {
  return current_position_ - previous_position_;
}
//マウスが見えているかどうか判定する
bool Mouse::IsMouseVisible() const { return false; }
}  // namespace input
}  // namespace legend