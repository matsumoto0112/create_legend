#ifndef LEGEND_CAMERA_PERSPECTIVE_CAMERA_H_
#define LEGEND_CAMERA_PERSPECTIVE_CAMERA_H_

/**
 * @file perspective_camera.h
 * @brief �p�[�X�y�N�e�B�u�J�����N���X��`
 */

#include "src/camera/camera.h"
#include "src/math/quaternion.h"
#include "src/math/vector_3.h"

namespace legend {
namespace camera {

/**
 * @class PerspectiveCamera
 * @brief �p�[�X�y�N�e�B�u�J�����N���X
 */
class PerspectiveCamera : public Camera {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  PerspectiveCamera();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~PerspectiveCamera();
  /**
   * @brief ������
   * @param name �J������
   * @param position �J�������W
   * @param rotation �J�����̉�]�p
   * @param fov ����p(rad)
   * @param aspect_ratio �A�X�y�N�g��
   * @param up_vector �J�����̏�����x�N�g��
   * @param near_z �ŋߓ_
   * @param far_z �ŉ��_
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(const std::wstring& name, const math::Vector3& position,
            const math::Quaternion& rotation, float fov, float aspect_ratio,
            const math::Vector3& up_vector = math::Vector3::kUpVector,
            float near_z = 0.1f, float far_z = 300.0f);

 protected:
  /**
   * * @brief �R���X�^���g�o�b�t�@�̏�Ԃ��X�V����
   */
  virtual void UpdateConstantBufferStaging() override;

 private:
  /**
   * @brief �r���[�s����쐬����
   */
  math::Matrix4x4 CreateView() const;
  /**
   * @brief �v���W�F�N�V�����s����쐬����
   */
  math::Matrix4x4 CreateProjection() const;

 public:
  /**
   * @brief ���W��ݒ肷��
   */
  inline void SetPosition(const math::Vector3& position) {
    this->position_ = position;
  }
  /**
   * @brief ���W���擾����
   */
  inline math::Vector3 GetPosition() const { return position_; }
  /**
   * @brief ��]�p��ݒ肷��
   */
  inline void SetRotation(const math::Quaternion& rotation) {
    this->rotation_ = rotation;
  }
  /**
   * @brief ��]�p���擾����
   */
  inline math::Quaternion GetRotation() const { return rotation_; }
  /**
   * @brief ������x�N�g����ݒ肷��
   */
  inline void SetUpVector(const math::Vector3& up_vector) {
    this->up_vector_ = up_vector;
  }
  /**
   * @brief ������x�N�g�����擾����
   */
  inline math::Vector3 GetUpVector() const { return up_vector_; }
  /**
   * @brief ����p��ݒ肷��
   * @param fov �p�x(rad)
   */
  inline void SetFov(float fov) { this->fov_ = fov; }
  /**
   * @brief ����p���擾����
   */
  inline float GetFov() const { return fov_; }
  /**
   * @brief �A�X�y�N�g���ݒ肷��
   */
  inline void SetAspectRatio(float ratio) { this->aspect_ratio_ = ratio; }
  /**
   * @brief �ݒ肳��Ă���A�X�y�N�g����擾����
   */
  inline float GetAspectRatio() const { return aspect_ratio_; }
  /**
   * @brief �ŋߓ_�܂ł̋�����ݒ肷��
   */
  inline void SetNearZ(float z) { this->near_z_ = z; }
  /**
   * @brief �ŋߓ_�܂ł̋������擾����
   */
  inline float GetNearZ() const { return near_z_; }
  /**
   * @brief �ŉ��_�܂ł̋�����ݒ肷��
   */
  inline void SetFarZ(float z) { this->far_z_ = z; }
  /**
   * @brief �ŉ��_�܂ł̋������擾����
   */
  inline float GetFarZ() const { return far_z_; }

 protected:
  //! �J�������W
  math::Vector3 position_;
  //! �J�����̉�]�p
  math::Quaternion rotation_;
  //! �J�����̏�����x�N�g��
  math::Vector3 up_vector_;
  //! ����p(rad)
  float fov_;
  //! �A�X�y�N�g��
  float aspect_ratio_;
  //! �ŋߓ_
  float near_z_;
  //! �ŉ��_
  float far_z_;
};

}  // namespace camera
}  // namespace legend

#endif  //! LEGEND_CAMERA_PERSPECTIVE_CAMERA_H_
