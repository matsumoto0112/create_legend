#include "src/object/obstacle.h"

#include "src/util/resource/resource_names.h"

namespace legend {
namespace object {

Obstacle::Obstacle() : Parent(L"Obstacle") {}

Obstacle::~Obstacle() {}

bool Obstacle::Init(const InitializeParameter& params) {
  if (!InitBuffer()) {
    return false;
  }

  this->transform_.SetPosition(params.position);
  this->transform_.SetRotation(params.rotation);
  this->collision_.SetPosition(transform_.GetPosition());
  this->collision_.SetRotation(transform_.GetRotation());
  this->collision_.SetScale(transform_.GetScale());
  this->collision_.SetLength(params.bounding_box_length);

  model_ = game::GameDevice::GetInstance()->GetResource().GetModel().Get(
      util::resource::resource_names::model::OBSTACLE);
  return true;
}
bool Obstacle::Update() { return true; }

}  // namespace object
}  // namespace legend