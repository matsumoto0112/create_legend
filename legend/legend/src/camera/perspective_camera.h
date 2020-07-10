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
  PerspectiveCamera();
  ~PerspectiveCamera();
  bool Init(const std::wstring& name, const math::Vector3& position,
            const math::Quaternion& rotation, const math::Vector3& up_vector,
            float fov, float aspect_ratio, float near_z = 0.1f,
            float far_z = 300.0f);
  void RenderStart();

 public:
  std::wstring name_;
  directx::buffer::ConstantBuffer<
      directx::constant_buffer_structure::WorldContext>
      world_context_constant_buffer_;
};

}  // namespace camera
}  // namespace legend

#endif  //! LEGEND_CAMERA_PERSPECTIVE_CAMERA_H_
