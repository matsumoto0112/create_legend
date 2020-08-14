#include "src/scenes/game_over.h"

#include "src/game/game_device.h"

namespace legend {
namespace scenes {

//コンストラクタ
GameOver::GameOver(ISceneChange* scene_change) : Scene(scene_change) {}

//初期化
bool GameOver::Initialize() { return true; }

//更新
bool GameOver::Update() {
  if (ImGui::Begin("Text")) {
    ImGui::Text("Push A to return title");
  }
  ImGui::End();

  auto& input = game::GameDevice::GetInstance()->GetInput();
  if (input.GetCommand(input::input_code::Decide)) {
    scene_change_->ChangeScene(SceneType::TITLE);
  }

  return true;
}

//描画
void GameOver::Draw() { Scene::Draw(); }
}  // namespace scenes
}  // namespace legend