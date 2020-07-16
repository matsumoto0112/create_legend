#ifndef LEGEND_PHYSICS_BOUNDING_BOX_H_
#define LEGEND_PHYSICS_BOUNDING_BOX_H_

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/game/game_device.h"
#include "src/math/matrix_4x4.h"
#include "src/util/loader/glb_loader.h"

namespace legend {
namespace physics {

struct Transform {
  math::Matrix4x4 world;
};

/**
 * @class BoundingBox
 * @brief 直方体のクラス
 */

class BoundingBox {
 public:
  /**
   * @brief コンストラクタ
   */
  BoundingBox();
  /**
   * @brief 初期化
   * @param デバイス
   * @param オブジェクトネーム
   * @param GLBLoader
   */
  bool Initialize(directx::DirectX12Device& device);
  /**
   * @brief 更新
   */
  void Update();
  /**
   * @brief 描画
   * @param デバイス
   */
  void Draw(directx::DirectX12Device& device);
  /*
   * @brief 方向ベクトルを取得
   * @param 軸番号
   */
  math::Vector3 GetDirection(i32 direction_num);
  /**
   * @brief 長さを取得
   * @param 軸番号
   */
  float GetLength(i32 length_num);
  /**
   * @brief 現在の位置を取得
   */
  math::Vector3 GetPosition();
  /**
   * @brief 現在の角度を取得
   */
  math::Vector3 GetRotation();
  /**
   * @brief 現在のスケールを取得
   */
  math::Vector3 GetScale();
  /**
   * @brief 各方向ベクトルの設定
   * @param X方向
   * @param Y方向
   * @param Z方向
   */
  void SetDirection(math::Vector3 direction_x, math::Vector3 direction_y,
                    math::Vector3 direction_z);
  /**
   * @brief 各方向の長さの設定
   * @param X方向の長さ
   * @param Y方向の長さ
   * @param Z方向の長さ
   */
  void SetLength(float length_x, float length_y, float length_z);
  /**
   * @brief 中心座標の更新
   * @param 座標
   */
  void SetPosition(math::Vector3 position);
  /**
   * @brief 回転角度の更新
   * @param 角度
   */
  void SetRotation(math::Vector3 rotate);
  /**
   * @brief 拡大縮小の更新
   * @param スケール
   */
  void SetScale(math::Vector3 scale);

 private:
  //!中心座標
  math::Vector3 position_;
  //!角度
  math::Vector3 rotation_;
  //!スケール
  math::Vector3 scale_;
  //!方向ベクトル
  std::vector<math::Vector3> directions_;
  //!各軸方向の長さ
  std::vector<float> lengthes_;

  directx::buffer::VertexBuffer vertex_buffer_;
  directx::buffer::IndexBuffer index_buffer_;
  directx::buffer::ConstantBuffer<Transform> transform_constant_buffer_;
};

}  // namespace physics
}  // namespace legend

#endif  //! LEGEND_PHYSICS_BOUNDING_BOX_H_
