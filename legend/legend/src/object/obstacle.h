#ifndef LEGEND_OBJECT_OBSTACLE_H_
#define LEGEND_OBJECT_OBSTACLE_H_

#include "src/actor/actor.h"
#include "src/bullet/bounding_box.h"

/**
 * @file obstacle.h
 * @brief 障害物クラス
 */

namespace legend {
namespace object {

/**
 * @brief 障害物
 */
class Obstacle : public actor::Actor {
 public:
  struct InitializeParameter {
    u32 model_id;
    math::Vector3 position;
    math::Quaternion rotation;
    math::Vector3 bounding_box_length;
  };

  using Parent = actor::Actor;

 public:
  /**
   * @brief コンストラクタ
   */
  Obstacle();
  /**
   * @briefデストラクタ
   */
  virtual ~Obstacle();
  /**
   * @brief 初期化
   */
  bool Init(actor::IActorMediator* mediator, const InitializeParameter& params);
  /**
   * @brief 更新
   */
  virtual bool Update() override;

 private:
  //! コライダー
  std::shared_ptr<bullet::BoundingBox> box_;
};

}  // namespace object
}  // namespace legend
#endif  //! LEGEND_OBJECT_OBSTACLE_H_
