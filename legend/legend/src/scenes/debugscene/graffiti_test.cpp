#include "src/scenes/debugscene/graffiti_test.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"

namespace legend {
namespace scenes {
namespace debugscene {

GraffitiTest::GraffitiTest(ISceneChange* scene_change) : Scene(scene_change) {}

GraffitiTest::~GraffitiTest() {}

bool GraffitiTest::Initialize() {
  if (!camera_.Init(L"MainCamera", math::Vector3(0.0f, 2.0f, -2.0f),
                    math::Quaternion::FromEular(45.0f * math::util::DEG_2_RAD,
                                                0.0f, 0.0f),
                    50.0f * math::util::DEG_2_RAD, 1280.0f / 720.0f)) {
    return false;
  }

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  if (!transform_cb_.Init(
          device, directx::shader::ConstantBufferRegisterID::TRANSFORM,
          device.GetHeapManager().GetLocalHeap(
              directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID),
          L"Transform")) {
    return false;
  }
  if (!graffiti_struct_cb_.Init(
          device, 2,
          device.GetHeapManager().GetLocalHeap(
              directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID),
          L"Graffiti")) {
    return false;
  }

  return true;
}

bool GraffitiTest::Update() {
  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  float alpha = graffiti_struct_cb_.GetStaging().alpha;
  alpha += 0.0001f;
  if (alpha > 1.0f) alpha -= 1.0f;
  graffiti_struct_cb_.GetStagingRef().alpha = alpha;
  graffiti_struct_cb_.UpdateStaging();
  return true;
}

void GraffitiTest::Draw() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  device.GetRenderResourceManager().SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::BACK_BUFFER, true,
      directx::render_target::DepthStencilTargetID::DEPTH_ONLY, true);
  resource.GetPipeline()
      .Get(util::resource::id::Pipeline::GRAFFITI)
      ->SetGraphicsCommandList(command_list);

  camera_.RenderStart();
  transform_cb_.SetToHeap(device);
  graffiti_struct_cb_.SetToHeap(device);
  resource.GetModel().Get(util::resource::id::Model::PLANE)->Draw(command_list);
}

void GraffitiTest::Finalize() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  device.WaitExecute();
}

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend