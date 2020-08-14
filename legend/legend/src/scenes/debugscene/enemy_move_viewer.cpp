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
  physics_field_.Init();

  directx::device::CommandList command_list;
  if (!command_list.Init(
          device, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT)) {
    return false;
  }

  if (!enemy_manager_.Initilaize()) {
    return false;
  }

  //机の初期化
  {
    //本来はステージデータから読み込む
    object::Desk::InitializeParameter desk_parameter;
    desk_parameter.bounding_box_length =
        math::Vector3(1.2f, 0.05f, 0.8f) / 4.0f;
    desk_parameter.normal = math::Vector3::kUpVector;
    for (i32 i = 0; i < 4; i++) {
      math::Vector3 pos = math::Vector3(-1.2f, 0, -0.8f) / 4.0f;
      if (i == 1)
        pos.x *= -1;
      else if (i == 2)
        pos *= -1;
      else if (i == 3)
        pos.z *= -1;
      desk_parameter.transform = util::Transform(
          pos, math::Quaternion::kIdentity, math::Vector3::kUnitVector);
      auto& desk = desks_.emplace_back();
      if (!desk.Init(desk_parameter)) {
        return false;
      }
      physics_field_.AddDesk(desk.GetCollisionRef());
    }
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
  enemy_manager_.SetPosition(physics_field_);
  enemy_manager_.SetVelocity(physics_field_);

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

  // 敵追加
  if (game::GameDevice::GetInstance()->GetInput().GetKeyboard()->GetKeyDown(
          input::key_code::A)) {
    enemy::Enemy::InitializeParameter enemy_parameter;
    enemy_parameter.bouding_box_length =
        math::Vector3(0.06f, 0.025f, 0.14f) / 4.0f;
    float x = game::GameDevice::GetInstance()->GetRandom().Range(-0.5f, 0.5f);
    float z = game::GameDevice::GetInstance()->GetRandom().Range(-0.25f, 0.25f);
    math::Vector3 pos = math::Vector3(x, 0.1f, z);
    enemy_parameter.transform = util::Transform(
        pos, math::Quaternion::kIdentity, math::Vector3::kUnitVector);
    enemy_manager_.Add(enemy_parameter, physics_field_);
  }
  // 敵削除
  else if (game::GameDevice::GetInstance()
               ->GetInput()
               .GetKeyboard()
               ->GetKeyDown(input::key_code::D)) {
    enemy_manager_.RandomDestroy(physics_field_);
  }
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

  for (auto&& desk : desks_) {
    desk.Draw();
  }
  enemy_manager_.Draw();
}

void EnemyMoveViewer::Finalize() {
  game::GameDevice::GetInstance()->GetDevice().WaitExecute();
}

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
