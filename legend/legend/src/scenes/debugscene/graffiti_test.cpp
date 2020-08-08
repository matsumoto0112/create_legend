#include "src/scenes/debugscene/graffiti_test.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"

namespace {
std::vector<legend::u8> pixels(64 * 64 * 4);

}  // namespace

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

  directx::device::CommandList command_list;
  if (!command_list.Init(
          device, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT)) {
    return false;
  }

  const object::GraffitiInitializeParameter param{math::Vector3::kZeroVector,
                                                  math::Vector3::kUnitVector};
  if (!graffiti_.Init(param, command_list)) {
    return false;
  }

  command_list.Close();
  device.ExecuteCommandList({command_list});
  device.WaitExecute();

  return true;
}

bool GraffitiTest::Update() {
  graffiti_.Update();
  return true;
}

void GraffitiTest::Draw() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  device.GetRenderResourceManager().SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::BACK_BUFFER, true,
      directx::render_target::DepthStencilTargetID::DEPTH_ONLY, true);

  camera_.RenderStart();
  graffiti_.Draw(command_list);
}

void GraffitiTest::Finalize() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  device.WaitExecute();
}

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend