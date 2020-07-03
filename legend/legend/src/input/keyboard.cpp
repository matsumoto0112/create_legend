#include "src/input/keyboard.h"

namespace legend {
namespace input {
//コンストラクタ
Keyboard::Keyboard(HWND hWnd) : current_keys_(), previous_keys_() {}
//デストラクタ
Keyboard::~Keyboard() {}
//更新
void Keyboard::Update() {
  //前フレームのキーの情報をコピーする
  std::copy(current_keys_.begin(), current_keys_.end(), previous_keys_.begin());
  //現在のキーの押下状態を取得する
  GetKeyboardState(current_keys_.data());
}
//キーの押下情報の取得
bool Keyboard::GetKey(KeyCode key) const {
  return CheckKeyDown(current_keys_, key);
}
//キーの押した瞬間かどうかを取得
bool Keyboard::GetKeyDown(KeyCode key) const {
  bool prev = CheckKeyDown(previous_keys_, key);
  bool cur = CheckKeyDown(current_keys_, key);
  return !prev && cur;
}
//キーの離した瞬間かどうかを取得
bool Keyboard::GetKeyUp(KeyCode key) const {
  bool prev = CheckKeyDown(previous_keys_, key);
  bool cur = CheckKeyDown(current_keys_, key);
  return prev && !cur;
}
//キーが押されているかどうか判定
bool Keyboard::CheckKeyDown(const KeyInfo& keys, KeyCode key) const {
  return (keys[key] & 0x80) != 0;
}
}  // namespace input
}  // namespace legend