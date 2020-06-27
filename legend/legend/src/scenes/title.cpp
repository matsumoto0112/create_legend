#include "src/scenes/title.h"

#include "src/libs/imgui/imgui.h"

namespace legend {
namespace scenes {

//�R���X�g���N�^
Title::Title(ISceneChange* scene_change) : Scene(scene_change) {}

//������
void Title::Initialize() {}

//�X�V
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

//�`��
void Title::Draw() { Scene::Draw(); }
}  // namespace scenes
}  // namespace legend