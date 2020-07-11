#ifndef LEGEND_CAMERA_PERSPECTIVE_CAMERA_H_
#define LEGEND_CAMERA_PERSPECTIVE_CAMERA_H_

/**
 * @file perspective_camera.h
 * @brief パースペクティブカメラクラス定義
 */

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/constant_buffer_structure.h"
#include "src/math/matrix_4x4.h"
#include "src/math/quaternion.h"
#include "src/math/vector_3.h"

namespace legend {
namespace camera {

/**
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
   * @brief カメラの初期化
   * @param name カメラ名
   * @param position 座標
   * @param lookat 注視点
   * @param up_vector カメラの上方向ベクトル
   * @param fov 視野角(rad)
   * @param aspect_ratio アスペクト比
   * @param near_z 最近点
   * @param far_z 最遠点
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(const std::wstring& name, const math::Vector3& position,
            const math::Vector3& lookat, const math::Vector3& up_vector,
            float fov, float aspect_ratio, float near_z = 0.1f,
            float far_z = 300.0f);
  /**
   * @brief 描画を開始する
   */
  void RenderStart();

 public:
  /**
   * @brief 名前の設定
   */
  inline void SetName(const std::wstring& name) { name_ = name; }
  /**
   * @brief 名前の取得
   */
  inline std::wstring GetName() const { return name_; }

 public:
  //! カメラ名
  std::wstring name_;
  //! 描画用コンスタントバッファ
  directx::buffer::ConstantBuffer<
      directx::constant_buffer_structure::WorldContext>
      world_context_constant_buffer_;
};

}  // namespace camera
}  // namespace legend

#endif  //! LEGEND_CAMERA_PERSPECTIVE_CAMERA_H_
