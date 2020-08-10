#include "src/scenes/debugscene/stage_generate_test.h"

#include "src/game/game_device.h"
#include "src/util/path.h"
#include "src/util/string_util.h"

namespace legend {
namespace scenes {
namespace debugscene {
StageGenerateTest::StageGenerateTest(ISceneChange* scene_change)
    : Scene(scene_change) {
  indexs_.clear();
  indexs_.push_back("no data");

  //プレイヤーの初期化
  player::Player::InitializeParameter player_parameter;
  player_parameter.transform =
      util::Transform(math::Vector3::kZeroVector, math::Quaternion::kIdentity,
                      math::Vector3::kUnitVector);
  player_parameter.bouding_box_length = math::Vector3(0.1f, 0.05f, 0.2f);
  player_parameter.min_power = 0;
  player_parameter.max_power = 1;
  if (!player_.Init(player_parameter)) {
    MY_LOG(L"プレイヤーの初期化に失敗しました。");
  }

  //机の初期化
  //本来はステージデータから読み込む
  object::Desk::InitializeParameter desk_parameter;
  desk_parameter.transform =
      util::Transform(math::Vector3::kZeroVector, math::Quaternion::kIdentity,
                      math::Vector3::kUnitVector);
  desk_parameter.bounding_box_length = math::Vector3(0.3f, 0.05f, 0.2f);
  desk_parameter.normal = math::Vector3::kUpVector;
  if (!desk_.Init(desk_parameter)) {
    MY_LOG(L"机の初期化に失敗しました。");
  }

  //カメラの初期化
  const math::Vector3 camera_position = math::Vector3(0, 3.5f, -2.5f);
  const math::Quaternion camera_rotation =
      math::Quaternion::FromEular(math::util::DEG_2_RAD * 45.0f, 0.0f, 0.0f);
  const math::IntVector2 screen_size =
      game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
  const float aspect_ratio = screen_size.x * 1.0f / screen_size.y;
  if (!camera_.Init(L"MainCamera", camera_position, camera_rotation,
                    math::util::DEG_2_RAD * 50.0f, aspect_ratio)) {
    MY_LOG(L"カメラの初期化に失敗しました。");
  }

  // auto& device = game::GameDevice::GetInstance()->GetDevice();
  // auto& resource = game::GameDevice::GetInstance()->GetResource();

  // directx::device::CommandList command_list;
  // if (!command_list.Init(
  //    device, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT)) {
  //}
}
StageGenerateTest::~StageGenerateTest() {}
bool StageGenerateTest::Initialize() { return true; }
bool StageGenerateTest::Update() {
  //ステージ選択
  if (ImGui::Begin("StageGenerateTest")) {
    if (ImGui::Button("Load aaaa")) {
      indexs_ = stage_generator_->LoadStringStageData(
          util::Path::GetInstance()->exe() / L"assets" / L"stage" /
          L"aaaa.txt");
      stage_generator_->LoadStage(
          util::Path::GetInstance()->exe() / L"assets" / L"stage" / L"aaaa.txt",
          &desk_, &player_, &enemy_manager_);
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
  }
  ImGui::End();

  //読み込んだ設定
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
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
  device.GetHeapManager().SetGraphicsCommandList(command_list);
  auto& render_resource_manager = device.GetRenderResourceManager();
  render_resource_manager.SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::BACK_BUFFER, false,
      directx::render_target::DepthStencilTargetID::DEPTH_ONLY, true);

  camera_.RenderStart();
  player_.Draw();
  desk_.Draw();
  enemy_manager_.Draw();

   render_resource_manager.SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::BACK_BUFFER,
      false, directx::render_target::DepthStencilTargetID::NONE, false);
   device.GetHeapManager().UpdateGlobalHeap(device, command_list);
   player_.GetCollisionRef().DebugDraw(command_list);
   desk_.GetCollisionRef().DebugDraw(command_list);
}
void StageGenerateTest::Finalize() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  device.WaitExecute();
}
}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
