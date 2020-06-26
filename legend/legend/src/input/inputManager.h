/**
 * @file InputManager.h
 * @brief �Q�[���ɂ�������͑S�ʂ��Ǘ�����
 */

#pragma once
#include "src/input/keyboard.h"
#include "src/input/mouse.h"
#include "src/input/gamePad.h"

namespace Framework::Input {
/**
 * @class InputManager
 * @brief ���͂��Ǘ�����
 */
class inputManager {
 public:
  /**
   * @brief �R���X�g���N�^
   * @param hWnd �E�B���h�E�n���h��
   */
  inputManager(HWND hWnd);
  /**
   * @brief �f�X�g���N�^
   */
  ~inputManager();
  /**
   * @brief ���͏����X�V����
   */
  void update();
  /**
   * @brief �L�[�{�[�h���擾����
   */
  inline keyboard* getKeyboard() const { return mKeyboard.get(); }
  /**
   * @brief �}�E�X���擾����
   */
  inline mouse* getMouse() const { return mMouse.get(); }
  /**
   * @brief �Q�[���p�b�h���擾����
   */
  inline gamePad* getGamepad() const { return mGamepad.get(); }

 private:
  std::unique_ptr<keyboard> mKeyboard;  //!< �L�[�{�[�h
  std::unique_ptr<mouse> mMouse;        //!< �}�E�X
  std::unique_ptr<gamePad> mGamepad;          //!< �Q�[���p�b�h
};
}  // namespace Framework::Input
