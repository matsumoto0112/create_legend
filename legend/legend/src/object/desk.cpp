#include "src/object/desk.h"

#include "src/directx/shader/shader_register_id.h"

namespace legend {
namespace object {

//�R���X�g���N
Desk::Desk() : Parent(L"Desk") {}

//�f�X�g���N�^
Desk::~Desk() {}

bool Desk::Init(const InitializeParameter& parameter) {
  if (!Parent::InitBuffer()) {
    return false;
  }

  this->transform_ = parameter.transform;
  this->collision_.SetPosition(transform_.GetPosition());
  this->collision_.SetRotation(transform_.GetRotation());
  this->collision_.SetScale(transform_.GetScale());
  this->collision_.SetLength(parameter.bounding_box_length);
  SetNormal(parameter.normal);

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  model_ = resource.GetModel().Get(util::resource::id::Model::DESK);
  return true;
}

//�X�V
bool Desk::Update() { return true; }

//���W�̐ݒ�
void Desk::SetPosition(math::Vector3 position) {
  transform_.SetPosition(position);
  collision_.SetPosition(position);
}

//��]�̐ݒ�
void Desk::SetRotation(math::Quaternion rotation) {
  transform_.SetRotation(rotation);
  collision_.SetRotation(rotation);
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