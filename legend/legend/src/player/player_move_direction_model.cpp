#include "src/player/player_move_direction_model.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace player {

//�R���X�g���N�^
PlayerMoveDirectionModel::PlayerMoveDirectionModel() {}

//�f�X�g���N�^
PlayerMoveDirectionModel::~PlayerMoveDirectionModel() {}

//������
bool PlayerMoveDirectionModel::Init() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  constexpr auto HEAP_ID =
      directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY;
  if (!transform_cb_.Init(device, device.GetLocalHandle(HEAP_ID),
                          L"PlayerMoveDirection_Transform")) {
    return false;
  }
  return true;
}

//���W�ݒ�
void PlayerMoveDirectionModel::SetPosition(const math::Vector3& position) {
  transform_.SetPosition(position);
}

//��]�ݒ�
void PlayerMoveDirectionModel::SetRotation(const math::Quaternion& rotation) {
  transform_.SetRotation(rotation);
}

//�`��
void PlayerMoveDirectionModel::Draw() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();

  auto& resource = game::GameDevice::GetInstance()->GetResource();
  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  transform_cb_.RegisterHandle(
      device, directx::shader::ConstantBufferRegisterID::TRANSFORM);

  resource.GetModel()
      .Get(util::resource::resource_names::model::ARROW)
      ->Draw(command_list);
}

}  // namespace player
}  // namespace legend