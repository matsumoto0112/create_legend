#ifndef LEGEND_CAMERA_LOOKAT_TARGET_CAMERA_H
#define LEGEND_CAMERA_LOOKAT_TARGET_CAMERA_H

/**
 * @file lookat_target_camera.h
 * @brief
 */

#include "src/actor/actor.h"
#include "src/camera/perspective_camera.h"

namespace legend {
namespace camera {
class LookatTargetCamera : public PerspectiveCamera {
 public:
  LookatTargetCamera();
  virtual ~LookatTargetCamera();
  virtual bool Init(const std::wstring& name, actor::Actor* target,
                    const math::Vector3& position, float fov,
                    float aspect_ratio,
                    const math::Vector3& up_vector = math::Vector3::kUpVector,
                    float near_z = 0.1f, float far_z = 300.0f);

  void SetTarget(actor::Actor* target) { this->target_ = target; }
  actor::Actor* GetTarget() const { return target_; }

 protected:
  /**
   * * @brief コンスタントバッファの状態を更新する
   */
  virtual void UpdateConstantBufferStaging() override;

 private:
  actor::Actor* target_;
};
}  // namespace camera
}  // namespace legend
#endif  //! LEGEND_CAMERA_LOOKAT_TARGET_CAMERA_H
