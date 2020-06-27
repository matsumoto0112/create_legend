#include "src/input/keyboard.h"

namespace legend {
namespace input {
//コンストラクタ
Keyboard::Keyboard(HWND hWnd) : mCurrentKeys_(), mPrevKeys_() {}
//デストラクタ
Keyboard::~Keyboard() {}
//更新
void Keyboard::Update() {
  //前フレームのキーの情報をコピーする
  std::copy(mCurrentKeys_.begin(), mCurrentKeys_.end(), mPrevKeys_.begin());
  //現在のキーの押下状態を取得する
  GetKeyboardState(mCurrentKeys_.data());
}
//キーの押下情報の取得
bool Keyboard::GetKey(KeyCode key) const {
  return CheckKeyDown(mCurrentKeys_, key);
}
//キーの押した瞬間かどうかを取得
bool Keyboard::GetKeyDown(KeyCode key) const {
  bool prev = CheckKeyDown(mPrevKeys_, key);
  bool cur = CheckKeyDown(mCurrentKeys_, key);
  return !prev && cur;
}
//キーの離した瞬間かどうかを取得
bool Keyboard::GetKeyUp(KeyCode key) const {
  bool prev = CheckKeyDown(mPrevKeys_, key);
  bool cur = CheckKeyDown(mCurrentKeys_, key);
  return prev && !cur;
}
//キーが押されているかどうか判定
bool Keyboard::CheckKeyDown(const KeyInfo& keys, KeyCode key) const {
  return (keys[key] & 0x80) != 0;
}
}  // namespace input
}  // namespace legend