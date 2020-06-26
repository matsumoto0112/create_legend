/**
 * @file mouse.h
 * @brief �}�E�X���͊Ǘ�
 */

#pragma once
#include <src/math/vector_2.h>

namespace Framework::Input {
namespace MouseButton {
/**
 * @enum Enum
 * @brief �}�E�X�̃{�^����`
 */
enum Enum {
  Left = VK_LBUTTON,
  Right = VK_RBUTTON,
  Middle = VK_MBUTTON,
};
};  // namespace MouseButton

/**
 * @class Mouse
 * @brief �}�E�X���Ǘ��N���X
 */
class mouse {
  using MouseButton = MouseButton::Enum;
  using MouseInfo = std::unordered_map<MouseButton, bool>;

 public:
  /**
   * @brief �R���X�g���N�^
   * @param hWnd �E�B���h�E�n���h��
   */
  mouse(HWND hWnd);
  /**
   * @brief �f�X�g���N�^
   */
  ~mouse();
  /**
   * @brief �X�V
   */
  void update();
  /**
   * @brief �}�E�X�̍��W���擾
   * @return �N���C�A���g�̈�ł̃}�E�X���W��Ԃ�
   */
  const legend::math::Vector2& getMousePosition() const;
  /**
   * @brief �}�E�X�̃{�^���������Ă��邩
   * @param button �{�^���̎��
   * @return �����Ă����ԂȂ�true��Ԃ�
   */
  bool getMouse(MouseButton button) const;
  /**
   * @brief �}�E�X�̃{�^������������
   * @param button �{�^���̎��
   * @return �������^�C�~���O�Ȃ�true��Ԃ�
   */
  bool getMouseDown(MouseButton button) const;
  /**
   * @brief �}�E�X�̃{�^���𗣂�����
   * @param button �{�^���̎��
   * @return �������^�C�~���O�Ȃ�true��Ԃ�
   */
  bool getMouseUp(MouseButton button) const;
  /**
   * @brief �}�E�X�̈ړ��ʂ��擾
   */
  legend::math::Vector2 getMove() const;
  /**
   * @brief �}�E�X���o�����Ă��邩�ǂ������肷��
   */
  bool isMouseVisible() const;

 private:
  HWND mHWnd;                           //!< �E�B���h�E�n���h��
  legend::math::Vector2 mPosition;      //!< ���̃}�E�X�̍��W
  legend::math::Vector2 mPrevPosition;  //!< �O�̃}�E�X�̍��W
  MouseInfo mPrevMouseInfo;  //!< �O�t���[���̃}�E�X�̃{�^���̏��
  MouseInfo mCurrentMouseInfo;  //!< ���t���[���̃}�E�X�̃{�^���̏��
};
}  // namespace Framework::Input
