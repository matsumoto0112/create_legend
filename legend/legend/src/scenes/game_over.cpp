#include "src/scenes/game_over.h"

#include "src/libs/imgui/imgui.h"

namespace legend {
namespace scenes {

//�R���X�g���N�^
GameOver::GameOver(ISceneChange* scene_change) : Scene(scene_change) {}

//������
bool GameOver::Initialize() { return true; }

//�X�V
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

//�`��
void GameOver::Draw() { Scene::Draw(); }
}  // namespace scenes
}  // namespace legend