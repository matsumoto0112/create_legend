#include "src/scenes/debugscene/player_move_viewer.h"

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
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  if (!player_.Initilaize(device)) {
    return false;
  }

  if (!plane_.Initialize(device)) {
    return false;
  }

  return true;
}

//�X�V
bool PlayerMoveViewer::Update() {
  if (!player_.Update()) {
    return false;
  }

  math::Vector3 velocity = player_.GetVelocity();
  player_.SetPower();
  float impulse = player_.GetImpulse();
  if (ImGui::Begin("PlayerVelocity")) {
    ImGui::SliderFloat3("Velocity", &velocity.x, -1.0f, 1.0f);
    if (ImGui::Button("Velocity_X_Plus")) {
      velocity.x += 0.1f;
    }
    if (ImGui::Button("Velocity_X_Minus")) {
      velocity.x -= 0.1f;
    }
    if (ImGui::Button("Velocity_Z_Plus")) {
      velocity.z += 0.1f;
    }
    if (ImGui::Button("Velocity_Z_Minus")) {
      velocity.z -= 0.1f;
    }

    ImGui::SliderFloat("Impulse", &impulse, 0, 1.0f);
    math::Vector3 position = player_.GetPosition();
    ImGui::SliderFloat3("Position", &position.x, -100.0f, 100.0f);
  }
  ImGui::End();

   player_.SetVelocity();
  //player_.SetVelocity(velocity);
  player_.Move();

  if (physics::Collision::GetInstance()->Collision_OBB_Plane(player_.GetOBB(),
                                                             plane_)) {
    MY_LOG(L"�����߂�");
  }

  return true;
}

//�`��
void PlayerMoveViewer::Draw() {
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  player_.Draw(device);
  plane_.Draw(device);
}

//�I��
void PlayerMoveViewer::Finalize() {
  game::GameDevice::GetInstance()->GetDevice().WaitForGPU();
}
}  // namespace debugscene
}  // namespace scenes
}  // namespace legend