#include "src/scenes/title.h"

#include "src/libs/imgui/imgui.h"

namespace legend {
namespace scenes {

//コンストラクタ
Title::Title(ISceneChange* scene_change) : Scene(scene_change) {}

//初期化
bool Title::Initialize() { return true; }

//更新
bool Title::Update() {
  //if (ImGui::Begin("Debug Window")) {
  //  ImGui::Text("Title");
  //  if (ImGui::Button("GameOver")) {
  //    scene_change_->ChangeScene(SceneType::GAMEOVER);
  //  }
  //}
  //ImGui::End();

    return true;
}

//描画
void Title::Draw() { Scene::Draw(); }
}  // namespace scenes
}  // namespace legend