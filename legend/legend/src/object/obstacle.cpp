#include "src/object/obstacle.h"

namespace {
using ModelID = legend::util::resource::id::Model;
std::unordered_map<legend::u32, ModelID> MODEL_ID{
    {0, ModelID::ERASER},
};
}  // namespace

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
      MODEL_ID.at(params.model_id));
  return true;
}
bool Obstacle::Update() { return true; }

}  // namespace object
}  // namespace legend