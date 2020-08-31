#include "src/object/back_ground.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"
#include "src/util/resource/resource_names.h"
#include "src/util/string_util.h"

namespace legend {
namespace object {

BackGround::BackGround() {}

BackGround::~BackGround() {}

bool BackGround::Init() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();

  for (u32 i = 0; i < bg_type::MAX; i++) {
    std::wstringstream wss;
    if (!transform_cbs_[i].Init(
            device,
            device.GetLocalHandle(directx::descriptor_heap::heap_parameter::
                                      LocalHeapID::ONE_PLAY),
            util::string_util::Format(L"BackGround[%d]", i))) {
      return false;
    }
    transform_cbs_[i].GetStagingRef().world = math::Matrix4x4::kIdentity;
    transform_cbs_[i].UpdateStaging();
  }

  transform_cbs_[bg_type::LEFT].GetStagingRef().world =
      math::Matrix4x4::CreateTranslate(math::Vector3(-350, -55 + 200, 0));
  transform_cbs_[bg_type::LEFT].UpdateStaging();

  transform_cbs_[bg_type::RIGHT].GetStagingRef().world =
      math::Matrix4x4::CreateTranslate(math::Vector3(350, -55 + 200, 0));
  transform_cbs_[bg_type::RIGHT].UpdateStaging();

  transform_cbs_[bg_type::FORWARD].GetStagingRef().world =
      math::Matrix4x4::CreateTranslate(math::Vector3(0, -55 + 200, -450));
  transform_cbs_[bg_type::FORWARD].UpdateStaging();

  transform_cbs_[bg_type::BACK].GetStagingRef().world =
      math::Matrix4x4::CreateTranslate(math::Vector3(0, -55 + 200, 450));
  transform_cbs_[bg_type::BACK].UpdateStaging();

  transform_cbs_[bg_type::FLOOR].GetStagingRef().world =
      math::Matrix4x4::CreateTranslate(math::Vector3(0, -55, 0));
  transform_cbs_[bg_type::FLOOR].UpdateStaging();

  return true;
}

void BackGround::Draw() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  using namespace util::resource::resource_names::model;
  constexpr const wchar_t* NAMES[] = {
      CLASSROOM_LEFTWALL, CLASSROOM_RIGHTWALL, CLASSROOM_FORWARDWALL,
      CLASSROOM_BACKWALL, CLASSROOM_FLOOR,     CLASSROOM_CEILING};
  resource.GetPipeline()
      .Get(util::resource::resource_names::pipeline::DIFFERED_RENDERING_PRE)
      ->SetCommandList(command_list);

  for (u32 i = 0; i < bg_type::MAX; i++) {
    transform_cbs_[i].RegisterHandle(
        device, directx::shader::ConstantBufferRegisterID::TRANSFORM);
    resource.GetModel().Get(NAMES[i])->Draw(command_list);
  }
}

}  // namespace object
}  // namespace legend