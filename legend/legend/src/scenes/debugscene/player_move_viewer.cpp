#include "src/scenes/debugscene/player_move_viewer.h"

namespace legend {
namespace scenes {
namespace debugscene {
//コンストラクタ
PlayerMoveViewer::PlayerMoveViewer(ISceneChange* scene_change)
    : Scene(scene_change) {}

//デストラクタ
PlayerMoveViewer::~PlayerMoveViewer() {}

//初期化
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

//更新
bool PlayerMoveViewer::Update() {
  if (!player_.Update()) {
    return false;
  }

  player_.SetVelocity();
  //player_.SetRotation();
  player_.SetImpulse();

  math::Vector3 velocity = player_.GetVelocity();
  float impulse = player_.GetImpulse();
  if (ImGui::Begin("Player")) {
    ImGui::SliderFloat3("Velocity", &velocity.x, -1.0f, 1.0f);
    ImGui::SliderFloat("Impulse", &impulse, 0, 1.0f);
    math::Vector3 position = player_.GetPosition();
    ImGui::SliderFloat3("Position", &position.x, -100.0f, 100.0f);
  }
  ImGui::End();
  //if (ImGui::Begin("Camera")) {
  //  //カメラ座標
  //  math::Vector3 camera_position = player_.GetCamera().GetPosition();
  //  ImGui::SliderFloat3("Position", &camera_position.x, -100.0f, 100.0f);
  //  player_.GetCamera().SetPosition(camera_position);
  //  //カメラ回転角
  //  math::Vector3 camera_rotation =
  //      math::Quaternion::ToEular(player_.GetCamera().GetRotation()) *
  //      math::util::RAD_2_DEG;
  //  ImGui::SliderFloat3("Rotation", &camera_rotation.x, -180.0f, 180.0f);
  //  player_.GetCamera().SetRotation(
  //      math::Quaternion::FromEular(camera_rotation * math::util::DEG_2_RAD));

  //  //カメラの上方向ベクトルを変更する
  //  if (ImGui::Button("X_UP")) {
  //    player_.GetCamera().SetUpVector(math::Vector3::kRightVector);
  //  }
  //  if (ImGui::Button("Y_UP")) {
  //    player_.GetCamera().SetUpVector(math::Vector3::kUpVector);
  //  }
  //  if (ImGui::Button("Z_UP")) {
  //    player_.GetCamera().SetUpVector(math::Vector3::kForwardVector);
  //  }
  //  float fov = player_.GetCamera().GetFov() * math::util::RAD_2_DEG;
  //  ImGui::SliderFloat("FOV", &fov, 0.01f, 90.0f);
  //  player_.GetCamera().SetFov(fov * math::util::DEG_2_RAD);
  //}
  //ImGui::End();

  player_.Move();

  if (physics::Collision::GetInstance()->Collision_OBB_Plane(player_.GetOBB(),
                                                             plane_)) {
    MY_LOG(L"押し戻し");
    player_.SetPosition(player_.GetOBB().GetPosition());
  }

  return true;
}

//描画
void PlayerMoveViewer::Draw() {
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  player_.Draw(device);
  plane_.Draw(device);
}

//終了
void PlayerMoveViewer::Finalize() {
  game::GameDevice::GetInstance()->GetDevice().WaitForGPU();
}
}  // namespace debugscene
}  // namespace scenes
}  // namespace legend