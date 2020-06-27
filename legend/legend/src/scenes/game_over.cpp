#include "src/scenes/game_over.h"

#include "src/libs/imgui/imgui.h"

namespace legend {
namespace scenes {

//コンストラクタ
GameOver::GameOver(ISceneChange* scene_change) : Scene(scene_change) {}

//初期化
void GameOver::Initialize() {}

//更新
void GameOver::Update() {
  // scene_change_->ChangeScene(SceneType::TITLE);
  if (ImGui::Begin("Debug Window")) {
    ImGui::Text("GameOver");
    if (ImGui::Button("Title")) {
      scene_change_->ChangeScene(SceneType::TITLE);
    }
  }
  ImGui::End();
}

//描画
void GameOver::Draw() { Scene::Draw(); }
}  // namespace scenes
}  // namespace legend