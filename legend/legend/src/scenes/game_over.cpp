#include "src/scenes/game_over.h"

#include "src/libs/imgui/imgui.h"

namespace legend {
namespace scenes {

//コンストラクタ
GameOver::GameOver(ISceneChange* scene_change) : Scene(scene_change) {}

//初期化
bool GameOver::Initialize() { return true; }

//更新
bool GameOver::Update() {
  //if (ImGui::Begin("Debug Window")) {
  //  ImGui::Text("GameOver");
  //  if (ImGui::Button("Title")) {
  //    scene_change_->ChangeScene(SceneType::TITLE);
  //  }
  //}
  //ImGui::End();

    return true;
}

//描画
void GameOver::Draw() { Scene::Draw(); }
}  // namespace scenes
}  // namespace legend