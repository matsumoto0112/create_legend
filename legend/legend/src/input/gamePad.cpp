#include "src/input/gamePad.h"
#include "src/math/math_util.h"

namespace {
/**
 * @brief �����_�ȉ��̌��C��
 * @param value �ύX���鐔�l
 * @param digit ����
 * @return �C���l
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
    65535;  // !< �X�e�B�b�N���͂̍ő�l
/*
 * @brief �X�e�B�b�N�̓��͒l��-1~1�ɐݒ�
 * @param value �ύX������͒l
 * @return �ύX�������͒l
 */
const float CLAMP_GAMEPAD_STICK_VALUE_MAX(unsigned long value) {
  auto result = (value / (float)GAMEPAD_STICK_VALUE_MAX - 0.5f) * 2.0f;
  return Digit(result, 3);
}
}  // namespace

namespace Framework::Input {

// �R���X�g���N�^
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
// �f�X�N�g���N�^
gamePad::~gamePad() {}
// �X�V
void gamePad::update() { setState(); }
// �ڑ���
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
// �Q�[���p�b�h�̍��X�e�B�b�N�̈ړ��l
gamePad::Vector2 gamePad::getStickLeft(unsigned int index) {
  legend::math::Vector2 result(xPos(index), -yPos(index));
  return result;
}
// �Q�[���p�b�h�̉E�X�e�B�b�N�̈ړ��l
gamePad::Vector2 gamePad::getStickRight(unsigned int index) {
  legend::math::Vector2 result(uPos(index), -rPos(index));
  return result;
}
// �Q�[���p�b�h�̏\���L�[�̈ړ��l
gamePad::Vector2 gamePad::getCrossKey(unsigned int index) {
  if (noError(index)) {
    auto result = legend::math::Vector2(getCrossHorizontal(index),
                                        getCrossVertical(index));
    return (result.Magnitude() <= 0) ? legend::math::Vector2::kZeroVector
                                     : result.Normalized();
  }
  return legend::math::Vector2::kZeroVector;
}
// �L�[�������Ă��邩
bool gamePad::getButton(JoyCode joyCode) const {
  unsigned int index = 0;
  if (noError(index)) {
    return (mCurrentJoys[index].dwButtons & joyCode);
  }
  return false;
}
// �L�[�����������i�g���K�[�j
bool gamePad::getButtonDown(JoyCode joyCode) const {
  unsigned int index = 0;
  if (noError(index)) {
    return ((mCurrentJoys[index].dwButtons & joyCode) &&
            !(mPreJoys[index] & joyCode));
  }
  return false;
}
// �L�[�𗣂������i�g���K�[�j
bool gamePad::getButtonUp(JoyCode joyCode) const {
  unsigned int index = 0;
  if (noError(index)) {
    return (!(mCurrentJoys[index].dwButtons & joyCode) &&
            (mPreJoys[index] & joyCode));
  }
  return false;
}
// �Q�[���p�b�h�̏�ԍX�V
void gamePad::setState() {
  // const unsigned int joyCount = sizeof(JOYINFOEX);

  for (int i = 0; i < padCount; i++) {
    mPreJoys[i] = mCurrentJoys[i].dwButtons;

    mCurrentJoys[i].dwSize = sizeof(JOYINFOEX);
    mCurrentJoys[i].dwFlags = JOY_RETURNALL;
    mResultJoys[i] = (JOYERR_NOERROR == joyGetPosEx(i, &mCurrentJoys[i]));
  }
}
// ���擾���A�G���[���N���邩
bool gamePad::noError(unsigned int index) const {
  if ((index < 0) || mResultJoys.size() <= index) {
    return false;
  }

  return mResultJoys[index];
}
// �Q�[���p�b�h�̃X�e�B�b�NX���̈ړ��l�擾
float gamePad::xPos(unsigned int index) {
  return (noError(index))
             ? CLAMP_GAMEPAD_STICK_VALUE_MAX(mCurrentJoys[index].dwXpos)
             : 0.0f;
}
// �Q�[���p�b�h�̃X�e�B�b�NY���̈ړ��l�擾
float gamePad::yPos(unsigned int index) {
  return (noError(index))
             ? CLAMP_GAMEPAD_STICK_VALUE_MAX(mCurrentJoys[index].dwYpos)
             : 0.0f;
}
// �Q�[���p�b�h�̃X�e�B�b�NZ���̈ړ��l�擾
float gamePad::zPos(unsigned int index) {
  return (noError(index))
             ? CLAMP_GAMEPAD_STICK_VALUE_MAX(mCurrentJoys[index].dwZpos)
             : 0.0f;
}
// �Q�[���p�b�h�̃X�e�B�b�NU���̈ړ��l�擾
float gamePad::uPos(unsigned int index) {
  return (noError(index))
             ? CLAMP_GAMEPAD_STICK_VALUE_MAX(mCurrentJoys[index].dwUpos)
             : 0.0f;
}
// �Q�[���p�b�h�̃X�e�B�b�NV���̈ړ��l�擾
float gamePad::vPos(unsigned int index) {
  return (noError(index))
             ? CLAMP_GAMEPAD_STICK_VALUE_MAX(mCurrentJoys[index].dwVpos)
             : 0.0f;
}
// �Q�[���p�b�h�̃X�e�B�b�NR���̈ړ��l�擾
float gamePad::rPos(unsigned int index) {
  return (noError(index))
             ? CLAMP_GAMEPAD_STICK_VALUE_MAX(mCurrentJoys[index].dwRpos)
             : 0.0f;
}
// �Q�[���p�b�h�̏\���L�[�����̈ړ��l�擾
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
// �Q�[���p�b�h�̏\���L�[�c���̈ړ��l�擾
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