#include "src/scenes/debugscene/player_move_viewer.h"

#include "src/physics/collision.h"

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
  auto& device = game::GameDevice::GetInstance()->GetDevice();

  //プレイヤーの初期化
  {
    player::Player::InitializeParameter player_parameter;
    player_parameter.transform =
        util::Transform(math::Vector3(0, 4.0f, 0), math::Quaternion::kIdentity,
                        math::Vector3::kUnitVector);
    player_parameter.bouding_box_length =
        math::Vector3(6.0f, 2.5f, 14.0f) / 4.0f;
    player_parameter.min_power = 0;
    player_parameter.max_power = 1;
    if (!player_.Init(player_parameter)) {
      return false;
    }
  }

  //机の初期化
  {
    //本来はステージデータから読み込む
    object::Desk::InitializeParameter desk_parameter;
    desk_parameter.transform =
        util::Transform(math::Vector3::kZeroVector, math::Quaternion::kIdentity,
                        math::Vector3::kUnitVector);
    desk_parameter.bounding_box_length =
        math::Vector3(120.0f, 5.0f, 80.0f) / 4.0f;
    desk_parameter.normal = math::Vector3::kUpVector;
    if (!desk_.Init(desk_parameter)) {
      return false;
    }
  }

  {
    directx::device::CommandList command_list;
    if (!command_list.Init(
            device, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT)) {
      return false;
    }
    object::GraffitiInitializeParameter parameter;
    parameter.transform =
        util::Transform(math::Vector3(0, 4.0f, 0), math::Quaternion::kIdentity,
                        math::Vector3::kUnitVector);
    parameter.bounding_box_length = math::Vector3(4.0f, 0.1f, 4.0f);
    parameter.remaining_grafitti = 100.0f;
    if (!graffiti_.Init(parameter, command_list)) {
      return false;
    }
  }

  //カメラの初期化
  {
    const math::Vector3 camera_position = math::Vector3(0, 50.0f, -50.0f);
    const math::Quaternion camera_rotation =
        math::Quaternion::FromEular(math::util::DEG_2_RAD * 45.0f, 0.0f, 0.0f);
    const math::IntVector2 screen_size =
        game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
    const float aspect_ratio = screen_size.x * 1.0f / screen_size.y;
    if (!camera_.Init(L"MainCamera", camera_position, camera_rotation,
                      math::util::DEG_2_RAD * 50.0f, aspect_ratio)) {
      return false;
    }
  }

  return true;
}

//更新
bool PlayerMoveViewer::Update() {
  float update_time =
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();

  if (!player_.Update()) {
    return false;
  }

  if (ImGui::Begin("Camera")) {
    //カメラ座標
    math::Vector3 camera_position = camera_.GetPosition();
    ImGui::SliderFloat3("Position", &camera_position.x, -100.0f, 100.0f);
    camera_.SetPosition(camera_position);
    //カメラ回転角
    math::Vector3 camera_rotation =
        math::Quaternion::ToEular(camera_.GetRotation()) *
        math::util::RAD_2_DEG;
    ImGui::SliderFloat3("Rotation", &camera_rotation.x, -180.0f, 180.0f);
    camera_.SetRotation(
        math::Quaternion::FromEular(camera_rotation * math::util::DEG_2_RAD));

    //カメラの上方向ベクトルを変更する
    if (ImGui::Button("X_UP")) {
      camera_.SetUpVector(math::Vector3::kRightVector);
    }
    if (ImGui::Button("Y_UP")) {
      camera_.SetUpVector(math::Vector3::kUpVector);
    }
    if (ImGui::Button("Z_UP")) {
      camera_.SetUpVector(math::Vector3::kForwardVector);
    }
    float fov = camera_.GetFov() * math::util::RAD_2_DEG;
    ImGui::SliderFloat("FOV", &fov, 0.01f, 90.0f);
    camera_.SetFov(fov * math::util::DEG_2_RAD);
  }
  ImGui::End();

  if (ImGui::Begin("Player")) {
    math::Vector3 position = player_.GetPosition();
    math::Vector3 velocity = player_.GetVelocity();
    float impulse = player_.GetImpulse();
    ImGui::SliderFloat3("Velocity", &velocity.x, -1.0f, 1.0f);
    ImGui::SliderFloat("Impulse", &impulse, 0, 1.0f);
    ImGui::SliderFloat3("Position", &position.x, -100.0f, 100.0f);

    math::Vector3 rotation = math::Quaternion::ToEular(player_.GetRotation()) *
                             math::util::RAD_2_DEG;
    ImGui::SliderFloat3("Rotation", &rotation.x, -180.0f, 180.0f);
    player_.SetRotation(
        math::Quaternion::FromEular(rotation * math::util::DEG_2_RAD));
  }
  ImGui::End();

  if (physics::Collision::GetInstance()->Collision_OBB_DeskOBB(
          player_.GetCollisionRef(), desk_.GetCollisionRef())) {
    // MY_LOG(L"消しゴムと机が衝突しました");
    // player_.SetPosition(player_.GetCollisionRef().GetPosition());
  }

  if (physics::Collision::GetInstance()->Collision_OBB_OBB(
          player_.GetCollisionRef(), graffiti_.GetCollisionRef(), false,
          false)) {
    if (!graffiti_.GetIsErase()) {
      MY_LOG(L"落書きと衝突");
      graffiti_.DecreaseGrafitti(20.0f * update_time);
    }
  }

  //for (i32 i = 0; i < fragments_.size(); i++) {
  //  if (physics::Collision::GetInstance()->Collision_OBB_OBB(
  //          player_.GetCollisionRef(), fragments_[i]->GetCollisionRef(), false,
  //          false)) {
  //    fragments_.erase(fragments_.begin() + i);
  //    player_.UpdateStrength(0.01f);
  //  }
  //}

  return true;
}

//描画
void PlayerMoveViewer::Draw() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
  device.GetHeapManager().SetCommandList(command_list);
  auto& render_resource_manager = device.GetRenderResourceManager();
  render_resource_manager.SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::BACK_BUFFER, false,
      directx::render_target::DepthStencilTargetID::DEPTH_ONLY, true);

  camera_.RenderStart();
  player_.Draw();
  desk_.Draw();
  graffiti_.Draw(command_list);
  //for (auto&& fragment : fragments_) {
  //  fragment.Draw();
  //}

  render_resource_manager.SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::BACK_BUFFER, false,
      directx::render_target::DepthStencilTargetID::NONE, false);
  device.GetHeapManager().SetHeapTableToGraphicsCommandList(device,
                                                            command_list);
  player_.GetCollisionRef().DebugDraw(command_list);
  desk_.GetCollisionRef().DebugDraw(command_list);
  graffiti_.GetCollisionRef().DebugDraw(command_list);
  //for (auto&& fragment : fragments_) {
  //  fragment.GetCollisionRef().DebugDraw(command_list);
  //}
}

//終了
void PlayerMoveViewer::Finalize() {
  game::GameDevice::GetInstance()->GetDevice().WaitExecute();
}

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend