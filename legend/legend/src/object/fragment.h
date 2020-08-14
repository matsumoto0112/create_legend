#ifndef LEGEND_OBJECT_FRAGMENT_H
#define LEGEND_OBJECT_FRAGMENT_H

/**
 * @file fragment.h
 * @brief
 */
#include "src/actor/actor.h"
#include "src/physics/bounding_box.h"

namespace legend {
namespace object {
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
  Fragment();
  virtual ~Fragment();
  virtual bool Init(const InitializeParameter& params);
  virtual bool Update() override;

 private:
};

}  // namespace object
}  // namespace legend
#endif  //! LEGEND_OBJECT_FRAGMENT_H
