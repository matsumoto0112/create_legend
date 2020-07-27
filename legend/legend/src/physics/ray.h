#ifndef LEGEND_PHYSICS_RAY_H_
#define LEGEND_PHYSICS_RAY_H_

#include "src/physics/bounding_box.h"

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
  Ray(math::Vector3 start_position, math::Vector3 direction, float max_distance);
  /**
   * @brief デストラクタ
   */
  ~Ray();
  /**
   * @brief 初期化
   */
  bool Initialize(directx::DirectX12Device& device);
  /**
   * @brief 更新
   */
  void Update();
  /**
   * @brief 描画
   */
  void Draw(directx::DirectX12Device& device);
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
  directx::buffer::ConstantBuffer<Transform> transform_constant_buffer_;

  directx::buffer::ConstantBuffer<WorldContext> world_constant_buffer_;
  directx::shader::GraphicsPipelineState pipeline_state_;
};

}  // namespace physics
}  // namespace legend

#endif  //! LEGEND_PHYSICS_RAY_H_
