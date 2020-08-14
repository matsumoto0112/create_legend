#include "src/scenes/game_over.h"

#include "src/game/game_device.h"

namespace legend {
namespace scenes {

//�R���X�g���N�^
GameOver::GameOver(ISceneChange* scene_change) : Scene(scene_change) {}

//������
bool GameOver::Initialize() { return true; }

//�X�V
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

//�`��
void GameOver::Draw() { Scene::Draw(); }
}  // namespace scenes
}  // namespace legend