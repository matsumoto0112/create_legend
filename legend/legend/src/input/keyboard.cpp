#include "src/input/keyboard.h"

namespace legend {
namespace input {
//�R���X�g���N�^
Keyboard::Keyboard(HWND hWnd) : mCurrentKeys_(), mPrevKeys_() {}
//�f�X�g���N�^
Keyboard::~Keyboard() {}
//�X�V
void Keyboard::Update() {
  //�O�t���[���̃L�[�̏����R�s�[����
  std::copy(mCurrentKeys_.begin(), mCurrentKeys_.end(), mPrevKeys_.begin());
  //���݂̃L�[�̉�����Ԃ��擾����
  GetKeyboardState(mCurrentKeys_.data());
}
//�L�[�̉������̎擾
bool Keyboard::GetKey(KeyCode key) const {
  return CheckKeyDown(mCurrentKeys_, key);
}
//�L�[�̉������u�Ԃ��ǂ������擾
bool Keyboard::GetKeyDown(KeyCode key) const {
  bool prev = CheckKeyDown(mPrevKeys_, key);
  bool cur = CheckKeyDown(mCurrentKeys_, key);
  return !prev && cur;
}
//�L�[�̗������u�Ԃ��ǂ������擾
bool Keyboard::GetKeyUp(KeyCode key) const {
  bool prev = CheckKeyDown(mPrevKeys_, key);
  bool cur = CheckKeyDown(mCurrentKeys_, key);
  return prev && !cur;
}
//�L�[��������Ă��邩�ǂ�������
bool Keyboard::CheckKeyDown(const KeyInfo& keys, KeyCode key) const {
  return (keys[key] & 0x80) != 0;
}
}  // namespace input
}  // namespace legend