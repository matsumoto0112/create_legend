#ifndef LEGEND_SKILL_SKILL_ITEM_BOX_H_
#define LEGEND_SKILL_SKILL_ITEM_BOX_H_

#include "src/actor/actor.h"
#include "src/bullet/bounding_box.h"
#include "src/skill/skill.h"
#include "src/util/timer.h"

namespace legend {
namespace skill {

class SkillItemBox : public actor::Actor {
  using Parent = actor::Actor;

 public:
  struct InitializeParameter {
    util::Transform transform;
    math::Vector3 bounding_box_length;
  };

 public:
  SkillItemBox();
  virtual ~SkillItemBox();
  virtual bool Init(actor::IActorMediator* mediator,
                    const InitializeParameter& parameter,
                    std::shared_ptr<Skill> skill);
  bool Update() override;
  void Draw() override;

  void ChangeDead();
  std::shared_ptr<Skill> GetSkill() const;

 private:
  bool is_dead_;
  std::shared_ptr<Skill> skill_;
  util::CountDownTimer delete_time_;
  //! コライダー
  std::shared_ptr<bullet::BoundingBox> box_;
};

}  // namespace skill
}  // namespace legend

#endif  //! LEGEND_SKILL_SKILL_ITEM_BOX_H_
