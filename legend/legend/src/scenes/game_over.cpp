#include "src/scenes/game_over.h"

#include "src/libs/imgui/imgui.h"

namespace legend {
namespace scenes {

//�R���X�g���N�^
GameOver::GameOver(ISceneChange* scene_change) : Scene(scene_change) {}

//������
void GameOver::Initialize() {}

//�X�V
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

//�`��
void GameOver::Draw() { Scene::Draw(); }
}  // namespace scenes
}  // namespace legend