#include "src/scenes/title.h"

#include "src/libs/imgui/imgui.h"

namespace legend {
namespace scenes {

//�R���X�g���N�^
Title::Title(ISceneChange* scene_change) : Scene(scene_change) {}

//������
bool Title::Initialize() { return true; }

//�X�V
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

//�`��
void Title::Draw() { Scene::Draw(); }
}  // namespace scenes
}  // namespace legend