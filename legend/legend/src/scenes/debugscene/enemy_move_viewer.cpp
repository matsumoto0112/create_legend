#include "src/scenes/debugscene/enemy_move_viewer.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"
#include "src/util/path.h"

namespace legend {
namespace scenes {
namespace debugscene {

//コンストラクタ
EnemyMoveViewer::EnemyMoveViewer(ISceneChange* scene_change)
    : Scene(scene_change) {}

EnemyMoveViewer::~EnemyMoveViewer() {}

//初期化
bool EnemyMoveViewer::Initialize() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  directx::device::CommandList command_list;
  if (!command_list.Init(
          device, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT)) {
    return false;
  }

  math::Vector3 min = math::Vector3::kUnitVector * -1;
  math::Vector3 max = math::Vector3::kUnitVector;
  if (!enemy_manager_.Initilaize(min, max)) {
    return false;
  }

  //カメラの初期化
  {
    const math::Vector3 camera_position = math::Vector3(0, 2, -2);
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

  command_list.Close();
  device.ExecuteCommandList({command_list});
  device.WaitExecute();
  return true;
}

//更新
bool EnemyMoveViewer::Update() {
  if (ImGui::Begin("Transform")) {
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

  // float speed = 0.1f;
  // auto pos = transform_.GetPosition();
  // auto h = game::GameDevice::GetInstance()->GetInput().GetHorizontal();
  // auto v = game::GameDevice::GetInstance()->GetInput().GetVertical();
  // auto mov = (math::Vector3(h, 0, v)).Normalized();
  // transform_.SetPosition(pos + mov * speed/*
  // *game::GameDevice::GetInstance()->GetFPSCounter().GetTotalSeconds()*/);
  // transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  // transform_cb_.UpdateStaging();

  enemy_manager_.Update();

  return true;
}

//描画
void EnemyMoveViewer::Draw() {
  Scene::Draw();

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& render_resource_manager = device.GetRenderResourceManager();
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();

  render_resource_manager.SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::BACK_BUFFER, false,
      directx::render_target::DepthStencilTargetID::DEPTH_ONLY, true);

  camera_.RenderStart();

  enemy_manager_.Draw();
}

void EnemyMoveViewer::Finalize() {
  game::GameDevice::GetInstance()->GetDevice().WaitExecute();
}

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
