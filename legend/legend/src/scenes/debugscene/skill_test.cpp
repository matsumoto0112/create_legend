#include "skill_test.h"

#include "src\\stdafx.h"

namespace legend {
namespace scenes {
namespace debugscene {

SkillTest::SkillTest(ISceneChange* scene_change) : Scene(scene_change) {}
SkillTest::~SkillTest() {}
bool SkillTest::Initialize() {
  skill_select_ui_.Init();

  return true;
}
bool SkillTest::Update() {
  if (ImGui::Begin("Skill Test")) {
    if (ImGui::Button("Add Skill")) {
      skill_select_ui_.AddSkill();
    }
  }
  ImGui::End();

  return true;
}
void SkillTest::Draw() { skill_select_ui_.Draw(); }
void SkillTest::Finalize() {}
}  // namespace debugscene
}  // namespace scenes
}  // namespace legend