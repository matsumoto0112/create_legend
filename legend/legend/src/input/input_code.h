#ifndef LEGEND_INPUT_INPUT_CODE_H_
#define LEGEND_INPUT_INPUT_CODE_H_

/**
 * @file input_code.h
 * @brief ���͒�`
 */

namespace legend {
namespace input {
namespace input_code {
/**
 * @enum Enum
 * @brief �L�[�ԍ�
 */
enum Enum : i32 {
  End = -1,       //!< �I���R�}���h
  Decide = 0,     //!< ����R�}���h
  Pause = 1,      //!< �ҋ@�R�}���h
  CANCEL,         //! �L�����Z���{�^��
  CAMERA_CHANGE,  //!< �J�����؂�ւ�
};
}  // namespace input_code
}  // namespace input
}  // namespace legend

#endif  // !LEGEND_INPUT_INPUT_CODE_H_