#ifndef LEGEND_ACTOR_ACTOR_H_
#define LEGEND_ACTOR_ACTOR_H_

/**
 * @file actor.h
 * @brief アクタークラス定義
 */

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/constant_buffer_structure.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"
#include "src/util/resource/resource_names.h"
#include "src/util/transform.h"

namespace legend {
namespace actor {

/**
 * @brief アクタークラス
 * @tparam T オブジェクトのコリジョン
 */
template <class T>
class Actor {
  using TransformConstantBuffer = directx::buffer::ConstantBuffer<
      directx::buffer::constant_buffer_structure::Transform>;

 public:
  /**
   * @brief コンストラクタ
   */
  Actor(const std::wstring& name);
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
  /**
   * @brief バッファの初期化
   * @return 初期化に成功したらtrueを返す
   */
  bool InitBuffer();

 public:
  /**
   * @brief トランスフォームコンスタントバッファの参照を取得する
   */
  TransformConstantBuffer& GetTransformConstantBufferRef() {
    return transform_cb_;
  }

 protected:
  //! アクター名
  std::wstring name_;
  //! トランスフォーム
  util::Transform transform_;
  //! トランスフォームコンスタントバッファ
  TransformConstantBuffer transform_cb_;
  //! 描画モデル
  std::shared_ptr<draw::Model> model_;
  //! コリジョン
  T collision_;
};

//コンストラクタk
template <class T>
inline Actor<T>::Actor(const std::wstring& name) : name_(name) {}

//デストラクタ
template <class T>
inline Actor<T>::~Actor() {}

//描画
template <class T>
inline void Actor<T>::Draw() {
  MY_ASSERTION(model_.get(), L"モデルが存在しません。");

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
  resource.GetPipeline()
      .Get(util::resource::resource_names::pipeline::MODEL_VIEW)
      ->SetCommandList(command_list);

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();
  transform_cb_.RegisterHandle(
      device, directx::shader::ConstantBufferRegisterID::TRANSFORM);

  model_->Draw(command_list);
}

template <class T>
inline bool Actor<T>::InitBuffer() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();

  //トランスフォームバッファを作成する
  if (!transform_cb_.Init(
          device,
          device.GetLocalHandle(
              directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY),
          name_ + L"_TransformConstantBuffer")) {
    return false;
  }

  if (!collision_.Init()) {
    return false;
  }

  return true;
}

}  // namespace actor
}  // namespace legend
#endif  //! LEGEND_ACTOR_ACTOR_H_
