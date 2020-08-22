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

  bullet::BoundingBox::InitializeParameter params;
  params.position = this->transform_.GetPosition();
  params.rotation = this->transform_.GetRotation();
  params.scale = parameter.bounding_box_length;
  params.mass = 0.01f;
  params.restitution = 0.0f;
  params.friction = 0.0f;
  box_ = std::make_shared<bullet::BoundingBox>(this, params);
  mediator_->AddCollider(box_);

  is_dead_ = false;
  dead_time_.Init(2.0f);

  auto& resource = game::GameDevice::GetInstance()->GetResource();
  model_ = resource.GetModel().Get(
      util::resource::resource_names::model::FRAGMENT_01);
  return true;
}

bool Fragment::Update() {
  if (is_dead_) {
    if (dead_time_.Update()) {
      mediator_->RemoveCollider(box_);
      mediator_->RemoveActor(this);
    }
  }
  return true;
}

void Fragment::ChangeDead() { is_dead_ = true; }

}  // namespace object
}  // namespace legend
