#include "src/scenes/mainscene/main_scene_1.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/game/game_device.h"

namespace legend {
namespace scenes {
namespace mainscene {

//コンストラクタ
MainScene1::MainScene1(ISceneChange* scene_change) : Scene(scene_change) {}

//デストラクタ
MainScene1::~MainScene1() {}

//初期化
bool MainScene1::Initialize() {
  turn_ = system::Turn::PLAYER_TURN;
  current_turn_ = system::TurnSystem();
  physics_field_.Init();

  auto& device = game::GameDevice::GetInstance()->GetDevice();

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
      return false;
    }
    physics_field_.SetPlayer(player_.GetCollisionRef());
  }

  //机の初期化
  {
    //本来はステージデータから読み込む
    object::Desk::InitializeParameter desk_parameter;
    desk_parameter.transform =
        util::Transform(math::Vector3::kZeroVector, math::Quaternion::kIdentity,
                        math::Vector3::kUnitVector);
    desk_parameter.bounding_box_length =
        math::Vector3(1.2f, 0.05f, 0.8f) / 4.0f;
    desk_parameter.normal = math::Vector3::kUpVector;
    if (!desk_.Init(desk_parameter)) {
      return false;
    }
    physics_field_.AddDesk(desk_.GetCollisionRef());
  }
  //障害物の初期化
  {
    object::Obstacle::InitializeParameter params;
    params.position = math::Vector3(0.15f, 0.03f, 0.03f);
    params.model_id = 0;
    params.rotation =
        math::Quaternion::FromEular(0.0f, 28.12f * math::util::DEG_2_RAD, 0.0f);
    params.bounding_box_length = math::Vector3(0.06f, 0.025f, 0.14f) / 4.0f;
    auto& obs = obstacles_.emplace_back();
    if (!obs.Init(params)) {
      return false;
    }
    physics_field_.AddObstacle(obs.GetCollisionRef());
  }

  //{
  //  if (!enemy_manager_.Initilaize()) {
  //    return false;
  //  }
  //  enemy::Enemy::InitializeParameter enemy_parameter;
  //  enemy_parameter.transform = util::Transform(math::Vector3(-0.04f, 0.1f, 0),
  //                                              math::Quaternion::kIdentity,
  //                                              math::Vector3::kUnitVector);
  //  enemy_parameter.bouding_box_length =
  //      math::Vector3(0.06f, 0.025f, 0.14f) / 4.0f;
  //  enemy_manager_.Add(enemy_parameter);
  //}

  //カメラの初期化
  {
    const math::Vector3 camera_position = math::Vector3(0, 0.5f, -0.5f);
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
bool MainScene1::Update() {
  if (!UpdateTurn()) {
    return false;
  }
  if (!physics_field_.Update(turn_, player_.GetVelocity(), player_.GetIsMove(),
                             player_.GetImpulse(), player_.GetPower(),
                             enemy_manager_.GetVelocities(),
                             !enemy_manager_.LastEnemyMoveEnd())) {
    return false;
  }

  player_.SetPosition(physics_field_.GetPlayerOBB().GetPosition());
  enemy_manager_.SetPosition();
  player_.SetVelocity(physics_field_.GetPlayerVelocity());
  enemy_manager_.SetVelocity();
  player_.SetRotation(physics_field_.GetPlayerRotation());

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

  return true;
}

//描画
void MainScene1::Draw() {
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
  player_.Draw();
  desk_.Draw();
  enemy_manager_.Draw();
  for (auto&& obs : obstacles_) {
    obs.Draw();
  }

  render_resource_manager.SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::BACK_BUFFER, false,
      directx::render_target::DepthStencilTargetID::NONE, false);
  device.GetHeapManager().UpdateGlobalHeap(device, command_list);
  player_.GetCollisionRef().DebugDraw(command_list);
  desk_.GetCollisionRef().DebugDraw(command_list);
  for (auto&& obs : obstacles_) {
    obs.GetCollisionRef().DebugDraw(command_list);
  }
  enemy_manager_.DebugDraw(command_list);
}

//終了
void MainScene1::Finalize() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  device.WaitExecute();
}

//ターン別の更新処理
bool MainScene1::UpdateTurn() {
  switch (turn_) {
    case legend::system::Turn::PLAYER_TURN:
      MY_LOG(L"PLAYER TURN");
      if (!player_.Update()) {
        return false;
      }
      //プレイヤーが動き終えたらターン切り替え
      if (player_.GetMoveEnd()) {
        turn_ = system::Turn::ENEMY_TURN;
        player_.ResetMoveEnd();
      }
      break;
    case legend::system::Turn::ENEMY_TURN:
      MY_LOG(L"ENEMY TURN");
      if (!enemy_manager_.Update()) {
        return false;
      }
      //最後に登録されているエネミーが動き終えたら又はエネミーが全ていなければ、ターン切り替え
      //ターン数加算
      if (enemy_manager_.LastEnemyMoveEnd() ||
          enemy_manager_.GetEnemiesSize() == 0) {
        turn_ = system::Turn::PLAYER_TURN;
        current_turn_.AddCurrentTurn();
        physics_field_.ResetEnemyMove();
      }
      break;
    default:
      break;
  }
  return true;
}
}  // namespace mainscene
}  // namespace scenes
}  // namespace legend