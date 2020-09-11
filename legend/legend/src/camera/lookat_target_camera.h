#ifndef LEGEND_CAMERA_LOOKAT_TARGET_CAMERA_H
#define LEGEND_CAMERA_LOOKAT_TARGET_CAMERA_H

/**
 * @file lookat_target_camera.h
 * @brief ターゲットを注視するカメラクラス定義
 */

#include "src/actor/actor.h"
#include "src/camera/perspective_camera.h"

namespace legend {
namespace camera {

/**
 * @brief ターゲットを注視するカメラ
 */
class LookatTargetCamera : public PerspectiveCamera {
 public:
  LookatTargetCamera();
  virtual ~LookatTargetCamera();
  /**
   * @brief 初期化
   * @param name カメラ名
   * @param target 注視する対象
   * @param position 設置する座標
   * @param fov FOV
   * @param ratio アスペクト比
   * @param up_vector カメラの上方向
   * @param near_z 最近点
   * @param far_z 最遠点
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
   * * @brief コンスタントバッファの状態を更新する
   */
  virtual void UpdateConstantBufferStaging() override;

 private:
  actor::Actor* target_;
};
}  // namespace camera
}  // namespace legend
#endif  //! LEGEND_CAMERA_LOOKAT_TARGET_CAMERA_H
