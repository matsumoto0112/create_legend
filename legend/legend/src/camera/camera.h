#ifndef LEGEND_CAMERA_CAMERA_H
#define LEGEND_CAMERA_CAMERA_H

/**
 * @file camera.h
 * @brief �J�����N���X��`
 */

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/constant_buffer_structure.h"
#include "src/math/matrix_4x4.h"

namespace legend {
namespace camera {

/**
 * @brief �J�����N���X
 */
class Camera {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  Camera();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~Camera();
  /**
   * @brief ������
   * @param name �J������
   * @param view �r���[�s��
   * @param projection �v���W�F�N�V�����s��
   */
  virtual bool Init(const std::wstring& name, const math::Matrix4x4& view,
                    const math::Matrix4x4& projection);
  /**
   * @brief �`��J�n
   */
  virtual void RenderStart();
  /**
   * @brief ���O�̐ݒ�
   */
  inline void SetName(const std::wstring& name) { name_ = name; }
  /**
   * @brief ���O�̎擾
   */
  inline std::wstring GetName() const { return name_; }

  inline math::Matrix4x4 GetView() const { return view_; }
  inline math::Matrix4x4 GetProjection() const { return projection_; }

 protected:
  /**
   * @brief �R���X�^���g�o�b�t�@�̏�����
   */
  virtual bool InitConstantBuffer();
  /**
   * @brief �R���X�^���g�o�b�t�@���X�V����
   */
  virtual void UpdateConstantBufferStaging();

 protected:
  //! �J������
  std::wstring name_;
  //! �r���[�s��
  math::Matrix4x4 view_;
  //! �v���W�F�N�V�����s��
  math::Matrix4x4 projection_;

  //! �`��p�R���X�^���g�o�b�t�@
  using WorldContext = directx::buffer::constant_buffer_structure::WorldContext;
  using ConstantBuffer = directx::buffer::ConstantBuffer<WorldContext>;
  ConstantBuffer world_context_constant_buffer_;
};

}  // namespace camera
}  // namespace legend
#endif  //! LEGEND_CAMERA_CAMERA_H
