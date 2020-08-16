#include "skill_test.h"

#include "src\\stdafx.h"

namespace legend {
namespace scenes {
namespace debugscene {

SkillTest::SkillTest(ISceneChange* scene_change) : Scene(scene_change) {}
SkillTest::~SkillTest() {}
bool SkillTest::Initialize() {
  skill_manager_.Init();
  if (!turn_system_.Init("bbbb")) {
    return false;
  }

  return true;
}
bool SkillTest::Update() {
  skill_manager_.Update();

  if (!turn_system_.Update()) {
    return false;
  }
  if (ImGui::Begin("Skill Test")) {
    if (ImGui::Button("Add Skill")) {
        std::shared_ptr<skill::SkillPencil> skill = std::make_shared<skill::SkillPencil>();
      skill->Init(*turn_system_.GetPlayer());
      skill_manager_.AddSkill(skill);
    }
  }
  ImGui::End();

  return true;
}
void SkillTest::Draw() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
  auto& render_resource_manager = device.GetRenderResourceManager();

  //まずは通常のモデル描画
  render_resource_manager.SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::BACK_BUFFER, true,
      directx::render_target::DepthStencilTargetID::DEPTH_ONLY, true);

  turn_system_.Draw();

  skill_manager_.Draw();

  //次にコリジョンなどのデバッグ描画
  render_resource_manager.SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::BACK_BUFFER, false,
      directx::render_target::DepthStencilTargetID::NONE, false);
  turn_system_.DebugDraw();
}
void SkillTest::Finalize() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  device.WaitExecute();
}
}  // namespace debugscene
}  // namespace scenes
}  // namespace legend