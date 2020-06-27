#ifndef LEGEND_INPUT_INPUTMANAGER_H_
#define LEGEND_INPUT_INPUTMANAGER_H_

/**
 * @file InputManager.h
 * @brief �Q�[���ɂ�������͑S�ʂ��Ǘ�����
 */

#include "src/input/gamePad.h"
#include "src/input/keyboard.h"
#include "src/input/mouse.h"

namespace legend {
namespace input {
/**
 * @class InputManager
 * @brief ���͂��Ǘ�����
 */
class InputManager {
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
  inline Keyboard* GetKeyboard() const { return mKeyboard_.get(); }
  /**
   * @brief �}�E�X���擾����
   */
  inline Mouse* GetMouse() const { return mMouse_.get(); }
  /**
   * @brief �Q�[���p�b�h���擾����
   */
  inline GamePad* GetGamepad() const { return mGamePad_.get(); }

 private:
  std::unique_ptr<Keyboard> mKeyboard_;  //!< �L�[�{�[�h
  std::unique_ptr<Mouse> mMouse_;        //!< �}�E�X
  std::unique_ptr<GamePad> mGamePad_;    //!< �Q�[���p�b�h
};
}  // namespace input
}  // namespace legend

#endif  // !LEGEND_INPUT_INPUTMANAGER_H_