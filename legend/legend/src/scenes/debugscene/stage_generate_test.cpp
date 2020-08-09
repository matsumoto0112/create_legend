#include "src/scenes/debugscene/stage_generate_test.h"

#include "src/game/game_device.h"
#include "src/util/path.h"
#include "src/util/string_util.h"

namespace legend {
namespace scenes {
namespace debugscene {
StageGenerateTest::StageGenerateTest(ISceneChange* scene_change)
    : Scene(scene_change) {
  indexs_.clear();
  indexs_.push_back("no data");
}
StageGenerateTest::~StageGenerateTest() {}
bool StageGenerateTest::Initialize() { return true; }
bool StageGenerateTest::Update() {
  if (ImGui::Begin("StageGenerateTest")) {
    if (ImGui::Button("Load aaaa")) {
      indexs_ =
          stage_generator_->LoadStageData(util::Path::GetInstance()->exe() /
                                          L"assets" / L"stage" / L"aaaa.txt");
    }
  }
  ImGui::End();

  if (ImGui::Begin("LoadData")) {
     for (auto&& index : indexs_) {
      ImGui::Text(index.c_str());
    }
  }
  ImGui::End();
  return true;
}
void StageGenerateTest::Draw() {}
void StageGenerateTest::Finalize() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  device.WaitExecute();
}
}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
