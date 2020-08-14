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
  if (!turn_system_.Init("aaaa")) {
    return false;
  }
  // turn_ = system::Turn::PLAYER_TURN;
  // current_turn_ = system::TurnSystem();
  // physics_field_.Init();

  // auto& device = game::GameDevice::GetInstance()->GetDevice();

  ////プレイヤーの初期化
  //{
  //  player::Player::InitializeParameter player_parameter;
  //  player_parameter.transform =
  //      util::Transform(math::Vector3(0, 10.0f, 0),
  //      math::Quaternion::kIdentity,
  //                      math::Vector3::kUnitVector);
  //  player_parameter.bouding_box_length =
  //      math::Vector3(6.0f, 2.5f, 14.0f) / 4.0f;
  //  player_parameter.min_power = 0;
  //  player_parameter.max_power = 1;
  //  if (!player_.Init(player_parameter)) {
  //    return false;
  //  }
  //  physics_field_.SetPlayer(player_.GetCollisionRef());
  //}

  ////机の初期化
  //{
  //  //本来はステージデータから読み込む
  //  object::Desk::InitializeParameter desk_parameter;
  //  desk_parameter.bounding_box_length =
  //      math::Vector3(120.0f, 5.0f, 80.0f) / 4.0f;
  //  desk_parameter.normal = math::Vector3::kUpVector;
  //  for (i32 i = 0; i < 4; i++) {
  //    math::Vector3 pos = math::Vector3(-120.0f, 0.0f, -80.0f) / 4.0f;
  //    if (i == 1)
  //      pos.x *= -1;
  //    else if (i == 2)
  //      pos *= -1;
  //    else if (i == 3)
  //      pos.z *= -1;
  //    desk_parameter.transform = util::Transform(
  //        pos, math::Quaternion::kIdentity, math::Vector3::kUnitVector);
  //    auto& desk = desks_.emplace_back();
  //    if (!desk.Init(desk_parameter)) {
  //      return false;
  //    }
  //    physics_field_.AddDesk(desk.GetCollisionRef());
  //  }
  //}

  //障害物の初期化
  //{
  //  object::Obstacle::InitializeParameter params;
  //  params.position = math::Vector3(15.0f, 3.0f, 3.0f);
  //  params.model_id = 0;
  //  params.rotation =
  //      math::Quaternion::FromEular(0.0f, 28.12f * math::util::DEG_2_RAD,
  //      0.0f);
  //  params.bounding_box_length = math::Vector3(18.0f, 10.0f, 40.0f) / 4.0f;
  //  auto& obs = obstacles_.emplace_back();
  //  if (!obs.Init(params)) {
  //    return false;
  //  }
  //  physics_field_.AddObstacle(obs.GetCollisionRef());
  //}

  //{
  //  enemy::Enemy::InitializeParameter enemy_parameter;
  //  enemy_parameter.bouding_box_length =
  //      math::Vector3(6.0f, 2.5f, 14.0f) / 4.0f;
  //  for (i32 i = 0; i < 2; i++) {
  //    float x =
  //        game::GameDevice::GetInstance()->GetRandom().Range(-0.5f, 0.5f) *
  //        100.0f;
  //    float z =
  //        game::GameDevice::GetInstance()->GetRandom().Range(-0.25f, 0.25f) *
  //        100.0f;
  //    math::Vector3 pos = math::Vector3(x, 10.0f, z);
  //    enemy_parameter.transform = util::Transform(
  //        pos, math::Quaternion::kIdentity, math::Vector3::kUnitVector);
  //    enemy_manager_.Add(enemy_parameter, physics_field_);
  //  }
  //}

  ////テストで消しカスを出現
  //{
  //  object::Fragment::InitializeParameter params;
  //  params.position = math::Vector3(50.0f, 2.0f, 0.0f);
  //  params.rotation = math::Quaternion::kIdentity;
  //  params.scale = math::Vector3::kUnitVector;
  //  params.bounding_box_length = math::Vector3::kUnitVector;
  //  auto& frag = fragments_.emplace_back();
  //  if (!frag.Init(params)) {
  //    return false;
  //  }
  //  physics_field_.AddFragment(frag.GetCollisionRef());
  //}

  ////カメラの初期化
  //{
  //  const math::Vector3 camera_position = math::Vector3(0, 50.0f, -50.0f);
  //  const math::Quaternion camera_rotation =
  //      math::Quaternion::FromEular(math::util::DEG_2_RAD * 45.0f, 0.0f,
  //      0.0f);
  //  const math::IntVector2 screen_size =
  //      game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
  //  const float aspect_ratio = screen_size.x * 1.0f / screen_size.y;
  //  if (!camera_.Init(L"MainCamera", camera_position, camera_rotation,
  //                    math::util::DEG_2_RAD * 50.0f, aspect_ratio)) {
  //    return false;
  //  }
  //}

  return true;
}

//更新
bool MainScene1::Update() {
  if (!turn_system_.Update()) {
    return false;
  }
  // if (!UpdateTurn()) {
  //  return false;
  //}

  // if (!physics_field_.Update(turn_, player_.GetVelocity(),
  // player_.GetIsMove(),
  //                           player_.GetImpulse(), player_.GetPower(),
  //                           enemy_manager_.GetVelocities(),
  //                           !enemy_manager_.LastEnemyMoveEnd())) {
  //  return false;
  //}

  // player_.SetPosition(physics_field_.GetPlayerOBB().GetPosition());
  // player_.SetVelocity(physics_field_.GetPlayerVelocity());
  // player_.SetRotation(physics_field_.GetPlayerOBB().GetRotation());
  // enemy_manager_.SetPosition(physics_field_);
  // enemy_manager_.SetVelocity(physics_field_);


  return true;
}

//描画
void MainScene1::Draw() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
  auto& render_resource_manager = device.GetRenderResourceManager();
  render_resource_manager.SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::BACK_BUFFER, true,
      directx::render_target::DepthStencilTargetID::DEPTH_ONLY, true);

  turn_system_.Draw();

  render_resource_manager.SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::BACK_BUFFER, false,
      directx::render_target::DepthStencilTargetID::NONE, false);
  turn_system_.DebugDraw();
}

//終了
void MainScene1::Finalize() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  device.WaitExecute();
}

//ターン別の更新処理
bool MainScene1::UpdateTurn() {
  // switch (turn_) {
  //  case legend::system::Turn::PLAYER_TURN:
  //    MY_LOG(L"PLAYER TURN");
  //    if (!player_.Update()) {
  //      return false;
  //    }
  //    //プレイヤーが動き終えたらターン切り替え
  //    if (player_.GetMoveEnd()) {
  //      turn_ = system::Turn::ENEMY_TURN;
  //      player_.ResetMoveEnd();
  //    }
  //    break;
  //  case legend::system::Turn::ENEMY_TURN:
  //    MY_LOG(L"ENEMY TURN");
  //    enemy_manager_.SetPlayer(player_.GetCollisionRef());
  //    if (!enemy_manager_.Update()) {
  //      return false;
  //    }
  //    //最後に登録されているエネミーが動き終えたら又はエネミーが全ていなければ、ターン切り替え
  //    //ターン数加算
  //    if (enemy_manager_.LastEnemyMoveEnd() ||
  //        enemy_manager_.GetEnemiesSize() == 0) {
  //      turn_ = system::Turn::PLAYER_TURN;
  //      current_turn_.AddCurrentTurn();
  //      physics_field_.ResetEnemyMove();
  //    }
  //    break;
  //  default:
  //    break;
  //}
  return true;
}
}  // namespace mainscene
}  // namespace scenes
}  // namespace legend