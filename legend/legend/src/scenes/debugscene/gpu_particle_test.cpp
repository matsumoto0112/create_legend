#include "src/scenes/debugscene/gpu_particle_test.h"

#include "src/draw/particle/smoke_particle.h"
#include "src/game/game_device.h"

namespace legend {
namespace scenes {
namespace debugscene {

GPUParticleTest::GPUParticleTest(ISceneChange* scene_change)
    : Scene(scene_change) {}

GPUParticleTest::~GPUParticleTest() {}

bool GPUParticleTest::Initialize() {
  if (!Scene::Initialize()) {
    return false;
  }

  auto& device = game::GameDevice::GetInstance()->GetDevice();

  const math::Vector3 pos = math::Vector3(0.0f, 0.0f, -5.0f);
  const math::Quaternion rot =
      math::Quaternion::FromEular(0.0f * math::util::DEG_2_RAD, 0.0f, 0.0f);
  const float fov = 50.0f * math::util::DEG_2_RAD;
  const float aspect = 1280.0f / 720.0f;
  if (!camera_.Init(L"MainCamera", pos, rot, fov, aspect)) {
    return false;
  }

  directx::device::CommandList command_list;
  if (!command_list.Init(
          device, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT)) {
    return false;
  }

  if (!smoke_particle_.Init(command_list)) {
    return false;
  }

  command_list.Close();
  device.ExecuteCommandList({command_list});
  device.WaitExecute();

  return true;
}

bool GPUParticleTest::Update() {
  auto& particle_compute_command_list =
      game::GameDevice::GetInstance()->GetParticleManager().GetCommandList();
  if (!smoke_particle_.Update(particle_compute_command_list)) {
    return false;
  }

  return true;
}

void GPUParticleTest::Draw() {
  camera_.RenderStart();

  auto& command_list = game::GameDevice::GetInstance()
                           ->GetDevice()
                           .GetCurrentFrameResource()
                           ->GetCommandList();
  smoke_particle_.Render(command_list);
}

void GPUParticleTest::Finalize() {}

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend