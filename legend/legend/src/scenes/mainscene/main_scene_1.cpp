#include "src/scenes/mainscene/main_scene_1.h"

#include "src/game/game_device.h"

namespace legend {
namespace scenes {
namespace mainscene {

//�R���X�g���N�^
MainScene1::MainScene1(ISceneChange* scene_change) : Scene(scene_change) {}

//�f�X�g���N�^
MainScene1::~MainScene1() {}

//������
bool MainScene1::Initialize() {
  if (!turn_system_.Init("bbbb")) {
    return false;
  }

  return true;
}

//�X�V
bool MainScene1::Update() {
  if (!turn_system_.Update()) {
    return false;
  }

  if (turn_system_.IsGameEnd()) {
    const auto data = turn_system_.GetResult();
    system::GameDataStorage::GetInstance()->Set(data);
    scene_change_->ChangeScene(SceneType::GAMEOVER);
  }
  return true;
}

//�`��
void MainScene1::Draw() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
  auto& render_resource_manager = device.GetRenderResourceManager();

  //�܂��͒ʏ�̃��f���`��
  render_resource_manager.SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::BACK_BUFFER, true,
      directx::render_target::DepthStencilTargetID::DEPTH_ONLY, true);

  turn_system_.Draw();

  //���ɃR���W�����Ȃǂ̃f�o�b�O�`��
  render_resource_manager.SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::BACK_BUFFER, false,
      directx::render_target::DepthStencilTargetID::NONE, false);
  turn_system_.DebugDraw();
}

//�I��
void MainScene1::Finalize() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  device.WaitExecute();
}

//�^�[���ʂ̍X�V����
bool MainScene1::UpdateTurn() { return true; }
}  // namespace mainscene
}  // namespace scenes
}  // namespace legend