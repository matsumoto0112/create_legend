#ifndef LEGEND_INPUT_KEYBOARD_H_
#define LEGEND_INPUT_KEYBOARD_H_

/**
 * @file keyboard.h
 * @brief �L�[�{�[�h���͊Ǘ�
 */

#include "src/input/key_code.h"

namespace legend {
namespace input {
/**
 * @class keyboard
 * @brief �L�[�{�[�h���͊Ǘ��N���X
 */
class Keyboard {
  static constexpr i32 KEY_MAX = 256;  //!< �L�[�ő吔
  using KeyInfo = std::array<BYTE, KEY_MAX>;
  using KeyCode = key_code::Enum;

 public:
  /**
   * @brief �R���X�g���N�^
   * @param hWnd �E�B���h�E�n���h��
   */
  Keyboard(HWND hWnd);
  /**
   * @brief �f�X�g���N�^
   */
  ~Keyboard();
  /**
   * @brief �X�V
   */
  void Update();
  /**
  * @brief �L�[�������Ă��邩
  * @param key ���肷��L�[
  * @return ��update�ŉ����Ă�����true
  ������Ă��Ȃ��A�R���g���[�������݂��Ȃ��ꍇ��false
  */
  bool GetKey(KeyCode key) const;
  /**
  * @brief �L�[�����������i�g���K�[�j
  * @param key ���肷��L�[
  * @return �Oupdate�ŉ�����Ă��Ȃ�����update�ŉ�����Ă�����true
  ����ȊO�̏ꍇ��false
  */
  bool GetKeyDown(KeyCode key) const;
  /**
  * @brief �L�[�𗣂������i�g���K�[�j
  * @param key ���肷��L�[
  * @return �Oupdate�ŉ�����Ă��Ă���update�ŗ����ꂽ��true
  ����ȊO�̏ꍇ��false
  */
  bool GetKeyUp(KeyCode key) const;

 private:
  KeyInfo current_keys_;  //!< ��update�Ŏ擾�����L�[���
  KeyInfo previous_keys_;     //!< �Oupdate�Ŏ擾�����L�[���
 private:
  /**
   * @brief �L�[�������Ă��邩
   * @param keys ����Ɏg���L�[�z��
   * @param key ���肷��L�[
   */
  bool CheckKeyDown(const KeyInfo& keys, KeyCode key) const;
};

}  // namespace input
}  // namespace legend

#endif  // !LEGEND_INPUT_KEYBOARD_H_