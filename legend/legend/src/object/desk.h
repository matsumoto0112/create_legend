#ifndef LEGEND_OBJECT_DESK_H_
#define LEGEND_OBJECT_DESK_H_

#include "src/directx/buffer/constant_buffer.h"
#include "src/draw/model.h"
#include "src/physics/bounding_box.h"
#include "src/util/transform.h"

namespace legend {
namespace object {

class Desk {
 public:
  /**
   * @brief コンストラクタ
   */
  Desk();
  /**
   * @brief コンストラクタ
   * @param 座標
   * @param 回転
   * @param スケール
   */
  Desk(math::Vector3 position, math::Quaternion rotation, math::Vector3 scale);
  /**
   * @brief デストラクタ
   */
  ~Desk();
  /**
   * @brief 初期化
   */
  bool Initialize(directx::DirectX12Device& device,
      util::resource::Resource& resource);
  /**
   * @brief 更新
   */
  bool Update();
  /**
   * @brief 描画
   */
  void Draw(directx::DirectX12Device& device);
  /**
   * @brief 座標の設定
   */
  void SetPosition(math::Vector3 position);
  /**
   * @brief 回転の設定
   */
  void SetRotation(math::Quaternion rotation);
  /**
   * @brief スケールの設定
   */
  void SetScale(math::Vector3 scale);
  /**
   * @brief 座標の取得
   */
  math::Vector3 GetPosition();
  /**
   * @brief 回転の取得
   */
  math::Quaternion GetRotation();
  /**
   * @brief スケールの設定
   */
  math::Vector3 GetScale();
  /**
   * @brief 直方体の取得
   */
  physics::BoundingBox& GetOBB();

 private:
  //! 衝突判定用の直方体
  physics::BoundingBox obb_;
  //! トランスフォーム転送用コンスタントバッファ
  directx::buffer::ConstantBuffer<directx::constant_buffer_structure::Transform>
      transform_cb_;
  //! トランスフォーム
  util::Transform transform_;
};

}  // namespace object
}  // namespace legend

#endif  //! LEGEND_OBJECT_DESK_H_
