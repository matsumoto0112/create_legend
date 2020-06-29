#include "src/input/mouse.h"

namespace legend {
namespace input {
//�R���X�g���N�^
Mouse::Mouse(HWND hWnd) : hwnd_(hWnd) {
#define CHECK_MOUSE_BUTTON_PRESS(key) \
  { current_mouse_info_[key] = GetKeyState(key) & 0x80; }

  //�ŏ��ɉ�����Ԃ𒲂ׁA�}�b�v���쐬���Ă���
  CHECK_MOUSE_BUTTON_PRESS(MouseButton::Left);
  CHECK_MOUSE_BUTTON_PRESS(MouseButton::Middle);
  CHECK_MOUSE_BUTTON_PRESS(MouseButton::Right);
}
//�f�X�g���N�^
Mouse::~Mouse() {}
//�X�V
void Mouse::Update() {
  //�O�̍��W���X�V����
  previous_position_ = current_position_;

  //�X�N���[�����W���擾����
  POINT pos;
  GetCursorPos(&pos);
  //�N���C�A���g���W�ɕϊ�����
  ScreenToClient(hwnd_, &pos);
  current_position_.x = (float)pos.x;
  current_position_.y = (float)pos.y;

  previous_mouse_info_ = current_mouse_info_;
  CHECK_MOUSE_BUTTON_PRESS(MouseButton::Left);
  CHECK_MOUSE_BUTTON_PRESS(MouseButton::Middle);
  CHECK_MOUSE_BUTTON_PRESS(MouseButton::Right);
}
//�}�E�X�̍��̍��W���擾
const legend::math::Vector2& Mouse::GetMousePosition() const {
  return current_position_;
}
//�}�E�X�̃{�^���̉�����Ԃ𔻒�
bool Mouse::GetMouse(MouseButton button) const {
  return current_mouse_info_.at(button);
}
//�}�E�X�̃{�^���������ꂽ���ǂ�������
bool Mouse::GetMouseDown(MouseButton button) const {
  return current_mouse_info_.at(button) && previous_mouse_info_.at(button);
}
//�}�E�X�̃{�^���������ꂽ���ǂ�������
bool Mouse::GetMouseUp(MouseButton button) const {
  return !current_mouse_info_.at(button) && previous_mouse_info_.at(button);
}
//�}�E�X�̑O�񂩂�̈ړ��ʂ��擾
legend::math::Vector2 Mouse::GetMove() const {
  return current_position_ - previous_position_;
}
//�}�E�X�������Ă��邩�ǂ������肷��
bool Mouse::IsMouseVisible() const { return false; }
}  // namespace input
}  // namespace legend