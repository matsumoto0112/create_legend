#ifndef LEGEND_CAMERA_LOOKAT_TARGET_CAMERA_H
#define LEGEND_CAMERA_LOOKAT_TARGET_CAMERA_H

/**
 * @file lookat_target_camera.h
 * @brief �^�[�Q�b�g�𒍎�����J�����N���X��`
 */

#include "src/actor/actor.h"
#include "src/camera/perspective_camera.h"

namespace legend {
namespace camera {

/**
 * @brief �^�[�Q�b�g�𒍎�����J����
 */
class LookatTargetCamera : public PerspectiveCamera {
 public:
  LookatTargetCamera();
  virtual ~LookatTargetCamera();
  /**
   * @brief ������
   * @param name �J������
   * @param target ��������Ώ�
   * @param position �ݒu������W
   * @param fov FOV
   * @param ratio �A�X�y�N�g��
   * @param up_vector �J�����̏����
   * @param near_z �ŋߓ_
   * @param far_z �ŉ��_
   */
  virtual bool Init(const std::wstring& name, actor::Actor* target,
                    const math::Vector3& position, float fov,
                    float aspect_ratio,
                    const math::Vector3& up_vector = math::Vector3::kUpVector,
                    float near_z = 0.1f, float far_z = 300.0f);

  void SetTarget(actor::Actor* target) { this->target_ = target; }
  actor::Actor* GetTarget() const { return target_; }

 protected:
  /**
   * * @brief �R���X�^���g�o�b�t�@�̏�Ԃ��X�V����
   */
  virtual void UpdateConstantBufferStaging() override;

 private:
  actor::Actor* target_;
};
}  // namespace camera
}  // namespace legend
#endif  //! LEGEND_CAMERA_LOOKAT_TARGET_CAMERA_H
