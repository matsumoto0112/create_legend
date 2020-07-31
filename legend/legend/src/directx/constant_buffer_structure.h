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
 * @brief ���[���h�s��f�[�^�\����
 */
struct Transform {
  //! ���[���h�ϊ��s��
  math::Matrix4x4 world;
};

/**
 * @brief ���[���h�`��p�f�[�^�\����
 */
struct WorldContext {
  //! �r���[�s��
  math::Matrix4x4 view;
  //! �v���W�F�N�V�����s��
  math::Matrix4x4 projection;
};

struct UVRect {
  float left;
  float top;
  float width;
  float height;
};

}  // namespace constant_buffer_structure
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_CONSTANT_BUFFER_STRUCTURE_H_
