#include "src/scenes/debugscene/stage_generate_test.h"

#include "src/game/game_device.h"
#include "src/util/path.h"
#include "src/util/string_util.h"

namespace legend {
namespace scenes {
namespace debugscene {
StageGenerateTest::StageGenerateTest(ISceneChange* scene_change)
    : Scene(scene_change) {}
StageGenerateTest::~StageGenerateTest() {}
bool StageGenerateTest::Initialize() {
  indexs_.clear();
  indexs_.push_back("no data");

  physics_field_.Init();

  //カメラの初期化
  const math::Vector3 camera_position = math::Vector3(0, 0.5f, -0.5f);
  const math::Quaternion camera_rotation =
      math::Quaternion::FromEular(math::util::DEG_2_RAD * 45.0f, 0.0f, 0.0f);
  const math::IntVector2 screen_size =
      game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
  const float aspect_ratio = screen_size.x * 1.0f / screen_size.y;
  if (!camera_.Init(L"MainCamera", camera_position, camera_rotation,
                    math::util::DEG_2_RAD * 50.0f, aspect_ratio)) {
    MY_LOG(L"カメラの初期化に失敗しました。");
  }

  //プレイヤーの初期化
  {
    player::Player::InitializeParameter player_parameter;
    player_parameter.transform =
        util::Transform(math::Vector3(0, 0.1f, 0), math::Quaternion::kIdentity,
                        math::Vector3::kUnitVector);
    player_parameter.bouding_box_length =
        math::Vector3(0.06f, 0.025f, 0.14f) / 4.0f;
    player_parameter.min_power = 0;
    player_parameter.max_power = 1;
    if (!player_.Init(player_parameter)) {
      // return false;
    }
    physics_field_.SetPlayer(player_.GetCollisionRef());
  }

  ////机の初期化
  //{
  //    //本来はステージデータから読み込む
  //    object::Desk::InitializeParameter desk_parameter;
  //    desk_parameter.transform =
  //        util::Transform(math::Vector3::kZeroVector,
  //        math::Quaternion::kIdentity,
  //            math::Vector3::kUnitVector);
  //    desk_parameter.bounding_box_length =
  //        math::Vector3(1.2f, 0.05f, 0.8f) / 4.0f;
  //    desk_parameter.normal = math::Vector3::kUpVector;
  //    auto& obs = desks_.emplace_back();
  //    if (!obs.Init(desk_parameter)) {
  //    }
  //    physics_field_.AddDesk(obs.GetCollisionRef());
  //}
  ////障害物の初期化
  //{
  //    object::Obstacle::InitializeParameter params;
  //    params.position = math::Vector3(0.15f, 0.1f, 0.03f);
  //    params.model_id = 0;
  //    params.rotation =
  //        math::Quaternion::FromEular(0.0f, 28.12f * math::util::DEG_2_RAD,
  //        0.0f);
  //    params.bounding_box_length = math::Vector3(0.06f, 0.025f, 0.14f) / 4.0f;
  //    auto& obs = obstacles_.emplace_back();
  //    if (!obs.Init(params)) {
  //    }
  //    physics_field_.AddObstacle(obs.GetCollisionRef());
  //}
  return true;
}
bool StageGenerateTest::Update() {
  //ステージ選択
  if (ImGui::Begin("StageGenerateTest")) {
    if (ImGui::Button("Load aaaa")) {
      indexs_ = stage_generator_->LoadStringStageData(
          util::Path::GetInstance()->exe() / L"assets" / L"stage" /
          L"aaaa.txt");
      // stage_generator_->LoadStage(
      //    util::Path::GetInstance()->exe() / L"assets" / L"stage" /
      //    L"aaaa.txt", "aaaa", &physics_field_, &map_actors_,
      //    &enemy_manager_);
      stage_generator_->LoadStage(
          util::Path::GetInstance()->exe() / L"assets" / L"stage" / L"aaaa.txt",
          "aaaa", &physics_field_, &desks_, &obstacles_, &player_,
          &enemy_manager_);
    }
  }
  ImGui::End();

  //カメラの設定
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

    if (ImGui::Button("BackCamera")) {
      camera_.SetPosition(math::Vector3(0, 0.5f, -0.5f));
      camera_.SetRotation(math::Quaternion::FromEular(
          math::util::DEG_2_RAD * 45.0f, 0.0f, 0.0f));
      camera_.SetUpVector(math::Vector3::kUpVector);
    }
    if (ImGui::Button("RightCamera")) {
      camera_.SetPosition(math::Vector3(0.5f, 0.05f, 0));
      camera_.SetRotation(math::Quaternion::FromEular(
          0.0f, math::util::DEG_2_RAD * -90.0f, 0.0f));
      camera_.SetUpVector(math::Vector3::kUpVector);
    }
    if (ImGui::Button("UpCamera")) {
      camera_.SetPosition(math::Vector3(0, 1.0f, 0));
      camera_.SetRotation(math::Quaternion::FromEular(
          math::util::DEG_2_RAD * 90.0f, 0.0f, 0.0f));
      camera_.SetUpVector(math::Vector3::kForwardVector);
    }
  }
  ImGui::End();

  //読み込んだステージ情報
  if (ImGui::Begin("LoadData")) {
    for (auto&& index : indexs_) {
      ImGui::Text(index.c_str());
    }
  }
  ImGui::End();
  return true;
}
void StageGenerateTest::Draw() {
    auto& device = game::GameDevice::GetInstance()->GetDevice();
    auto& resource = game::GameDevice::GetInstance()->GetResource();
    auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
    device.GetRenderResourceManager().SetRenderTargets(
        command_list, directx::render_target::RenderTargetID::BACK_BUFFER, true,
        directx::render_target::DepthStencilTargetID::DEPTH_ONLY, true);

    auto& render_resource_manager = device.GetRenderResourceManager();
    render_resource_manager.SetRenderTargets(
        command_list, directx::render_target::RenderTargetID::BACK_BUFFER, false,
        directx::render_target::DepthStencilTargetID::DEPTH_ONLY, true);

  camera_.RenderStart();

  // for (auto object : map_actors_) {
  //    object.Draw();
  //}
  player_.Draw();
  enemy_manager_.Draw();
  for (auto object : desks_) {
    object.Draw();
  }
  for (auto object : obstacles_) {
    object.Draw();
  }


  render_resource_manager.SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::BACK_BUFFER, false,
      directx::render_target::DepthStencilTargetID::NONE, false);
  device.GetHeapManager().SetHeapTableToGraphicsCommandList(device, command_list);

  // for (auto object : map_actors_) {
  //    object.GetCollisionRef().DebugDraw(command_list);
  //}
  player_.GetCollisionRef().DebugDraw(command_list);
  for (auto&& obs : desks_) {
    obs.GetCollisionRef().DebugDraw(command_list);
  }
  for (auto&& obs : obstacles_) {
    obs.GetCollisionRef().DebugDraw(command_list);
  }
}
void StageGenerateTest::Finalize() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  device.WaitExecute();
}
}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
