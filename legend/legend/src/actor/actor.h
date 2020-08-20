#ifndef LEGEND_ACTOR_ACTOR_H_
#define LEGEND_ACTOR_ACTOR_H_

/**
 * @file actor.h
 * @brief アクタークラス定義
 */

#include "src/actor/actor_mediator.h"
#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/constant_buffer_structure.h"
#include "src/draw/model.h"
#include "src/util/transform.h"

namespace legend {
namespace actor {

/**
 * @brief アクタークラス
 */
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
  virtual ~Actor();
  /**
   * @brief 初期化
   * @param mediator アクター仲介
   */
  virtual bool Init(IActorMediator* mediator);
  /**
   * @brief 更新処理
   * @return オブジェクトを削除するときにtrueを返す
   */
  virtual bool Update() = 0;
  /**
   * @brief 描画
   */
  virtual void Draw();
  virtual void Destroy(){};

 public:
  std::wstring GetName() const { return name_; }
  util::Transform& GetTransformRef() { return transform_; }
  util::Transform GetTransform() const { return transform_; }

 protected:
  //! アクター名
  std::wstring name_;
  //! トランスフォーム
  util::Transform transform_;
  //! トランスフォームコンスタントバッファ
  TransformConstantBuffer transform_cb_;
  //! 描画モデル
  std::shared_ptr<draw::Model> model_;
  //! 仲介者
  IActorMediator* mediator_;
};

}  // namespace actor
}  // namespace legend

#endif  //! LEGEND_ACTOR_ACTOR_H_
