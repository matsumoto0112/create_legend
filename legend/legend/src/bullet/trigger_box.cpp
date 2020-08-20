#include "src/bullet/trigger_box.h"

#include "src/actor/actor.h"
#include "src/bullet/bullet_helper.h"

namespace legend {
namespace bullet {

TriggerBox::TriggerBox(actor::Actor* owner,
                       const InitializeParameter& parameter)
    : Trigger(owner) {
  //立方体に設定
  shape_ = std::make_shared<btBoxShape>(helper::TobtVector3(parameter.scale));

  btTransform tr(helper::TobtQuaternion(parameter.rotation),
                 helper::TobtVector3(parameter.position));

  //剛体オブジェクト生成
  ghost_ = std::make_shared<btGhostObject>();
  ghost_->setCollisionShape(shape_.get());
  ghost_->setCollisionFlags(ghost_->getCollisionFlags() |
                            btCollisionObject::CF_NO_CONTACT_RESPONSE);
  ghost_->setWorldTransform(tr);
  ghost_->setUserPointer(this);
}

TriggerBox::~TriggerBox() {}

}  // namespace bullet
}  // namespace legend