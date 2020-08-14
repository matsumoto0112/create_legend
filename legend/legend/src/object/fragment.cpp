#include "src/object/fragment.h"

#include "src/game/game_device.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace object {
Fragment::Fragment() : Parent(L"Fragment") {}

Fragment::~Fragment() {}

bool Fragment::Init(const InitializeParameter& params) {
  if (!Parent::InitBuffer()) {
    return false;
  }

  this->transform_ =
      util::Transform(params.position, params.rotation, params.scale);
  this->collision_.SetPosition(transform_.GetPosition());
  this->collision_.SetRotation(transform_.GetRotation());
  this->collision_.SetScale(transform_.GetScale());
  this->collision_.SetLength(params.bounding_box_length);

  auto& resource = game::GameDevice::GetInstance()->GetResource();
  model_ =
      resource.GetModel().Get(util::resource::resource_names::model::FRAGMENT_01);
  return true;
}

bool Fragment::Update() { return true; }

}  // namespace object
}  // namespace legend
