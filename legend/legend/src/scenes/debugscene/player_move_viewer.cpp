#include "src/scenes/debugscene/player_move_viewer.h"

#include "src/physics/collision.h"

namespace legend {
namespace scenes {
namespace debugscene {
//�R���X�g���N�^
PlayerMoveViewer::PlayerMoveViewer(ISceneChange* scene_change)
    : Scene(scene_change) {}

//�f�X�g���N�^
PlayerMoveViewer::~PlayerMoveViewer() {}

//������
bool PlayerMoveViewer::Initialize() {
  if (!turn_system_.Init("bbbb")) {
    return false;
  }

  return true;
}

//�X�V
bool PlayerMoveViewer::Update() {
  if (!turn_system_.Update()) {
    return false;
  }

  return true;
}

//�`��
void PlayerMoveViewer::Draw() {
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
void PlayerMoveViewer::Finalize() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  device.WaitExecute();
}

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend