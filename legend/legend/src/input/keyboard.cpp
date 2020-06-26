#include "src/input/keyboard.h"

namespace Framework::Input {
//コンストラクタ
keyboard::keyboard(HWND hWnd) : mCurrentKeys(), mPrevKeys() {}
//デストラクタ
keyboard::~keyboard() {}
//更新
void keyboard::update() {
  //前フレームのキーの情報をコピーする
  std::copy(mCurrentKeys.begin(), mCurrentKeys.end(), mPrevKeys.begin());
  //現在のキーの押下状態を取得する
  GetKeyboardState(mCurrentKeys.data());
}
//キーの押下情報の取得
bool keyboard::getKey(KeyCode key) const {
  return checkKeyDown(mCurrentKeys, key);
}
//キーの押した瞬間かどうかを取得
bool keyboard::getKeyDown(KeyCode key) const {
  bool prev = checkKeyDown(mPrevKeys, key);
  bool cur = checkKeyDown(mCurrentKeys, key);
  return !prev && cur;
}
//キーの離した瞬間かどうかを取得
bool keyboard::getKeyUp(KeyCode key) const {
  bool prev = checkKeyDown(mPrevKeys, key);
  bool cur = checkKeyDown(mCurrentKeys, key);
  return prev && !cur;
}
//キーが押されているかどうか判定
bool keyboard::checkKeyDown(const KeyInfo& keys, KeyCode key) const {
  return (keys[key] & 0x80) != 0;
}

}  // namespace Framework::Input
