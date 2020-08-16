#include "src/scenes/title.h"

#include "src/game/game_device.h"

namespace legend {
namespace scenes {

//�R���X�g���N�^
Title::Title(ISceneChange* scene_change) : Scene(scene_change) {}

//������
bool Title::Initialize() {
  game::GameDevice::GetInstance()->GetDevice().GetHeapManager().ResetLocalHeap(
      directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY);

  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();

  //if (!audio.LoadWav(L"free_2.wav", AudioType::BGM, AudioSplitType::SPLIT)) {
  //  return false;
  //}
  //bgm_key_ = audio.Start(L"free_2.wav", 1.0f, true);

  return true;
}

//�X�V
bool Title::Update() {
  auto& input = game::GameDevice::GetInstance()->GetInput();

  if (ImGui::Begin("Text")) {
    ImGui::Text("Push A to play");
  }
  ImGui::End();
  if (input.GetCommand(input::input_code::Decide)) {
    scene_change_->ChangeScene(SceneType::MAIN_SCENE_1);
  }
  return true;
}

//�`��
void Title::Draw() { Scene::Draw(); }

void Title::Finalize() {
}

}  // namespace scenes
}  // namespace legend