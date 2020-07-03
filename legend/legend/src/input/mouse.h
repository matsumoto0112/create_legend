#ifndef LEGEND_INPUT_MOUSE_H_
#define LEGEND_INPUT_MOUSE_H_

/**
 * @file mouse.h
 * @brief �}�E�X���͊Ǘ�
 */

#include "src/math/vector_2.h"

namespace legend {
namespace input {
namespace mouse_button {
/**
 * @enum Enum
 * @brief �}�E�X�̃{�^����`
 */
enum Enum {
  Left = VK_LBUTTON,
  Right = VK_RBUTTON,
  Middle = VK_MBUTTON,
};
};  // namespace mouse_button

/**
 * @class Mouse
 * @brief �}�E�X���Ǘ��N���X
 */
class Mouse {
  using MouseButton = mouse_button::Enum;
  using MouseInfo = std::unordered_map<MouseButton, bool>;

 public:
  /**
   * @brief �R���X�g���N�^
   * @param hWnd �E�B���h�E�n���h��
   */
  Mouse(HWND hWnd);
  /**
   * @brief �f�X�g���N�^
   */
  ~Mouse();
  /**
   * @brief �X�V
   */
  void Update();
  /**
   * @brief �}�E�X�̍��W���擾
   * @return �N���C�A���g�̈�ł̃}�E�X���W��Ԃ�
   */
  const math::Vector2& GetMousePosition() const;
  /**
   * @brief �}�E�X�̃{�^���������Ă��邩
   * @param button �{�^���̎��
   * @return �����Ă����ԂȂ�true��Ԃ�
   */
  bool GetMouse(MouseButton button) const;
  /**
   * @brief �}�E�X�̃{�^������������
   * @param button �{�^���̎��
   * @return �������^�C�~���O�Ȃ�true��Ԃ�
   */
  bool GetMouseDown(MouseButton button) const;
  /**
   * @brief �}�E�X�̃{�^���𗣂�����
   * @param button �{�^���̎��
   * @return �������^�C�~���O�Ȃ�true��Ԃ�
   */
  bool GetMouseUp(MouseButton button) const;
  /**
   * @brief �}�E�X�̈ړ��ʂ��擾
   */
  math::Vector2 GetMove() const;
  /**
   * @brief �}�E�X���o�����Ă��邩�ǂ������肷��
   */
  bool IsMouseVisible() const;

 private:
  HWND hwnd_;                           //!< �E�B���h�E�n���h��
  math::Vector2 current_position_;      //!< ���̃}�E�X�̍��W
  math::Vector2 previous_position_;  //!< �O�̃}�E�X�̍��W
  MouseInfo previous_mouse_info_;  //!< �O�t���[���̃}�E�X�̃{�^���̏��
  MouseInfo current_mouse_info_;  //!< ���t���[���̃}�E�X�̃{�^���̏��
};
}  // namespace input
}  // namespace legend

#endif  // !LEGEND_INPUT_MOUSE_H_