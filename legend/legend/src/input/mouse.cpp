#include "src/input/mouse.h"

namespace legend {
namespace input {
//�R���X�g���N�^
Mouse::Mouse(HWND hWnd) : mHWnd_(hWnd) {
#define CHECK_MOUSE_BUTTON_PRESS(key) \
  { mCurrentMouseInfo_[key] = GetKeyState(key) & 0x80; }

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
  mPrevPosition_ = mPosition_;

  //�X�N���[�����W���擾����
  POINT pos;
  GetCursorPos(&pos);
  //�N���C�A���g���W�ɕϊ�����
  ScreenToClient(mHWnd_, &pos);
  mPosition_.x = (float)pos.x;
  mPosition_.y = (float)pos.y;

  mPrevMouseInfo_ = mCurrentMouseInfo_;
  CHECK_MOUSE_BUTTON_PRESS(MouseButton::Left);
  CHECK_MOUSE_BUTTON_PRESS(MouseButton::Middle);
  CHECK_MOUSE_BUTTON_PRESS(MouseButton::Right);
}
//�}�E�X�̍��̍��W���擾
const legend::math::Vector2& Mouse::GetMousePosition() const {
  return mPosition_;
}
//�}�E�X�̃{�^���̉�����Ԃ𔻒�
bool Mouse::GetMouse(MouseButton button) const {
  return mCurrentMouseInfo_.at(button);
}
//�}�E�X�̃{�^���������ꂽ���ǂ�������
bool Mouse::GetMouseDown(MouseButton button) const {
  return mCurrentMouseInfo_.at(button) && mPrevMouseInfo_.at(button);
}
//�}�E�X�̃{�^���������ꂽ���ǂ�������
bool Mouse::GetMouseUp(MouseButton button) const {
  return !mCurrentMouseInfo_.at(button) && mPrevMouseInfo_.at(button);
}
//�}�E�X�̑O�񂩂�̈ړ��ʂ��擾
legend::math::Vector2 Mouse::GetMove() const {
  return mPosition_ - mPrevPosition_;
}
//�}�E�X�������Ă��邩�ǂ������肷��
bool Mouse::IsMouseVisible() const { return false; }
}  // namespace input
}  // namespace legend