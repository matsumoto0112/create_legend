#include "src/input/gamePad.h"
#include "src/math/math_util.h"

namespace {
/**
 * @brief 小数点以下の桁修正
 * @param value 変更する数値
 * @param digit 桁数
 * @return 修正値
 */
float Digit(float value, long digit = 1) {
  digit = (digit < 0) ? 0 : (7 <= digit) ? 7 : digit;

  if (digit == 0) {
    return (float)((int)value);
  }

  float d = 1.0f;
  for (int i = 0; i < digit; i++) {
    d *= 10.0f;
  }

  auto result = (float)((int)(value * d)) / d;
  return result;
}

const unsigned long GAMEPAD_STICK_VALUE_MAX =
    65535;  // !< スティック入力の最大値
/*
 * @brief スティックの入力値を-1~1に設定
 * @param value 変更する入力値
 * @return 変更した入力値
 */
const float CLAMP_GAMEPAD_STICK_VALUE_MAX(unsigned long value) {
  auto result = (value / (float)GAMEPAD_STICK_VALUE_MAX - 0.5f) * 2.0f;
  return Digit(result, 3);
}
}  // namespace

namespace Framework::Input {

// コンストラクタ
gamePad::gamePad(HWND hWnd) : mHWnd(hWnd) {
  padCount = (sizeof(JOYINFOEX) < Input::gamePad::PAD_MAX)
                 ? sizeof(JOYINFOEX)
                 : Input::gamePad::PAD_MAX;
  // const unsigned int joyCount = sizeof(JOYINFOEX);
  mCurrentJoys = std::vector<JOYINFOEX>(padCount);
  mPreJoys = std::vector<unsigned long>(padCount);
  mResultJoys = std::vector<MMRESULT>(padCount);

  for (int i = 0; i < padCount; i++) {
    mCurrentJoys[i].dwSize = sizeof(JOYINFOEX);
    mCurrentJoys[i].dwFlags = JOY_RETURNALL;
    mPreJoys[i] = 0;
    mResultJoys[i] = 0;
  }
}
// デスクトラクタ
gamePad::~gamePad() {}
// 更新
void gamePad::update() { setState(); }
// 接続数
int gamePad::joyCount() {
  int count = 0;
  for (unsigned int i = 0; i < joyGetNumDevs(); i++) {
    if ((unsigned int)padCount <= i) {
      return count;
    }
    if (noError(i)) {
      count++;
    }
  }
  return count;
}
// ゲームパッドの左スティックの移動値
gamePad::Vector2 gamePad::getStickLeft(unsigned int index) {
  legend::math::Vector2 result(xPos(index), -yPos(index));
  return result;
}
// ゲームパッドの右スティックの移動値
gamePad::Vector2 gamePad::getStickRight(unsigned int index) {
  legend::math::Vector2 result(uPos(index), -rPos(index));
  return result;
}
// ゲームパッドの十字キーの移動値
gamePad::Vector2 gamePad::getCrossKey(unsigned int index) {
  if (noError(index)) {
    auto result = legend::math::Vector2(getCrossHorizontal(index),
                                        getCrossVertical(index));
    return (result.Magnitude() <= 0) ? legend::math::Vector2::kZeroVector
                                     : result.Normalized();
  }
  return legend::math::Vector2::kZeroVector;
}
// キーを押しているか
bool gamePad::getButton(JoyCode joyCode) const {
  unsigned int index = 0;
  if (noError(index)) {
    return (mCurrentJoys[index].dwButtons & joyCode);
  }
  return false;
}
// キーを押したか（トリガー）
bool gamePad::getButtonDown(JoyCode joyCode) const {
  unsigned int index = 0;
  if (noError(index)) {
    return ((mCurrentJoys[index].dwButtons & joyCode) &&
            !(mPreJoys[index] & joyCode));
  }
  return false;
}
// キーを離したか（トリガー）
bool gamePad::getButtonUp(JoyCode joyCode) const {
  unsigned int index = 0;
  if (noError(index)) {
    return (!(mCurrentJoys[index].dwButtons & joyCode) &&
            (mPreJoys[index] & joyCode));
  }
  return false;
}
// ゲームパッドの状態更新
void gamePad::setState() {
  // const unsigned int joyCount = sizeof(JOYINFOEX);

  for (int i = 0; i < padCount; i++) {
    mPreJoys[i] = mCurrentJoys[i].dwButtons;

    mCurrentJoys[i].dwSize = sizeof(JOYINFOEX);
    mCurrentJoys[i].dwFlags = JOY_RETURNALL;
    mResultJoys[i] = (JOYERR_NOERROR == joyGetPosEx(i, &mCurrentJoys[i]));
  }
}
// 情報取得時、エラーが起きるか
bool gamePad::noError(unsigned int index) const {
  if ((index < 0) || mResultJoys.size() <= index) {
    return false;
  }

  return mResultJoys[index];
}
// ゲームパッドのスティックX軸の移動値取得
float gamePad::xPos(unsigned int index) {
  return (noError(index))
             ? CLAMP_GAMEPAD_STICK_VALUE_MAX(mCurrentJoys[index].dwXpos)
             : 0.0f;
}
// ゲームパッドのスティックY軸の移動値取得
float gamePad::yPos(unsigned int index) {
  return (noError(index))
             ? CLAMP_GAMEPAD_STICK_VALUE_MAX(mCurrentJoys[index].dwYpos)
             : 0.0f;
}
// ゲームパッドのスティックZ軸の移動値取得
float gamePad::zPos(unsigned int index) {
  return (noError(index))
             ? CLAMP_GAMEPAD_STICK_VALUE_MAX(mCurrentJoys[index].dwZpos)
             : 0.0f;
}
// ゲームパッドのスティックU軸の移動値取得
float gamePad::uPos(unsigned int index) {
  return (noError(index))
             ? CLAMP_GAMEPAD_STICK_VALUE_MAX(mCurrentJoys[index].dwUpos)
             : 0.0f;
}
// ゲームパッドのスティックV軸の移動値取得
float gamePad::vPos(unsigned int index) {
  return (noError(index))
             ? CLAMP_GAMEPAD_STICK_VALUE_MAX(mCurrentJoys[index].dwVpos)
             : 0.0f;
}
// ゲームパッドのスティックR軸の移動値取得
float gamePad::rPos(unsigned int index) {
  return (noError(index))
             ? CLAMP_GAMEPAD_STICK_VALUE_MAX(mCurrentJoys[index].dwRpos)
             : 0.0f;
}
// ゲームパッドの十字キー横軸の移動値取得
float gamePad::getCrossHorizontal(unsigned int index) {
  if (noError(index)) {
    auto crossKey = (float)mCurrentJoys[index].dwPOV / 100.0f;
    if (360.0f <= crossKey) {
      return 0.0f;
    }

    crossKey *= legend::math::util::DEG_2_RAD;
    return Digit(legend::math::util::Sin(crossKey), 3);
  }
  return 0.0f;
}
// ゲームパッドの十字キー縦軸の移動値取得
float gamePad::getCrossVertical(unsigned int index) {
  if (noError(index)) {
    auto crossKey = (float)mCurrentJoys[index].dwPOV / 100.0f;
    if (360.0f <= crossKey) {
      return 0.0f;
    }

    crossKey *= legend::math::util::DEG_2_RAD;
    return Digit(legend::math::util::Cos(crossKey), 3);
  }
  return 0.0f;
}
}  // namespace Framework::Input