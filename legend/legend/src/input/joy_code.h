#ifndef LEGEND_INPUT_JOY_CODE_H_
#define LEGEND_INPUT_JOY_CODE_H_

/**
 * @file joyCode.h
 * @brief �Q�[���p�b�h��`
 */

namespace legend {
namespace input {
namespace joy_code {
/**
 * @enum Enum
 * @brief �L�[�ԍ�
 */
enum Enum : i32 {
  A = 1,                 //!< A�{�^��
  B = 2,                 //!< B�{�^��
  X = 4,                 //!< X�{�^��
  Y = 8,                 //!< Y�{�^��
  LB = 16,               //!< LB�{�^��
  RB = 32,               //!< RB�{�^��
  SELECT = 64,           //!< SELECT�{�^��
  START = 128,           //!< START�{�^��
  LEFT_STICK = 256,      //!< ���X�e�B�b�N�{�^��
  RIGHT_STICK = 512,     //!< �E�X�e�B�b�N�{�^��
};
}  // namespace joy_code
}  // namespace input
}  // namespace legend

#endif  // !LEGEND_INPUT_JOY_CODE_H_