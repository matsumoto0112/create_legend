#include "src/scenes/title.h"

#include "src/libs/imgui/imgui.h"

namespace legend {
namespace scenes {

//コンストラクタ
Title::Title(ISceneChange* scene_change) : Scene(scene_change) {}

//初期化
void Title::Initialize() {}

//更新
void Title::Update() {
  // scene_change_->ChangeScene(SceneType::GAMEOVER);
  if (ImGui::Begin("Debug Window")) {
    ImGui::Text("Title");
    if (ImGui::Button("GameOver")) {
      scene_change_->ChangeScene(SceneType::GAMEOVER);
    }
  }
  ImGui::End();
}

//描画
void Title::Draw() { Scene::Draw(); }
}  // namespace scenes
}  // namespace legend