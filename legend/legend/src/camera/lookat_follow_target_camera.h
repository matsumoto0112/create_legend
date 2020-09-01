#ifndef LEGEND_CAMERA_LOOKAT_FOLLOW_TARGET_CAMERA_H
#define LEGEND_CAMERA_LOOKAT_FOLLOW_TARGET_CAMERA_H

/**
 * @file lookat_follow_target_camera.h
 * @brief
 * ターゲットとなるアクターを注視し、追尾するカメラ機能定義
 */

#include "src/actor/actor.h"
#include "src/camera/perspective_camera.h"

namespace legend {
namespace camera {
class LookatFollowTargetCamera : public PerspectiveCamera {
 public:
  LookatFollowTargetCamera();
  virtual ~LookatFollowTargetCamera();
  virtual bool Init(const std::wstring& name, actor::Actor* target,
                    float height, float distance, float fov, float aspect_ratio,
                    const math::Vector3& up_vector = math::Vector3::kUpVector,
                    float near_z = 0.1f, float far_z = 300.0f);

  void SetTarget(actor::Actor* target) { this->target_ = target; }
  actor::Actor* GetTarget() const { return target_; }
  void SetHeight(float height) { this->height_ = height; }
  float GetHeight() const { return height_; }
  void SetDistance(float distance) { this->distance_ = distance; }
  float GetDistance() const { return distance_; }
  void SetTheta(float theta) { this->theta_ = theta; }
  float GetTheta() const { return theta_; }

 protected:
  /**
   * * @brief コンスタントバッファの状態を更新する
   */
  virtual void UpdateConstantBufferStaging() override;

 private:
  actor::Actor* target_;
  float height_;
  float distance_;
  float theta_;
};
}  // namespace camera
}  // namespace legend
#endif  //! LEGEND_CAMERA_LOOKAT_FOLLOW_TARGET_CAMERA_H
