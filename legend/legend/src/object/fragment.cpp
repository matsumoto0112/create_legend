#include "src/object/fragment.h"

#include "src/game/game_device.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace object {
Fragment::Fragment() : Parent(L"Fragment") {}

Fragment::~Fragment() {}

bool Fragment::Init(actor::IActorMediator* mediator,
                    const InitializeParameter& parameter) {
  if (!Parent::Init(mediator)) {
    return false;
  }

  this->transform_ =
      util::Transform(parameter.position, parameter.rotation, parameter.scale);

  // bullet::TriggerBox::InitializeParameter params;
  // params.position = this->transform_.GetPosition();
  // params.rotation = this->transform_.GetRotation();
  // params.scale = parameter.scale;
  // box_ = std::make_shared<bullet::TriggerBox>(this, params);
  // mediator_->AddTrigger(box_);

  auto& resource = game::GameDevice::GetInstance()->GetResource();
  model_ = resource.GetModel().Get(
      util::resource::resource_names::model::FRAGMENT_01);
  return true;
}

bool Fragment::Update() { return true; }

}  // namespace object
}  // namespace legend
