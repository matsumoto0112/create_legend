#include "src/object/obstacle.h"

#include "src/game/game_device.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace object {
//コンストラクタ
Obstacle::Obstacle() : Parent(L"Obstacle") {}

//デストラクタ
Obstacle::~Obstacle() {}

//初期化
bool Obstacle::Init(actor::IActorMediator* mediator,
                    const InitializeParameter& parameter) {
  if (!Parent::Init(mediator)) {
    return false;
  }
  this->transform_.SetPosition(parameter.position);
  this->transform_.SetRotation(parameter.rotation);

  bullet::BoundingBox::InitializeParameter params;
  params.position = transform_.GetPosition();
  params.rotation = transform_.GetRotation();
  params.scale = parameter.bounding_box_length;
  params.mass = 0.0f;
  params.restitution = 1.0f;
  params.friction = 0.0f;
  box_ = std::make_unique<bullet::BoundingBox>(this, params);
  mediator->AddCollider(box_);

  model_ = game::GameDevice::GetInstance()->GetResource().GetModel().Get(
      util::resource::resource_names::model::FIELD_OBJECT_01);
  return true;
}

//更新
bool Obstacle::Update() { return true; }

}  // namespace object
}  // namespace legend