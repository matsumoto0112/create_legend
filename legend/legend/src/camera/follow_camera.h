#ifndef LEGEND_CAMERA_FOLLOW_CAMERA_H
#define LEGEND_CAMERA_FOLLOW_CAMERA_H

/**
 * @file follow_camera.h
 * @brief �^�[�Q�b�g��ǔ�����J����
 */

#include "src/actor/actor.h"
#include "src/camera/perspective_camera.h"

namespace legend {
namespace camera {

/**
 * @brief �ǔ��J����
 */
class FollowCamera : public PerspectiveCamera {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  FollowCamera();
  /**
   * @breif �f�X�g���N�^
   */
  virtual ~FollowCamera();
  /**
   * @brief ������
   * @param name �J������
   * @param target �ǔ��Ώۂ̃J����
   * @param offset �ǔ��ΏۂƗ����I�t�Z�b�g�l
   * @param rotation �J�����̉�]
   * @param fov FOV�l�i���W�A���j
   * @param aspect_ratio �A�X�y�N�g��
   * @param up_vector �J�����̏�����x�N�g��
   * @param near_z �ŋߓ_
   * @param far_z �ŉ��_
   */
  bool Init(const std::wstring& name, actor::Actor* target,
            const math::Vector3& offset, const math::Quaternion& rotation,
            float fov, float aspect_ratio,
            const math::Vector3& up_vector = math::Vector3::kUpVector,
            float near_z = 0.1f, float far_z = 300.0f);

 protected:
  /**
   * * @brief �R���X�^���g�o�b�t�@�̏�Ԃ��X�V����
   */
  virtual void UpdateConstantBufferStaging() override;

 private:
  //! �ǔ��ΏۂƂȂ�A�N�^�[
  actor::Actor* follow_target_;
  //! �I�t�Z�b�g
  math::Vector3 offset_;
};

}  // namespace camera
}  // namespace legend

#endif  //! LEGEND_CAMERA_FOLLOW_CAMERA_H
