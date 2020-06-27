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
    return static_cast<float>(static_cast<int>(value));
  }

  float d = 1.0f;
  for (int i = 0; i < digit; i++) {
    d *= 10.0f;
  }

  return static_cast<float>(static_cast<int>(value * d)) / d;
}

const unsigned long GAMEPAD_STICK_VALUE_MAX =
    65535;  // !< スティック入力の最大値
/*
 * @brief スティックの入力値を-1~1に設定
 * @param value 変更する入力値
 * @return 変更した入力値
 */
const float CLAMP_GAMEPAD_STICK_VALUE_MAX(unsigned long value) {
  float result =
      (value / static_cast<float>(GAMEPAD_STICK_VALUE_MAX) - 0.5f) * 2.0f;
  return Digit(result, 3);
}
}  // namespace

namespace legend {
namespace input {

// コンストラクタ
GamePad::GamePad(HWND hWnd) : mHWnd_(hWnd) {
  padCount_ = (sizeof(JOYINFOEX) < PAD_MAX) ? sizeof(JOYINFOEX) : PAD_MAX;
  mCurrentJoys_ = std::vector<JOYINFOEX>(padCount_);
  mPreJoys_ = std::vector<u64>(padCount_);
  mResultJoys_ = std::vector<MMRESULT>(padCount_);

  for (i32 i = 0; i < padCount_; i++) {
    mCurrentJoys_[i].dwSize = sizeof(JOYINFOEX);
    mCurrentJoys_[i].dwFlags = JOY_RETURNALL;
    mPreJoys_[i] = 0;
    mResultJoys_[i] = 0;
  }
}
// デスクトラクタ
GamePad::~GamePad() {}
// 更新
void GamePad::Update() { SetState(); }
// 接続数
i32 GamePad::GetCount() {
  u32 count = 0;
  for (u32 i = 0; i < joyGetNumDevs(); i++) {
    if (static_cast<u32>(padCount_) <= i) {
      return count;
    }
    if (NoError(i)) {
      count++;
    }
  }
  return count;
}
// ゲームパッドの左スティックの移動値
legend::math::Vector2 GamePad::GetStickLeft(u32 index) {
  legend::math::Vector2 result(Stick_X_Pos(index), -Stick_Y_Pos(index));
  return result;
}
// ゲームパッドの右スティックの移動値
legend::math::Vector2 GamePad::GetStickRight(u32 index) {
  legend::math::Vector2 result(Stick_U_Pos(index), -Stick_R_Pos(index));
  return result;
}
// ゲームパッドの十字キーの移動値
legend::math::Vector2 GamePad::GetCrossKey(u32 index) {
  legend::math::Vector2 result = legend::math::Vector2::kZeroVector;
  if (NoError(index)) {
    float h = GetCrossHorizontal(index);
    float v = GetCrossVertical(index);
    result = legend::math::Vector2(h, v);
    return (result.Magnitude() <= 0) ? legend::math::Vector2::kZeroVector
                                     : result.Normalized();
  }
  return result;
}
// キーを押しているか
bool GamePad::GetButton(JoyCode joyCode, u32 index) const {
  if (NoError(index)) {
    return (mCurrentJoys_[index].dwButtons & joyCode);
  }
  return false;
}
// キーを押したか（トリガー）
bool GamePad::GetButtonDown(JoyCode joyCode, u32 index) const {
  if (NoError(index)) {
    return ((mCurrentJoys_[index].dwButtons & joyCode) &&
            !(mPreJoys_[index] & joyCode));
  }
  return false;
}
// キーを離したか（トリガー）
bool GamePad::GetButtonUp(JoyCode joyCode, u32 index) const {
  if (NoError(index)) {
    return (!(mCurrentJoys_[index].dwButtons & joyCode) &&
            (mPreJoys_[index] & joyCode));
  }
  return false;
}
// キーを押しているか
bool GamePad::GetButton(JoyCode joyCode) const { return GetButton(joyCode, 0); }
// キーを押したか（トリガー）
bool GamePad::GetButtonDown(JoyCode joyCode) const {
  return GetButton(joyCode, 0);
}
// キーを離したか（トリガー）
bool GamePad::GetButtonUp(JoyCode joyCode) const {
  return GetButton(joyCode, 0);
}
// ゲームパッドの状態更新
void GamePad::SetState() {

  for (i32 i = 0; i < padCount_; i++) {
    mPreJoys_[i] = mCurrentJoys_[i].dwButtons;

    mCurrentJoys_[i].dwSize = sizeof(JOYINFOEX);
    mCurrentJoys_[i].dwFlags = JOY_RETURNALL;
    mResultJoys_[i] = (JOYERR_NOERROR == joyGetPosEx(i, &mCurrentJoys_[i]));
  }
}
// 情報取得時、エラーが起きるか
bool GamePad::NoError(u32 index) const {
  if ((index < 0) || mResultJoys_.size() <= index) {
    return false;
  }

  return mResultJoys_[index];
}
// ゲームパッドのスティックX軸の移動値取得
float GamePad::Stick_X_Pos(u32 index) {
  return (NoError(index))
             ? CLAMP_GAMEPAD_STICK_VALUE_MAX(mCurrentJoys_[index].dwXpos)
             : 0.0f;
}
// ゲームパッドのスティックY軸の移動値取得
float GamePad::Stick_Y_Pos(u32 index) {
  return (NoError(index))
             ? CLAMP_GAMEPAD_STICK_VALUE_MAX(mCurrentJoys_[index].dwYpos)
             : 0.0f;
}
// ゲームパッドのスティックZ軸の移動値取得
float GamePad::Stick_Z_Pos(u32 index) {
  return (NoError(index))
             ? CLAMP_GAMEPAD_STICK_VALUE_MAX(mCurrentJoys_[index].dwZpos)
             : 0.0f;
}
// ゲームパッドのスティックU軸の移動値取得
float GamePad::Stick_U_Pos(u32 index) {
  return (NoError(index))
             ? CLAMP_GAMEPAD_STICK_VALUE_MAX(mCurrentJoys_[index].dwUpos)
             : 0.0f;
}
// ゲームパッドのスティックV軸の移動値取得
float GamePad::Stick_V_Pos(u32 index) {
  return (NoError(index))
             ? CLAMP_GAMEPAD_STICK_VALUE_MAX(mCurrentJoys_[index].dwVpos)
             : 0.0f;
}
// ゲームパッドのスティックR軸の移動値取得
float GamePad::Stick_R_Pos(u32 index) {
  return (NoError(index))
             ? CLAMP_GAMEPAD_STICK_VALUE_MAX(mCurrentJoys_[index].dwRpos)
             : 0.0f;
}
// ゲームパッドの十字キー横軸の移動値取得
float GamePad::GetCrossHorizontal(u32 index) {
  if (NoError(index)) {
    float crossKey = static_cast<float>(mCurrentJoys_[index].dwPOV) / 100.0f;
    if (360.0f <= crossKey) {
      return 0.0f;
    }

    crossKey *= legend::math::util::DEG_2_RAD;
    return Digit(legend::math::util::Sin(crossKey), 3);
  }
  return 0.0f;
}
// ゲームパッドの十字キー縦軸の移動値取得
float GamePad::GetCrossVertical(u32 index) {
  if (NoError(index)) {
    float crossKey = static_cast<float>(mCurrentJoys_[index].dwPOV) / 100.0f;
    if (360.0f <= crossKey) {
      return 0.0f;
    }

    crossKey *= legend::math::util::DEG_2_RAD;
    return Digit(legend::math::util::Cos(crossKey), 3);
  }
  return 0.0f;
}
}  // namespace input
}  // namespace legend
