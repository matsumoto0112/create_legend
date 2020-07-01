#ifndef LEGEND_INPUT_INPUT_MANAGER_H_
#define LEGEND_INPUT_INPUT_MANAGER_H_

/**
 * @file InputManager.h
 * @brief �Q�[���ɂ�������͑S�ʂ��Ǘ�����
 */

#include <functional>
#include <unordered_map>

#include "src/input/keyboard.h"
#include "src/input/mouse.h"
#include "src/input/game_pad.h"
#include "src/input/input_command.h"

namespace legend {
namespace input {
/**
 * @class InputManager
 * @brief ���͂��Ǘ�����
 */
class InputManager {
  using InputCode = input_code::Enum;

 public:
  /**
   * @brief �R���X�g���N�^
   * @param hWnd �E�B���h�E�n���h��
   */
  InputManager(HWND hWnd);
  /**
   * @brief �f�X�g���N�^
   */
  ~InputManager();
  /**
   * @brief ���͏����X�V����
   */
  void Update();
  /**
   * @brief �L�[�{�[�h���擾����
   */
  inline Keyboard* GetKeyboard() const { return input_keyboard_.get(); }
  /**
   * @brief �}�E�X���擾����
   */
  inline Mouse* GetMouse() const { return input_mouse_.get(); }
  /**
   * @brief �Q�[���p�b�h���擾����
   */
  inline GamePad* GetGamepad() const { return input_game_pad_.get(); }
  /**
   * @brief �Q�[���p�b�h���擾����
   */
  bool GetCommand(InputCode code) const;

  /**
   * @brief �����ړ����͎擾
   */
  float GetHorizontal() const;
  /**
   * @brief �c���ړ����͎擾
   */
  float GetVertical() const;

 private:
  std::unique_ptr<Keyboard> input_keyboard_;  //!< �L�[�{�[�h
  std::unique_ptr<Mouse> input_mouse_;        //!< �}�E�X
  std::unique_ptr<GamePad> input_game_pad_;   //!< �Q�[���p�b�h

  std::unique_ptr<InputCommand> input_command_;  //!< �R�}���h����
};
}  // namespace input
}  // namespace legend

#endif  // !LEGEND_INPUT_INPUT_MANAGER_H_