#include "skill_test.h"

#include "src\\stdafx.h"

namespace legend {
namespace scenes {
namespace debugscene {

SkillTest::SkillTest(ISceneChange* scene_change) : Scene(scene_change) {}
SkillTest::~SkillTest() {}
bool SkillTest::Initialize() {
  skill_manager_.Init();

  return true;
}
bool SkillTest::Update() {
  if (ImGui::Begin("Skill Test")) {
    if (ImGui::Button("Add Skill")) {
        skill_manager_.AddSkill();
    }
  }
  ImGui::End();

  return true;
}
void SkillTest::Draw() { skill_manager_.Draw(); }
void SkillTest::Finalize() {}
}  // namespace debugscene
}  // namespace scenes
}  // namespace legend