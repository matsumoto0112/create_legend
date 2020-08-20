#include "src/scenes/debugscene/enemy_move_viewer.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/game/game_device.h"

namespace legend {
namespace scenes {
namespace debugscene {

//コンストラクタ
EnemyMoveViewer::EnemyMoveViewer(ISceneChange* scene_change)
    : Scene(scene_change) {}

//デストラクタ
EnemyMoveViewer::~EnemyMoveViewer() {}

//初期化
bool EnemyMoveViewer::Initialize() {
  turn_ = system::Mode::PLAYER_MOVING;
  // current_turn_ = system::TurnSystem();
  physics_field_.Init();

  auto& device = game::GameDevice::GetInstance()->GetDevice();

  //プレイヤーの初期化
  {
    player::Player::InitializeParameter player_parameter;
    player_parameter.transform =
        util::Transform(math::Vector3(0, 10.0f, 0), math::Quaternion::kIdentity,
                        math::Vector3::kUnitVector);
    player_parameter.bouding_box_length =
        math::Vector3(6.0f, 2.5f, 14.0f) / 4.0f;
    player_parameter.min_power = 0;
    player_parameter.max_power = 1;
    if (!player_.Init(nullptr, player_parameter)) {
      return false;
    }
    // physics_field_.SetPlayer(player_.GetCollisionRef());
  }

  //机の初期化
  {
    //本来はステージデータから読み込む
    object::Desk::InitializeParameter desk_parameter;
    desk_parameter.bounding_box_length =
        math::Vector3(120.0f, 5.0f, 80.0f) / 4.0f;
    desk_parameter.normal = math::Vector3::kUpVector;
    for (i32 i = 0; i < 4; i++) {
      math::Vector3 pos = math::Vector3(-120.0f, 0.0f, -80.0f) / 4.0f;
      if (i == 1)
        pos.x *= -1;
      else if (i == 2)
        pos *= -1;
      else if (i == 3)
        pos.z *= -1;
      desk_parameter.transform = util::Transform(
          pos, math::Quaternion::kIdentity, math::Vector3::kUnitVector);
      auto& desk = desks_.emplace_back();
      //if (!desk.Init(desk_parameter)) {
      //  return false;
      //}
      // physics_field_.AddDesk(desk.GetCollisionRef());
    }
  }

  //障害物の初期化
  {
    object::Obstacle::InitializeParameter params;
    params.position = math::Vector3(15.0f, 3.0f, 3.0f);
    params.model_id = 0;
    params.rotation =
        math::Quaternion::FromEular(0.0f, 28.12f * math::util::DEG_2_RAD, 0.0f);
    params.bounding_box_length = math::Vector3(6.0f, 2.5f, 14.0f) / 4.0f;
    auto& obs = obstacles_.emplace_back();
    if (!obs.Init(params)) {
      return false;
    }
    // physics_field_.AddObstacle(obs.GetCollisionRef());
  }

  {
    enemy::Enemy::InitializeParameter enemy_parameter;
    enemy_parameter.bouding_box_length =
        math::Vector3(6.0f, 2.5f, 14.0f) / 4.0f;
    for (i32 i = 0; i < 2; i++) {
      float x =
          game::GameDevice::GetInstance()->GetRandom().Range(-0.5f, 0.5f) *
          100.0f;
      float z =
          game::GameDevice::GetInstance()->GetRandom().Range(-0.25f, 0.25f) *
          100.0f;
      math::Vector3 pos = math::Vector3(x, 10.0f, z);
      enemy_parameter.transform = util::Transform(
          pos, math::Quaternion::kIdentity, math::Vector3::kUnitVector);
      enemy_manager_.Add(enemy_parameter, physics_field_);
    }

    {
      // search_manager_.Initialize(&player_.GetCollisionRef());
      search_manager_.Add({
          math::Vector3(1.0f, 0.0f, 1.0f) * 25.0f,
          math::Vector3(-1.0f, 0.0f, 1.0f) * 25.0f,
          math::Vector3(1.0f, 0.0f, -1.0f) * 25.0f,
          math::Vector3(-1.0f, 0.0f, -1.0f) * 25.0f,
      });
      search_manager_.SetBranch(0, {1, 2, 3});
      search_manager_.SetBranch(1, {0, 2, 3});
      search_manager_.SetBranch(2, {0, 1, 3});
      search_manager_.SetBranch(3, {0, 1, 2});
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
bool EnemyMoveViewer::Update() {
  if (!UpdateTurn()) {
    return false;
  }

  if (!physics_field_.Update(turn_, player_.GetVelocity(), player_.GetImpulse(),
                             player_.GetPower(),
                             enemy_manager_.GetVelocities())) {
    return false;
  }

  player_.SetPosition(physics_field_.GetPlayerOBB().GetPosition());
  player_.SetVelocity(physics_field_.GetPlayerVelocity());
  player_.SetRotation(physics_field_.GetPlayerOBB().GetRotation());
  enemy_manager_.SetPosition(physics_field_);
  enemy_manager_.SetVelocity(physics_field_);

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
      camera_.SetPosition(math::Vector3(0, 50.0f, -50.0f));
      camera_.SetRotation(math::Quaternion::FromEular(
          math::util::DEG_2_RAD * 45.0f, 0.0f, 0.0f));
      camera_.SetUpVector(math::Vector3::kUpVector);
    }
    if (ImGui::Button("RightCamera")) {
      camera_.SetPosition(math::Vector3(50.0f, 5.0f, 0));
      camera_.SetRotation(math::Quaternion::FromEular(
          0.0f, math::util::DEG_2_RAD * -90.0f, 0.0f));
      camera_.SetUpVector(math::Vector3::kUpVector);
    }
    if (ImGui::Button("UpCamera")) {
      camera_.SetPosition(math::Vector3(0, 100.0f, 0));
      camera_.SetRotation(math::Quaternion::FromEular(
          math::util::DEG_2_RAD * 90.0f, 0.0f, 0.0f));
      camera_.SetUpVector(math::Vector3::kForwardVector);
    }
  }
  ImGui::End();

  return true;
}

//描画
void EnemyMoveViewer::Draw() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
  auto& render_resource_manager = device.GetRenderResourceManager();
  render_resource_manager.SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::BACK_BUFFER, true,
      directx::render_target::DepthStencilTargetID::DEPTH_ONLY, true);

  camera_.RenderStart();
  player_.Draw();
  for (auto&& desk : desks_) {
    desk.Draw();
  }
  enemy_manager_.Draw();
  for (auto&& obs : obstacles_) {
    obs.Draw();
  }

  render_resource_manager.SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::BACK_BUFFER, false,
      directx::render_target::DepthStencilTargetID::NONE, false);
  // player_.GetCollisionRef().DebugDraw(command_list);
  // for (auto&& desk : desks_) {
  //  desk.GetCollisionRef().DebugDraw(command_list);
  //}
  // for (auto&& obs : obstacles_) {
  //  obs.GetCollisionRef().DebugDraw(command_list);
  //}
  enemy_manager_.DebugDraw(command_list);
  search_manager_.DebugDraw(command_list);
}

//終了
void EnemyMoveViewer::Finalize() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  device.WaitExecute();
}

//ターン別の更新処理
bool EnemyMoveViewer::UpdateTurn() {
  switch (turn_) {
    case legend::system::Mode::PLAYER_MOVING:
      MY_LOG(L"PLAYER TURN");
      if (!player_.Update()) {
        return false;
      }
      //プレイヤーが動き終えたらターン切り替え
      if (player_.GetMoveEnd()) {
        turn_ = system::Mode::ENEMY_MOVING;
      }
      break;
    case legend::system::Mode::ENEMY_MOVING:
      MY_LOG(L"ENEMY TURN");
      // enemy_manager_.SetPlayer(player_.GetCollisionRef());
      if (!enemy_manager_.Update(&search_manager_)) {
        return false;
      }
      //最後に登録されているエネミーが動き終えたら又はエネミーが全ていなければ、ターン切り替え
      //ターン数加算
      if (enemy_manager_.LastEnemyMoveEnd() ||
          enemy_manager_.GetEnemiesSize() == 0) {
        turn_ = system::Mode::PLAYER_MOVING;
        // current_turn_.AddCurrentTurn();

        //{ //ボス追加
        //  enemy::Boss::InitializeParameter boss_parameter;
        //  boss_parameter.bouding_box_length =
        //      math::Vector3(6.0f, 2.5f, 14.0f) / 4.0f * 2.0f;
        //  float x =
        //      game::GameDevice::GetInstance()->GetRandom().Range(-0.5f, 0.5f)
        //      * 100.0f;
        //  float z = game::GameDevice::GetInstance()->GetRandom().Range(-0.25f,
        //                                                               0.25f)
        //                                                               *
        //            100.0f;
        //  math::Vector3 pos = math::Vector3(x, 10.0f, z);
        //  boss_parameter.transform =
        //      util::Transform(pos, math::Quaternion::kIdentity,
        //                      math::Vector3::kUnitVector * 2.0f);
        //  enemy_manager_.Add(boss_parameter, physics_field_);
        //}
        physics_field_.ResetEnemyMove();
      }
      break;
    default:
      break;
  }
  return true;
}
}  // namespace debugscene
}  // namespace scenes
}  // namespace legend