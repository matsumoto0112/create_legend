#include "src/input/keyboard.h"

namespace Framework::Input {
//�R���X�g���N�^
keyboard::keyboard(HWND hWnd) : mCurrentKeys(), mPrevKeys() {}
//�f�X�g���N�^
keyboard::~keyboard() {}
//�X�V
void keyboard::update() {
  //�O�t���[���̃L�[�̏����R�s�[����
  std::copy(mCurrentKeys.begin(), mCurrentKeys.end(), mPrevKeys.begin());
  //���݂̃L�[�̉�����Ԃ��擾����
  GetKeyboardState(mCurrentKeys.data());
}
//�L�[�̉������̎擾
bool keyboard::getKey(KeyCode key) const {
  return checkKeyDown(mCurrentKeys, key);
}
//�L�[�̉������u�Ԃ��ǂ������擾
bool keyboard::getKeyDown(KeyCode key) const {
  bool prev = checkKeyDown(mPrevKeys, key);
  bool cur = checkKeyDown(mCurrentKeys, key);
  return !prev && cur;
}
//�L�[�̗������u�Ԃ��ǂ������擾
bool keyboard::getKeyUp(KeyCode key) const {
  bool prev = checkKeyDown(mPrevKeys, key);
  bool cur = checkKeyDown(mCurrentKeys, key);
  return prev && !cur;
}
//�L�[��������Ă��邩�ǂ�������
bool keyboard::checkKeyDown(const KeyInfo& keys, KeyCode key) const {
  return (keys[key] & 0x80) != 0;
}

}  // namespace Framework::Input
