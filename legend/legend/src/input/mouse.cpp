#include "src/input/mouse.h"

namespace Framework::Input {
//�R���X�g���N�^
mouse::mouse(HWND hWnd) : mHWnd(hWnd) {
#define CHECK_MOUSE_BUTTON_PRESS(key) \
  { mCurrentMouseInfo[key] = GetKeyState(key) & 0x80; }

  //�ŏ��ɉ�����Ԃ𒲂ׁA�}�b�v���쐬���Ă���
  CHECK_MOUSE_BUTTON_PRESS(MouseButton::Left);
  CHECK_MOUSE_BUTTON_PRESS(MouseButton::Middle);
  CHECK_MOUSE_BUTTON_PRESS(MouseButton::Right);
}
//�f�X�g���N�^
mouse::~mouse() {}
//�X�V
void mouse::update() {
  //�O�̍��W���X�V����
  mPrevPosition = mPosition;

  //�X�N���[�����W���擾����
  POINT pos;
  GetCursorPos(&pos);
  //�N���C�A���g���W�ɕϊ�����
  ScreenToClient(mHWnd, &pos);
  mPosition.x = (float)pos.x;
  mPosition.y = (float)pos.y;

  mPrevMouseInfo = mCurrentMouseInfo;
  CHECK_MOUSE_BUTTON_PRESS(MouseButton::Left);
  CHECK_MOUSE_BUTTON_PRESS(MouseButton::Middle);
  CHECK_MOUSE_BUTTON_PRESS(MouseButton::Right);
}
//�}�E�X�̍��̍��W���擾
const legend::math::Vector2& mouse::getMousePosition() const { return mPosition; }
//�}�E�X�̃{�^���̉�����Ԃ𔻒�
bool mouse::getMouse(MouseButton button) const {
  return mCurrentMouseInfo.at(button);
}
//�}�E�X�̃{�^���������ꂽ���ǂ�������
bool mouse::getMouseDown(MouseButton button) const {
  return mCurrentMouseInfo.at(button) && mPrevMouseInfo.at(button);
}
//�}�E�X�̃{�^���������ꂽ���ǂ�������
bool mouse::getMouseUp(MouseButton button) const {
  return !mCurrentMouseInfo.at(button) && mPrevMouseInfo.at(button);
}
//�}�E�X�̑O�񂩂�̈ړ��ʂ��擾
legend::math::Vector2 mouse::getMove() const { return mPosition - mPrevPosition; }
//�}�E�X�������Ă��邩�ǂ������肷��
bool mouse::isMouseVisible() const { return false; }
}  // namespace Framework::Input
