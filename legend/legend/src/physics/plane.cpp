#include "src/physics/plane.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"
#include "src/util/path.h"
#include "src/util/resource/pixel_shader.h"
#include "src/util/resource/resource_names.h"
#include "src/util/resource/vertex_shader.h"

namespace legend {
namespace physics {
//�R���X�g���N�^
Plane::Plane() : transform_(), normal_(math::Vector3::kUpVector) {
  right_ = (normal_ == math::Vector3::kRightVector ||
            normal_ == math::Vector3::kLeftVector)
               ? 0
               : transform_.GetPosition().x + 1;
  left_ = (normal_ == math::Vector3::kRightVector ||
           normal_ == math::Vector3::kLeftVector)
              ? 0
              : transform_.GetPosition().x - 1;
  forward_ = (normal_ == math::Vector3::kForwardVector ||
              normal_ == math::Vector3::kBackwardVector)
                 ? 0
                 : transform_.GetPosition().z - 1;
  back_ = (normal_ == math::Vector3::kForwardVector ||
           normal_ == math::Vector3::kBackwardVector)
              ? 0
              : transform_.GetPosition().z + 1;
  up_ = (normal_ == math::Vector3::kUpVector ||
         normal_ == math::Vector3::kDownVector)
            ? 0
            : transform_.GetPosition().y + 1;
  down_ = (normal_ == math::Vector3::kUpVector ||
           normal_ == math::Vector3::kDownVector)
              ? 0
              : transform_.GetPosition().y - 1;
}

Plane::Plane(math::Vector3 position, math::Vector3 normal)
    : transform_(), normal_(normal) {
  transform_.SetPosition(position);
  right_ = (normal_ == math::Vector3::kRightVector ||
            normal_ == math::Vector3::kLeftVector)
               ? 0
               : transform_.GetPosition().x + 1;
  left_ = (normal_ == math::Vector3::kRightVector ||
           normal_ == math::Vector3::kLeftVector)
              ? 0
              : transform_.GetPosition().x - 1;
  forward_ = (normal_ == math::Vector3::kForwardVector ||
              normal_ == math::Vector3::kBackwardVector)
                 ? 0
                 : transform_.GetPosition().z - 1;
  back_ = (normal_ == math::Vector3::kForwardVector ||
           normal_ == math::Vector3::kBackwardVector)
              ? 0
              : transform_.GetPosition().z + 1;
  up_ = (normal_ == math::Vector3::kUpVector ||
         normal_ == math::Vector3::kDownVector)
            ? 0
            : transform_.GetPosition().y + 1;
  down_ = (normal_ == math::Vector3::kUpVector ||
           normal_ == math::Vector3::kDownVector)
              ? 0
              : transform_.GetPosition().y - 1;
}

Plane::~Plane() {}

//������
bool Plane::Initialize() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  //�g�����X�t�H�[���o�b�t�@���쐬����
  if (!transform_cb_.Init(
          device, directx::shader::ConstantBufferRegisterID::TRANSFORM,
          device.GetLocalHandle(
              directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID),
          L"Transform ConstantBuffer")) {
    return false;
  }

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  return true;
}

//�`��
void Plane::Draw() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();

  transform_cb_.SetToHeap(device);
  game::GameDevice::GetInstance()
      ->GetResource()
      .GetModel()
      .Get(util::resource::resource_names::model::DESK)
      ->Draw(command_list);
}

//�ʒu�̐ݒ�
void Plane::SetPosition(math::Vector3 position) {
  transform_.SetPosition(position);
}

//�@���x�N�g���̐ݒ�
void Plane::SetNormal(math::Vector3 normal) { normal_ = normal; }

//�ʒu�̎擾
math::Vector3 Plane::GetPosition() const { return transform_.GetPosition(); }

//�@���x�N�g���̎擾
math::Vector3 Plane::GetNormal() const { return normal_; }

//�������擾
float Plane::GetDistance(math::Vector3 position) const {
  return math::util::Abs(math::Vector3::Dot(position, GetNormal()));
}
}  // namespace physics
}  // namespace legend