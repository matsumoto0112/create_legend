#ifndef LEGEND_ACTOR_ACTOR_H_
#define LEGEND_ACTOR_ACTOR_H_

/**
 * @file actor.h
 */

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/constant_buffer_structure.h"
#include "src/game/game_device.h"
#include "src/util/resource/resource_id.h"
#include "src/util/transform.h"

namespace legend {
namespace actor {

/**
 * @brief アクタークラス
 * @tparam T
 */
template <class T>
class Actor {
 public:
  /**
   * @brief コンストラクタ
   */
  Actor();
  /**
   * @brief デストラクタ
   */
  ~Actor();
  /**
   * @brief 更新処理
   * @return オブジェクトを削除するときにtrueを返す
   */
  virtual bool Update() = 0;
  /**
   * @brief 描画
   */
  virtual void Draw();
  /**
   * @brief コリジョンを取得する
   */
  T& GetCollisionRef() { return collision_; }

 protected:
  //! トランスフォーム
  util::Transform transform_;
  //! トランスフォームコンスタントバッファ
  directx::buffer::ConstantBuffer<directx::constant_buffer_structure::Transform>
      transform_cb_;

  //! 描画モデル
  std::shared_ptr<draw::Model> model_;
  //! コリジョン
  T collision_;
};

template <class T>
inline Actor<T>::Actor() {}

template <class T>
inline Actor<T>::~Actor() {}

template <class T>
inline void Actor<T>::Draw() {
  MY_ASSERTION(model_.get(), L"モデルが存在しません。");

  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();
  transform_cb_.SetToHeap(device);

  model_->Draw();
}

}  // namespace actor
}  // namespace legend
#endif  //! LEGEND_ACTOR_ACTOR_H_
