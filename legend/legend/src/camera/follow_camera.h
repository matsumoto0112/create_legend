#ifndef LEGEND_CAMERA_FOLLOW_CAMERA_H
#define LEGEND_CAMERA_FOLLOW_CAMERA_H

/**
 * @file follow_camera.h
 * @brief ターゲットを追尾するカメラ
 */

#include "src/actor/actor.h"
#include "src/camera/perspective_camera.h"

namespace legend {
namespace camera {

/**
 * @brief 追尾カメラ
 */
class FollowCamera : public PerspectiveCamera {
 public:
  /**
   * @brief コンストラクタ
   */
  FollowCamera();
  /**
   * @breif デストラクタ
   */
  virtual ~FollowCamera();
  /**
   * @brief 初期化
   * @param name カメラ名
   * @param target 追尾対象のカメラ
   * @param offset 追尾対象と離れるオフセット値
   * @param rotation カメラの回転
   * @param fov FOV値（ラジアン）
   * @param aspect_ratio アスペクト比
   * @param up_vector カメラの上方向ベクトル
   * @param near_z 最近点
   * @param far_z 最遠点
   */
  bool Init(const std::wstring& name, actor::Actor* target,
            const math::Vector3& offset, const math::Quaternion& rotation,
            float fov, float aspect_ratio,
            const math::Vector3& up_vector = math::Vector3::kUpVector,
            float near_z = 0.1f, float far_z = 300.0f);

 protected:
  /**
   * * @brief コンスタントバッファの状態を更新する
   */
  virtual void UpdateConstantBufferStaging() override;

 private:
  //! 追尾対象となるアクター
  actor::Actor* follow_target_;
  //! オフセット
  math::Vector3 offset_;
};

}  // namespace camera
}  // namespace legend

#endif  //! LEGEND_CAMERA_FOLLOW_CAMERA_H
