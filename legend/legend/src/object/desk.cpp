#include "src/object/desk.h"

#include "src/directx/shader/shader_register_id.h"

namespace legend {
namespace object {

//�R���X�g���N
Desk::Desk() : actor::Actor<physics::BoundingBox>() {}

//�f�X�g���N�^
Desk::~Desk() {}

bool Desk::Init(const InitializeParameter& parameter) {
  this->transform_ = parameter.transform;
  this->collision_ = physics::BoundingBox();
  this->collision_.SetLength(parameter.bounding_box_length.x,
                             parameter.bounding_box_length.y,
                             parameter.bounding_box_length.z);

  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();
  util::resource::Resource& resource =
      game::GameDevice::GetInstance()->GetResource();

  //�g�����X�t�H�[���o�b�t�@���쐬����
  if (!transform_cb_.Init(
          device, directx::shader::ConstantBufferRegisterID::Transform,
          device.GetLocalHeapHandle(directx::descriptor_heap::heap_parameter::
                                        LocalHeapID::PLAYER_MOVE_VIEWER),
          L"Transform ConstantBuffer")) {
    return false;
  }

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  model_ = resource.GetModel().Get(util::resource::ModelID::DESK);
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

//���W�̎擾
math::Vector3 Desk::GetPosition() { return transform_.GetPosition(); }

//��]�̎擾
math::Quaternion Desk::GetRotation() { return transform_.GetRotation(); }

//�X�P�[���̎擾
math::Vector3 Desk::GetScale() { return transform_.GetScale(); }

}  // namespace object
}  // namespace legend