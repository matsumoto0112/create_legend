#ifndef LEGEND_PHYSICS_SPHERE_H_
#define LEGEND_PHYSICS_SPHERE_H_

#include "src/physics/bounding_box.h"

namespace legend {
namespace physics {
/**
 * @class Sphere
 * @brief 球クラス
 */
class Sphere {
 public:
  /**
   * @brief コンストラクタ
   */
  Sphere();
  /**
   * @brief コンストラクタ
   * @param 座標
   * @param 回転量
   */
  Sphere(math::Vector3 position, math::Quaternion rotation);
  /**
   * @brief デストラクタ
   */
  ~Sphere();
  /**
   * @brief 初期化
   */
  bool Initialize(directx::DirectX12Device& device);
  /**
   * @brief 更新
   */
  void Update();
  /**
   * @brief デストラクタ
   */
  void Draw(directx::DirectX12Device& device);
  /**
   * @brief 座標の取得
   */
  math::Vector3 GetPosition() const;
  /**
   * @brief 回転量の取得
   */
  math::Quaternion GetRotation() const;
  /**
   * @brief スケールの取得
   */
  float GetScale() const;
  /**
   * @brief 半径の取得
   */
  float GetRadius() const;
  /**
   * @brief 半径の2乗を取得
   */
  float GetSquareRadius() const;
  /**
   * @brief 座標の設定
   */
  void SetPosition(math::Vector3 position);
  /**
   * @brief 回転量の設定
   */
  void SetRotation(math::Quaternion rotate);
  /**
   * @brief スケールの設定
   */
  void SetScale(float scale);
  /**
   * @brief 半径の設定
   */
  void SetRadius(float radius);

 private:
  //!中心座標
  math::Vector3 position_;
  //!回転
  math::Quaternion rotation_;
  //!スケール
  float scale_;
  //!半径
  float radius_;

  const i32 u_max = 24;
  const i32 v_max = 12;

  directx::buffer::VertexBuffer vertex_buffer_;
  directx::buffer::IndexBuffer index_buffer_;
  directx::buffer::ConstantBuffer<Transform> transform_constant_buffer_;

  directx::buffer::ConstantBuffer<WorldContext> world_constant_buffer_;
  directx::shader::GraphicsPipelineState pipeline_state_;
};

}  // namespace physics
}  // namespace legend

#endif  //! LEGEND_PHYSICS_SPHERE_H_
