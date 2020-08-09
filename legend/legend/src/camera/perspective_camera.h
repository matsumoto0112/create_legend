#ifndef LEGEND_CAMERA_PERSPECTIVE_CAMERA_H_
#define LEGEND_CAMERA_PERSPECTIVE_CAMERA_H_

/**
 * @file perspective_camera.h
 * @brief パースペクティブカメラクラス定義
 */

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/constant_buffer_structure.h"
#include "src/math/matrix_4x4.h"
#include "src/math/quaternion.h"
#include "src/math/vector_3.h"

namespace legend {
namespace camera {

/**
 * @class PerspectiveCamera
 * @brief パースペクティブカメラクラス
 */
class PerspectiveCamera {
 public:
  /**
   * @brief コンストラクタ
   */
  PerspectiveCamera();
  /**
   * @brief デストラクタ
   */
  ~PerspectiveCamera();
  /**
   * @brief 初期化
   * @param name カメラ名
   * @param position カメラ座標
   * @param rotation カメラの回転角
   * @param fov 視野角(rad)
   * @param aspect_ratio アスペクト比
   * @param up_vector カメラの上方向ベクトル
   * @param near_z 最近点
   * @param far_z 最遠点
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(const std::wstring& name, const math::Vector3& position,
            const math::Quaternion& rotation, float fov, float aspect_ratio,
            const math::Vector3& up_vector = math::Vector3::kUpVector,
            float near_z = 0.1f, float far_z = 300.0f);
  /**
   * @brief 描画を開始する
   */
  void RenderStart();

 private:
  /**
   * @brief コンスタントバッファの初期化
   */
  bool InitConstantBuffer();
  /**
   * @brief コンスタントバッファを更新する
   */
  void UpdateConstantBufferStaging();

 public:
  /**
   * @brief 名前の設定
   */
  inline void SetName(const std::wstring& name) { name_ = name; }
  /**
   * @brief 名前の取得
   */
  inline std::wstring GetName() const { return name_; }
  /**
   * @brief 座標を設定する
   */
  inline void SetPosition(const math::Vector3& position) {
    this->position_ = position;
  }
  /**
   * @brief 座標を取得する
   */
  inline math::Vector3 GetPosition() const { return position_; }
  /**
   * @brief 回転角を設定する
   */
  inline void SetRotation(const math::Quaternion& rotation) {
    this->rotation_ = rotation;
  }
  /**
   * @brief 回転角を取得する
   */
  inline math::Quaternion GetRotation() const { return rotation_; }
  /**
   * @brief 上方向ベクトルを設定する
   */
  inline void SetUpVector(const math::Vector3& up_vector) {
    this->up_vector_ = up_vector;
  }
  /**
   * @brief 上方向ベクトルを取得する
   */
  inline math::Vector3 GetUpVector() const { return up_vector_; }
  /**
   * @brief 視野角を設定する
   * @param fov 角度(rad)
   */
  inline void SetFov(float fov) { this->fov_ = fov; }
  /**
   * @brief 視野角を取得する
   */
  inline float GetFov() const { return fov_; }
  /**
   * @brief アスペクト比を設定する
   */
  inline void SetAspectRatio(float ratio) { this->aspect_ratio_ = ratio; }
  /**
   * @brief 設定されているアスペクト比を取得する
   */
  inline float GetAspectRatio() const { return aspect_ratio_; }
  /**
   * @brief 最近点までの距離を設定する
   */
  inline void SetNearZ(float z) { this->near_z_ = z; }
  /**
   * @brief 最近点までの距離を取得する
   */
  inline float GetNearZ() const { return near_z_; }
  /**
   * @brief 最遠点までの距離を設定する
   */
  inline void SetFarZ(float z) { this->far_z_ = z; }
  /**
   * @brief 最遠点までの距離を取得する
   */
  inline float GetFarZ() const { return far_z_; }

 protected:
  //! カメラ名
  std::wstring name_;
  //! カメラ座標
  math::Vector3 position_;
  //! カメラの回転角
  math::Quaternion rotation_;
  //! カメラの上方向ベクトル
  math::Vector3 up_vector_;
  //! 視野角(rad)
  float fov_;
  //! アスペクト比
  float aspect_ratio_;
  //! 最近点
  float near_z_;
  //! 最遠点
  float far_z_;
  //! 描画用コンスタントバッファ
  directx::buffer::ConstantBuffer<
      directx::buffer::constant_buffer_structure::WorldContext>
      world_context_constant_buffer_;
};

}  // namespace camera
}  // namespace legend

#endif  //! LEGEND_CAMERA_PERSPECTIVE_CAMERA_H_
