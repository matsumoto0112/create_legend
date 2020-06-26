/**
 * @file gamePad.h
 * @brief �Q�[���p�b�h���͊Ǘ�
 */

#pragma once
#pragma comment(lib, "winmm.lib")

#include <stdio.h>
#include <windows.h>
#include <src/math/vector_2.h>
#include "src/input/joyCode.h"

namespace Framework::Input {
class gamePad {
  static constexpr int PAD_MAX = 4;  //!< �Q�[���p�b�h�ő吔
  using JoyCode = joyCode::Enum;
  using Vector2 = legend::math::Vector2;

 public:
  /**
   * @brief �R���X�g���N�^
   * @param hWnd �E�B���h�E�n���h��
   */
  gamePad(HWND hWnd);
  /**
   * @brief �f�X�g���N�^
   */
  ~gamePad();
  /**
   * @brief �X�V
   */
  void update();
  /**
   * @brief �ڑ���
   * @return �ڑ���
   */
  int joyCount();

  /**
  * @brief �Q�[���p�b�h�̍��X�e�B�b�N�̈ړ��l
  * @param index �Ή�����R���g���[���[
  * @return ���X�e�B�b�N�̈ړ��l
  �����Ă��Ȃ��A�R���g���[�������݂��Ȃ��ꍇ��0
  */
  Vector2 getStickLeft(unsigned int index);
  /**
  * @brief �Q�[���p�b�h�̉E�X�e�B�b�N�̈ړ��l
  * @param index �Ή�����R���g���[���[
  * @return �E�X�e�B�b�N�̈ړ��l
  �����Ă��Ȃ��A�R���g���[�������݂��Ȃ��ꍇ��0
  */
  Vector2 getStickRight(unsigned int index);
  /**
  * @brief �Q�[���p�b�h�̏\���L�[�̈ړ��l
  * @param index �Ή�����R���g���[���[
  * @return �\���L�[�̈ړ��l
  �����Ă��Ȃ��A�R���g���[�������݂��Ȃ��ꍇ��0
  */
  Vector2 getCrossKey(unsigned int index);

  /**
  * @brief �L�[�������Ă��邩
  * @param joy ���肷��L�[
  * @return ��update�ŉ����Ă�����true
  ������Ă��Ȃ��A�R���g���[�������݂��Ȃ��ꍇ��false
  */
  bool getButton(JoyCode joyCode) const;
  /**
  * @brief �L�[�����������i�g���K�[�j
  * @param joy ���肷��L�[
  * @return �Oupdate�ŉ�����Ă��Ȃ�����update�ŉ�����Ă�����true
  ����ȊO�̏ꍇ��false
  */
  bool getButtonDown(JoyCode joyCode) const;
  /**
  * @brief �L�[�𗣂������i�g���K�[�j
  * @param joy ���肷��L�[
  * @return �Oupdate�ŉ�����Ă��Ă���update�ŗ����ꂽ��true
  ����ȊO�̏ꍇ��false
  */
  bool getButtonUp(JoyCode joyCode) const;

 private:
  /**
   * @brief �Q�[���p�b�h�̏�ԍX�V
   */
  void setState();
  /**
   * @brief ���擾���A�G���[���N���邩
   * @return �ڑ��G���[���N���Ȃ����true
   */
  bool noError(unsigned int index) const;

  /**
   * @brief �Q�[���p�b�h�̃X�e�B�b�NX���̈ړ��l�擾
   * @return �X�e�B�b�N��X��
   */
  float xPos(unsigned int index);
  /**
   * @brief �Q�[���p�b�h�̃X�e�B�b�NY���̈ړ��l�擾
   * @return �X�e�B�b�N��Y��
   */
  float yPos(unsigned int index);
  /**
   * @brief �Q�[���p�b�h�̃X�e�B�b�NZ���̈ړ��l�擾
   * @return �X�e�B�b�N��Z��
   */
  float zPos(unsigned int index);
  /**
   * @brief �Q�[���p�b�h�̃X�e�B�b�NU���̈ړ��l�擾
   * @return �X�e�B�b�N��U��
   */
  float uPos(unsigned int index);
  /**
   * @brief �Q�[���p�b�h�̃X�e�B�b�NV���̈ړ��l�擾
   * @return �X�e�B�b�N��V��
   */
  float vPos(unsigned int index);
  /**
   * @brief �Q�[���p�b�h�̃X�e�B�b�NR���̈ړ��l�擾
   * @return �X�e�B�b�N��R��
   */
  float rPos(unsigned int index);

  /**
   * @brief �Q�[���p�b�h�̏\���L�[�����̈ړ��l�擾
   * @return �\���L�[�̉���
   */
  float getCrossHorizontal(unsigned int index);
  /**
   * @brief �Q�[���p�b�h�̏\���L�[�c���̈ړ��l�擾
   * @return �\���L�[�̏c��
   */
  float getCrossVertical(unsigned int index);

 private:
  HWND mHWnd;        //!< �E�B���h�E�n���h��
  int padCount = 0;  //!< �Q�[���p�b�h��

  std::vector<JOYINFOEX> mCurrentJoys;  //!< ���݂̃Q�[���p�b�h�̏��
  std::vector<unsigned long> mPreJoys;  //!< �O��̃Q�[���p�b�h�̃{�^�����擾
  std::vector<MMRESULT> mResultJoys;  //!< �Q�[���p�b�h���擾�ł��邩�̔���
};

}  // namespace Framework::Input
