#ifndef LEGEND_DIRECTX_CONSTANT_BUFFER_STRUCTURE_H_
#define LEGEND_DIRECTX_CONSTANT_BUFFER_STRUCTURE_H_

/**
 * @file constant_buffer_structure.h
 * @brief �R���X�^���g�o�b�t�@�Ŏg�p����\���̒�`
 */

#include "src/math/matrix_4x4.h"

namespace legend {
namespace directx {
namespace constant_buffer_structure {

/**
 * @brief ���[���h�`��p�f�[�^�\����
 */
struct WorldContext {
  //! �r���[�s��
  math::Matrix4x4 view;
  //! �v���W�F�N�V�����s��
  math::Matrix4x4 projection;
};
}  // namespace constant_buffer_structure
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_CONSTANT_BUFFER_STRUCTURE_H_
