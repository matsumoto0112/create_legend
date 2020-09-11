#ifndef LEGEND_OBJECT_FRAGMENT_H
#define LEGEND_OBJECT_FRAGMENT_H

#include "src/actor/actor.h"
#include "src/bullet/bounding_box.h"
#include "src/util/timer.h"

/**
 * @file fragment.h
 * @brief �����J�X�N���X
 */

namespace legend {
namespace object {
/**
 * @brief �����J�X
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
  Fragment();
  virtual ~Fragment();
  virtual bool Init(actor::IActorMediator* mediator,
                    const InitializeParameter& params);
  virtual bool Update() override;
  void Draw() override;

  void ChangeDead();
  bool GetIsDead() const;

 private:
  std::shared_ptr<bullet::BoundingBox> box_;
  bool is_dead_;
  util::CountDownTimer dead_time_;
};

}  // namespace object
}  // namespace legend
#endif  //! LEGEND_OBJECT_FRAGMENT_H
