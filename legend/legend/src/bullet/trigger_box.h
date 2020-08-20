#ifndef LEGEND_BULLET_TRIGGER_BOX_H
#define LEGEND_BULLET_TRIGGER_BOX_H

/**
 * @file trigger_box.h
 * @brief
 */

#include "src/bullet/trigger.h"

namespace legend {
namespace bullet {

class TriggerBox : public Trigger {
 public:
  /**
   * @brief �������p�����[�^
   */
  struct InitializeParameter {
    math::Vector3 position = math::Vector3::kZeroVector;      //���W
    math::Quaternion rotation = math::Quaternion::kIdentity;  //��]
    math::Vector3 scale = math::Vector3::kUnitVector;         //�傫��
  };

 public:
  TriggerBox(actor::Actor* owner, const InitializeParameter& parameter);
  virtual ~TriggerBox();
};

}  // namespace bullet
}  // namespace legend
#endif  //! LEGEND_BULLET_TRIGGER_BOX_H
