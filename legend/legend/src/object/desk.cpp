#include "src/object/desk.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace object {

//�R���X�g���N
Desk::Desk() : Parent(L"Desk") {}

//�f�X�g���N�^
Desk::~Desk() {}

bool Desk::Init(actor::IActorMediator* mediator,
                const InitializeParameter& parameter) {
  if (!Parent::Init(mediator)) {
    return false;
  }
  if (!Parent::InitBuffer()) {
    return false;
  }

  this->transform_ = parameter.transform;

  bullet::BoundingBox::InitializeParameter params;
  params.mass = 0.0f;
  params.position = transform_.GetPosition();
  params.rotation = transform_.GetRotation();
  params.restitution = 1.0f;
  params.friction = 0.8f;
  params.scale = parameter.bounding_box_length;
  box_ = std::make_shared<bullet::BoundingBox>(this, params);
  mediator_->AddCollider(box_);
  SetNormal(parameter.normal);

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  model_ = resource.GetModel().Get(util::resource::resource_names::model::DESK);
  return true;
}

//�X�V
bool Desk::Update() { return true; }

//���W�̐ݒ�
void Desk::SetPosition(math::Vector3 position) {
  transform_.SetPosition(position);
  // collision_.SetPosition(position);
}

//��]�̐ݒ�
void Desk::SetRotation(math::Quaternion rotation) {
  transform_.SetRotation(rotation);
  // collision_.SetRotation(rotation);
}

//�X�P�[���̐ݒ�
void Desk::SetScale(math::Vector3 scale) { transform_.SetScale(scale); }

//�@���x�N�g���̐ݒ�
void Desk::SetNormal(math::Vector3 normal) { normal_ = normal; }

//���W�̎擾
math::Vector3 Desk::GetPosition() { return transform_.GetPosition(); }

//��]�̎擾
math::Quaternion Desk::GetRotation() { return transform_.GetRotation(); }

//�X�P�[���̎擾
math::Vector3 Desk::GetScale() { return transform_.GetScale(); }

//�@���x�N�g���̎擾
math::Vector3 Desk::GetNormal() { return normal_; }

}  // namespace object
}  // namespace legend