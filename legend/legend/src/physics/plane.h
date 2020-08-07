#ifndef LEGEND_PHYSICS_PLANE_H_
#define LEGEND_PHYSICS_PLANE_H_

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/constant_buffer_structure.h"
#include "src/util/transform.h"

namespace legend {
namespace physics {

/**
 * @class Plane
 * @brief 平面クラス
 */
class Plane {
 public:
  /**
   * @brief コンストラクタ
   */
  Plane();
  /**
   * @brief コンストラクタ
   * @param 座標
   * @param 法線ベクトル
   */
  Plane(math::Vector3 position, math::Vector3 normal);
  /**
   * @brief デストラクタ
   */
  ~Plane();
  /**
   * @brief 初期化
   */
  bool Initialize();
  /**
   * @brief 描画
   */
  void Draw();
  /**
   * @brief 位置の設定
   */
  void SetPosition(math::Vector3 position);
  /**
   * @brief 法線ベクトルの設定
   */
  void SetNormal(math::Vector3 normal);
  /**
   * @brief 位置の取得
   */
  math::Vector3 GetPosition() const;
  /**
   * @brief 法線ベクトルの取得
   */
  math::Vector3 GetNormal() const;
  /**
   * @brief 距離を取得
   */
  float GetDistance(math::Vector3 position) const;

 private:
  util::Transform transform_;
  //! 法線ベクトル
  math::Vector3 normal_;

  float right_;
  float left_;
  float forward_;
  float back_;
  float up_;
  float down_;

  //! トランスフォーム転送用コンスタントバッファ
  directx::buffer::ConstantBuffer<
      directx::buffer::constant_buffer_structure::Transform>
      transform_cb_;
};

}  // namespace physics
}  // namespace legend

#endif  //! LEGEND_PHYSICS_PLANE_H_
