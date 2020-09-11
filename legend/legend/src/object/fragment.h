#ifndef LEGEND_OBJECT_FRAGMENT_H
#define LEGEND_OBJECT_FRAGMENT_H

#include "src/actor/actor.h"
#include "src/bullet/bounding_box.h"
#include "src/util/timer.h"

/**
 * @file fragment.h
 * @brief 消しカスクラス
 */

namespace legend {
namespace object {
/**
 * @brief 消しカス
 */
class Fragment : public actor::Actor {
  using Parent = actor::Actor;

 public:
  struct InitializeParameter {
    math::Vector3 position;
    math::Quaternion rotation;
    math::Vector3 scale;
    math::Vector3 bounding_box_length;
  };

 public:
  /**
   * @brief コンストラクタ
   */
  Fragment();
  /**
   * @brief デストラクタ
   */
  virtual ~Fragment();
  /**
   * @brief 初期化
   */
  virtual bool Init(actor::IActorMediator* mediator,
                    const InitializeParameter& params);
  /**
   * @brief 更新
   */
  virtual bool Update() override;
  /**
   * @brief 描画
   */
  void Draw() override;
  /**
   * @brief 死亡判定切り替え
   */
  void ChangeDead();
  /**
   * @brief 死亡判定の取得
   */
  bool GetIsDead() const;

 private:
  //! コライダー
  std::shared_ptr<bullet::BoundingBox> box_;
  //! 死亡判定
  bool is_dead_;
  //! 削除時間
  util::CountDownTimer dead_time_;
};

}  // namespace object
}  // namespace legend
#endif  //! LEGEND_OBJECT_FRAGMENT_H
