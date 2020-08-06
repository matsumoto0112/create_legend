#include "src/scenes/debugscene/stage_generate_test.h"

#include "src/game/game_device.h"
#include "src/util/path.h"
#include "src/util/string_util.h"

namespace legend {
namespace scenes {
namespace debugscene {
StageGenerateTest::StageGenerateTest(ISceneChange* scene_change)
    : Scene(scene_change) {}
StageGenerateTest::~StageGenerateTest() {
  game::GameDevice::GetInstance()->GetDevice().WaitForGPU();
}
bool StageGenerateTest::Initialize() { return true; }
bool StageGenerateTest::Update() {
  if (ImGui::Begin("StageGenerateTest")) {
    if (ImGui::Button("Load aaaa")) {
      stage_generator_->LoadStageData(util::Path::GetInstance()->exe() /
                                      L"assets" / L"stage" / L"aaaa.bin");
    }
  }
  ImGui::End();
  return true;
}
void StageGenerateTest::Draw() {}
void StageGenerateTest::Finalize() {}
}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
