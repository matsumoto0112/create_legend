#ifndef LEGEND_INPUT_GAME_PAD_H_
#define LEGEND_INPUT_GAME_PAD_H_

/**
 * @file gamePad.h
 * @brief �Q�[���p�b�h���͊Ǘ�
 */

#pragma comment(lib, "winmm.lib")

#include "src/input/joy_code.h"
#include "src/math/vector_2.h"

namespace legend {
namespace input {
class GamePad {
  static constexpr i32 PAD_MAX = 4;  //!< �Q�[���p�b�h�ő吔
  using JoyCode = joy_code::Enum;

 public:
  /**
   * @brief �R���X�g���N�^
   * @param hWnd �E�B���h�E�n���h��
   */
  GamePad(HWND hWnd);
  /**
   * @brief �f�X�g���N�^
   */
  ~GamePad();
  /**
   * @brief �X�V
   */
  void Update();
  /**
   * @brief �ڑ���
   * @return �ڑ���
   */
  i32 GetCount() const;

  /**
  * @brief �Q�[���p�b�h�̍��X�e�B�b�N�̈ړ��l
  * @param index �Ή�����R���g���[���[
  * @return ���X�e�B�b�N�̈ړ��l
  �����Ă��Ȃ��A�R���g���[�������݂��Ȃ��ꍇ��0
  */
  math::Vector2 GetStickLeft(u32 index) const;
  /**
  * @brief �Q�[���p�b�h�̉E�X�e�B�b�N�̈ړ��l
  * @param index �Ή�����R���g���[���[
  * @return �E�X�e�B�b�N�̈ړ��l
  �����Ă��Ȃ��A�R���g���[�������݂��Ȃ��ꍇ��0
  */
  math::Vector2 GetStickRight(u32 index) const;
  /**
  * @brief �Q�[���p�b�h�̏\���L�[�̈ړ��l
  * @param index �Ή�����R���g���[���[
  * @return �\���L�[�̈ړ��l
  �����Ă��Ȃ��A�R���g���[�������݂��Ȃ��ꍇ��0
  */
  math::Vector2 GetCrossKey(u32 index) const;

  /**
  * @brief �L�[�������Ă��邩
  * @param joy ���肷��L�[
  * @param index ���肷��R���g���[���[
  * @return ��update�ŉ����Ă�����true
  ������Ă��Ȃ��A�R���g���[�������݂��Ȃ��ꍇ��false
  */
  bool GetButton(JoyCode joyCode, u32 index) const;
  /**
  * @brief �L�[�����������i�g���K�[�j
  * @param joy ���肷��L�[
  * @param index ���肷��R���g���[���[
  * @return �Oupdate�ŉ�����Ă��Ȃ�����update�ŉ�����Ă�����true
  ����ȊO�̏ꍇ��false
  */
  bool GetButtonDown(JoyCode joyCode, u32 index) const;
  /**
  * @brief �L�[�𗣂������i�g���K�[�j
  * @param joy ���肷��L�[
  * @param index ���肷��R���g���[���[
  * @return �Oupdate�ŉ�����Ă��Ă���update�ŗ����ꂽ��true
  ����ȊO�̏ꍇ��false
  */
  bool GetButtonUp(JoyCode joyCode, u32 index) const;

  /**
  * @brief �L�[�������Ă��邩
  * @param joy ���肷��L�[
  * @return ��update�ŉ����Ă�����true
  ������Ă��Ȃ��A�R���g���[�������݂��Ȃ��ꍇ��false
  */
  bool GetButton(JoyCode joyCode) const;
  /**
  * @brief �L�[�����������i�g���K�[�j
  * @param joy ���肷��L�[
  * @return �Oupdate�ŉ�����Ă��Ȃ�����update�ŉ�����Ă�����true
  ����ȊO�̏ꍇ��false
  */
  bool GetButtonDown(JoyCode joyCode) const;
  /**
  * @brief �L�[�𗣂������i�g���K�[�j
  * @param joy ���肷��L�[
  * @return �Oupdate�ŉ�����Ă��Ă���update�ŗ����ꂽ��true
  ����ȊO�̏ꍇ��false
  */
  bool GetButtonUp(JoyCode joyCode) const;

 private:
  HWND hwnd_;        //!< �E�B���h�E�n���h��
  i32 pad_count_ = 0;  //!< �Q�[���p�b�h��

  std::vector<JOYINFOEX> current_joys_;  //!< ���݂̃Q�[���p�b�h�̏��
  std::vector<u64> previous_joys_;  //!< �O��̃Q�[���p�b�h�̃{�^�����擾
  std::vector<MMRESULT> result_joys_;  //!< �Q�[���p�b�h���擾�ł��邩�̔���

 private:
  /**
   * @brief �Q�[���p�b�h�̏�ԍX�V
   */
  void SetState();
  /**
   * @brief ���擾���A�G���[���N���邩
   * @return �ڑ��G���[���N���Ȃ����true
   */
  bool NoError(u32 index) const;

  /**
   * @brief �Q�[���p�b�h�̃X�e�B�b�NX���̈ړ��l�擾
   * @return �X�e�B�b�N��X��
   */
  float Stick_X_Pos(u32 index) const;
  /**
   * @brief �Q�[���p�b�h�̃X�e�B�b�NY���̈ړ��l�擾
   * @return �X�e�B�b�N��Y��
   */
  float Stick_Y_Pos(u32 index) const;
  /**
   * @brief �Q�[���p�b�h�̃X�e�B�b�NZ���̈ړ��l�擾
   * @return �X�e�B�b�N��Z��
   */
  float Stick_Z_Pos(u32 index) const;
  /**
   * @brief �Q�[���p�b�h�̃X�e�B�b�NU���̈ړ��l�擾
   * @return �X�e�B�b�N��U��
   */
  float Stick_U_Pos(u32 index) const;
  /**
   * @brief �Q�[���p�b�h�̃X�e�B�b�NV���̈ړ��l�擾
   * @return �X�e�B�b�N��V��
   */
  float Stick_V_Pos(u32 index) const;
  /**
   * @brief �Q�[���p�b�h�̃X�e�B�b�NR���̈ړ��l�擾
   * @return �X�e�B�b�N��R��
   */
  float Stick_R_Pos(u32 index) const;

  /**
   * @brief �Q�[���p�b�h�̏\���L�[�����̈ړ��l�擾
   * @return �\���L�[�̉���
   */
  float GetCrossHorizontal(u32 index) const;
  /**
   * @brief �Q�[���p�b�h�̏\���L�[�c���̈ړ��l�擾
   * @return �\���L�[�̏c��
   */
  float GetCrossVertical(u32 index) const;
};

}  // namespace input
}  // namespace legend

#endif  // !LEGEND_INPUT_GAME_PAD_H_