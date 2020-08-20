#ifndef LEGEND_OBJECT_OBSTACLE_H_
#define LEGEND_OBJECT_OBSTACLE_H_

/**
 * @file obstacle.h
 */

#include "src/actor/actor.h"
#include "src/bullet/bounding_box.h"

namespace legend {
namespace object {

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
  Obstacle();
  virtual ~Obstacle();
  bool Init(actor::IActorMediator* mediator, const InitializeParameter& params);
  virtual bool Update() override;

 private:
  std::shared_ptr<bullet::BoundingBox> box_;
};

}  // namespace object
}  // namespace legend
#endif  //! LEGEND_OBJECT_OBSTACLE_H_
