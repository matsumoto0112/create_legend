#ifndef LEGEND_OBJECT_OBSTACLE_H_
#define LEGEND_OBJECT_OBSTACLE_H_

/**
 * @file obstacle.h
 */

#include "src/actor/actor.h"
#include "src/physics/bounding_box.h"

namespace legend {
namespace object {

class Obstacle : public actor::Actor<physics::BoundingBox> {
 public:
  struct InitializeParameter {
    u32 model_id;
    math::Vector3 position;
    math::Quaternion rotation;
    math::Vector3 bounding_box_length;
  };

  using Parent = actor::Actor<physics::BoundingBox>;

 public:
  Obstacle();
  virtual ~Obstacle();
  bool Init(const InitializeParameter& params);
  virtual bool Update() override;

 private:
  util::resource::id::Model model_id_;
};

}  // namespace object
}  // namespace legend
#endif  //! LEGEND_OBJECT_OBSTACLE_H_