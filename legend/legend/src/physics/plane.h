#ifndef LEGEND_PHYSICS_PLANE_H_
#define LEGEND_PHYSICS_PLANE_H_

#include "src/physics/bounding_box.h"

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
     * @brief 初期化
     */
    bool Initialize(directx::DirectX12Device& device);
    /**
     * @brief 描画
     */
    void Draw(directx::DirectX12Device& device);
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
    math::Vector3 GetPosition();
    /**
     * @brief 法線ベクトルの取得
     */
    math::Vector3 GetNormal();
private:
    //! 位置
    math::Vector3 position_;
    //! 法線ベクトル
    math::Vector3 normal_;

    directx::buffer::VertexBuffer vertex_buffer_;
    directx::buffer::IndexBuffer index_buffer_;
    directx::buffer::ConstantBuffer<Transform> transform_constant_buffer_;
};

}  // namespace physics
}  // namespace legend

#endif  //! LEGEND_PHYSICS_PLANE_H_
