#ifndef LEGEND_PHYSICS_RAY_H_
#define LEGEND_PHYSICS_RAY_H_

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/constant_buffer_structure.h"
#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/math/quaternion.h"

namespace legend {
namespace physics {

/**
 * @class Ray
 * @brief レイ（光線）クラス
 */
class Ray {
 public:
  /**
   * @brief コンストラクタ
   */
  Ray();
  /**
   * @brief コンストラクタ
   * @param 方向ベクトル
   * @param 最大範囲
   */
  Ray(math::Vector3 direction, float max_distance);
  /**
   * @brief デフォルトコンストラクタ
   * @param 始点
   * @param 方向ベクトル
   * @param 最大範囲
   */
  Ray(math::Vector3 start_position, math::Vector3 direction,
      float max_distance);
  /**
   * @brief デストラクタ
   */
  ~Ray();
  /**
   * @brief 初期化
   */
  bool Initialize();
  /**
   * @brief 更新
   */
  void Update();
  /**
   * @brief 描画
   */
  void Draw();
  /**
   * @brief 始点の取得
   */
  math::Vector3 GetStartPosition() const;
  /**
   * @brief 方向ベクトルの取得
   */
  math::Vector3 GetDirection() const;
  /**
   * @brief 最大範囲の取得
   */
  float GetDistance() const;

 private:
  // !始点
  math::Vector3 start_position_;
  // !方向ベクトル
  math::Vector3 direction_;
  // !衝突範囲
  float max_distance_;

  directx::buffer::VertexBuffer vertex_buffer_;
  directx::buffer::IndexBuffer index_buffer_;
  directx::buffer::ConstantBuffer<
      directx::buffer::constant_buffer_structure::Transform>
      transform_constant_buffer_;
};

}  // namespace physics
}  // namespace legend

#endif  //! LEGEND_PHYSICS_RAY_H_
