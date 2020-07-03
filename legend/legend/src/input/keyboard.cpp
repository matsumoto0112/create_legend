#include "src/input/keyboard.h"

namespace legend {
namespace input {
//�R���X�g���N�^
Keyboard::Keyboard(HWND hWnd) : current_keys_(), previous_keys_() {}
//�f�X�g���N�^
Keyboard::~Keyboard() {}
//�X�V
void Keyboard::Update() {
  //�O�t���[���̃L�[�̏����R�s�[����
  std::copy(current_keys_.begin(), current_keys_.end(), previous_keys_.begin());
  //���݂̃L�[�̉�����Ԃ��擾����
  GetKeyboardState(current_keys_.data());
}
//�L�[�̉������̎擾
bool Keyboard::GetKey(KeyCode key) const {
  return CheckKeyDown(current_keys_, key);
}
//�L�[�̉������u�Ԃ��ǂ������擾
bool Keyboard::GetKeyDown(KeyCode key) const {
  bool prev = CheckKeyDown(previous_keys_, key);
  bool cur = CheckKeyDown(current_keys_, key);
  return !prev && cur;
}
//�L�[�̗������u�Ԃ��ǂ������擾
bool Keyboard::GetKeyUp(KeyCode key) const {
  bool prev = CheckKeyDown(previous_keys_, key);
  bool cur = CheckKeyDown(current_keys_, key);
  return prev && !cur;
}
//�L�[��������Ă��邩�ǂ�������
bool Keyboard::CheckKeyDown(const KeyInfo& keys, KeyCode key) const {
  return (keys[key] & 0x80) != 0;
}
}  // namespace input
}  // namespace legend