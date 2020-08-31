#include "src/object/back_ground.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace object {

BackGround::BackGround() {}

BackGround::~BackGround() {}

bool BackGround::Init() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();

  if (!transform_cb_.Init(
          device,
          device.GetLocalHandle(
              directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY),
          L"BackGround")) {
    return false;
  }

  transform_cb_.GetStagingRef().world = math::Matrix4x4::kIdentity;
  transform_cb_.UpdateStaging();
  return true;
}

void BackGround::Draw() {
  //auto& device = game::GameDevice::GetInstance()->GetDevice();

  //transform_cb_.RegisterHandle(
  //    device, directx::shader::ConstantBufferRegisterID::TRANSFORM);

  //auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
  //auto& resource = game::GameDevice::GetInstance()->GetResource();
  //resource.GetModel()
  //    .Get(util::resource::resource_names::model::BACK_FIELD_01)
  //    ->Draw(command_list);
}

}  // namespace object
}  // namespace legend