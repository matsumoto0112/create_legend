#include "src/input/game_pad.h"
#include "src/math/math_util.h"

namespace {

const unsigned long GAMEPAD_STICK_VALUE_MAX =
    65535;  // !< スティック入力の最大値
/*
 * @brief スティックの入力値を-1~1に設定
 * @param value 変更する入力値
 * @return 変更した入力値
 */
const float Clamp_GamePadStick_ValueMax(unsigned long value) {
  float result =
      (value / static_cast<float>(GAMEPAD_STICK_VALUE_MAX) - 0.5f) * 2.0f;
  return legend::math::util::Round(result, 3);
}
}  // namespace

namespace legend {
namespace input {

// コンストラクタ
GamePad::GamePad(HWND hWnd) : hwnd_(hWnd) {
  pad_count_ = (sizeof(JOYINFOEX) < PAD_MAX) ? sizeof(JOYINFOEX) : PAD_MAX;
  current_joys_ = std::vector<JOYINFOEX>(pad_count_);
  previous_joys_ = std::vector<u64>(pad_count_);
  result_joys_ = std::vector<MMRESULT>(pad_count_);

  for (i32 i = 0; i < pad_count_; i++) {
    current_joys_[i].dwSize = sizeof(JOYINFOEX);
    current_joys_[i].dwFlags = JOY_RETURNALL;
    previous_joys_[i] = 0;
    result_joys_[i] = 0;
  }
}
// デスクトラクタ
GamePad::~GamePad() {}
// 更新
void GamePad::Update() { SetState(); }
// 接続数
i32 GamePad::GetCount() const {
  u32 count = 0;
  for (u32 i = 0; i < joyGetNumDevs(); i++) {
    if (static_cast<u32>(pad_count_) <= i) {
      return count;
    }
    if (NoError(i)) {
      count++;
    }
  }
  return count;
}
// ゲームパッドの左スティックの移動値
legend::math::Vector2 GamePad::GetStickLeft(u32 index) const {
  legend::math::Vector2 result(Stick_X_Pos(index), -Stick_Y_Pos(index));
  return result;
}
// ゲームパッドの右スティックの移動値
legend::math::Vector2 GamePad::GetStickRight(u32 index) const {
  legend::math::Vector2 result(Stick_U_Pos(index), -Stick_R_Pos(index));
  return result;
}
// ゲームパッドの十字キーの移動値
legend::math::Vector2 GamePad::GetCrossKey(u32 index) const {
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
// ゲームパッドの左スティックの移動値
math::Vector2 GamePad::GetStickLeft() const { return GetStickLeft(0); }
// ゲームパッドの右スティックの移動値
math::Vector2 GamePad::GetStickRight() const { return GetStickRight(0); }
// ゲームパッドの十字キーの移動値
math::Vector2 GamePad::GetCrossKey() const { return GetCrossKey(0); }
// キーを押しているか
bool GamePad::GetButton(JoyCode joyCode, u32 index) const {
  if (NoError(index)) {
    return (current_joys_[index].dwButtons & joyCode);
  }
  return false;
}
// キーを押したか（トリガー）
bool GamePad::GetButtonDown(JoyCode joyCode, u32 index) const {
  if (NoError(index)) {
    return ((current_joys_[index].dwButtons & joyCode) &&
            !(previous_joys_[index] & joyCode));
  }
  return false;
}
// キーを離したか（トリガー）
bool GamePad::GetButtonUp(JoyCode joyCode, u32 index) const {
  if (NoError(index)) {
    return (!(current_joys_[index].dwButtons & joyCode) &&
            (previous_joys_[index] & joyCode));
  }
  return false;
}
// キーを押しているか
bool GamePad::GetButton(JoyCode joyCode) const { return GetButton(joyCode, 0); }
// キーを押したか（トリガー）
bool GamePad::GetButtonDown(JoyCode joyCode) const {
  return GetButtonDown(joyCode, 0);
}
// キーを離したか（トリガー）
bool GamePad::GetButtonUp(JoyCode joyCode) const {
  return GetButtonUp(joyCode, 0);
}
// ゲームパッドの状態更新
void GamePad::SetState() {

  for (i32 i = 0; i < pad_count_; i++) {
    previous_joys_[i] = current_joys_[i].dwButtons;

    current_joys_[i].dwSize = sizeof(JOYINFOEX);
    current_joys_[i].dwFlags = JOY_RETURNALL;
    result_joys_[i] = (JOYERR_NOERROR == joyGetPosEx(i, &current_joys_[i]));
  }
}
// 情報取得時、エラーが起きるか
bool GamePad::NoError(u32 index) const {
  if ((index < 0) || result_joys_.size() <= index) {
    return false;
  }

  return result_joys_[index];
}
// ゲームパッドのスティックX軸の移動値取得
float GamePad::Stick_X_Pos(u32 index) const{
  return (NoError(index))
             ? Clamp_GamePadStick_ValueMax(current_joys_[index].dwXpos)
             : 0.0f;
}
// ゲームパッドのスティックY軸の移動値取得
float GamePad::Stick_Y_Pos(u32 index) const {
  return (NoError(index))
             ? Clamp_GamePadStick_ValueMax(current_joys_[index].dwYpos)
             : 0.0f;
}
// ゲームパッドのスティックZ軸の移動値取得
float GamePad::Stick_Z_Pos(u32 index) const {
  return (NoError(index))
             ? Clamp_GamePadStick_ValueMax(current_joys_[index].dwZpos)
             : 0.0f;
}
// ゲームパッドのスティックU軸の移動値取得
float GamePad::Stick_U_Pos(u32 index) const {
  return (NoError(index))
             ? Clamp_GamePadStick_ValueMax(current_joys_[index].dwUpos)
             : 0.0f;
}
// ゲームパッドのスティックV軸の移動値取得
float GamePad::Stick_V_Pos(u32 index) const {
  return (NoError(index))
             ? Clamp_GamePadStick_ValueMax(current_joys_[index].dwVpos)
             : 0.0f;
}
// ゲームパッドのスティックR軸の移動値取得
float GamePad::Stick_R_Pos(u32 index) const {
  return (NoError(index))
             ? Clamp_GamePadStick_ValueMax(current_joys_[index].dwRpos)
             : 0.0f;
}
// ゲームパッドの十字キー横軸の移動値取得
float GamePad::GetCrossHorizontal(u32 index) const {
  if (NoError(index)) {
    float crossKey = static_cast<float>(current_joys_[index].dwPOV) / 100.0f;
    if (360.0f <= crossKey) {
      return 0.0f;
    }

    crossKey *= legend::math::util::DEG_2_RAD;
    return math::util::Round(legend::math::util::Sin(crossKey), 3);
  }
  return 0.0f;
}
// ゲームパッドの十字キー縦軸の移動値取得
float GamePad::GetCrossVertical(u32 index) const {
  if (NoError(index)) {
    float crossKey = static_cast<float>(current_joys_[index].dwPOV) / 100.0f;
    if (360.0f <= crossKey) {
      return 0.0f;
    }

    crossKey *= legend::math::util::DEG_2_RAD;
    return math::util::Round(legend::math::util::Cos(crossKey), 3);
  }
  return 0.0f;
}
}  // namespace input
}  // namespace legend
