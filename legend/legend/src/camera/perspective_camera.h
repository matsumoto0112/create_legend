#ifndef LEGEND_CAMERA_PERSPECTIVE_CAMERA_H_
#define LEGEND_CAMERA_PERSPECTIVE_CAMERA_H_

/**
 * @file perspective_camera.h
 * @brief �p�[�X�y�N�e�B�u�J�����N���X��`
 */

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/constant_buffer_structure.h"
#include "src/math/matrix_4x4.h"
#include "src/math/quaternion.h"
#include "src/math/vector_3.h"

namespace legend {
namespace camera {

/**
 * @brief �p�[�X�y�N�e�B�u�J�����N���X
 */
class PerspectiveCamera {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  PerspectiveCamera();
  /**
   * @brief �f�X�g���N�^
   */
  ~PerspectiveCamera();
  /**
   * @brief �J�����̏�����
   * @param name �J������
   * @param position ���W
   * @param lookat �����_
   * @param up_vector �J�����̏�����x�N�g��
   * @param fov ����p(rad)
   * @param aspect_ratio �A�X�y�N�g��
   * @param near_z �ŋߓ_
   * @param far_z �ŉ��_
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(const std::wstring& name, const math::Vector3& position,
            const math::Vector3& lookat, const math::Vector3& up_vector,
            float fov, float aspect_ratio, float near_z = 0.1f,
            float far_z = 300.0f);
  /**
   * @brief �`����J�n����
   */
  void RenderStart();

 public:
  /**
   * @brief ���O�̐ݒ�
   */
  inline void SetName(const std::wstring& name) { name_ = name; }
  /**
   * @brief ���O�̎擾
   */
  inline std::wstring GetName() const { return name_; }

 public:
  //! �J������
  std::wstring name_;
  //! �`��p�R���X�^���g�o�b�t�@
  directx::buffer::ConstantBuffer<
      directx::constant_buffer_structure::WorldContext>
      world_context_constant_buffer_;
};

}  // namespace camera
}  // namespace legend

#endif  //! LEGEND_CAMERA_PERSPECTIVE_CAMERA_H_
