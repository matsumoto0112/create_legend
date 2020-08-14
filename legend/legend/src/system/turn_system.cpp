#include "src/system/turn_system.h"

namespace legend {
namespace system {

//コンストラクタ
TurnSystem::TurnSystem() : current_turn_(0) {}

//デストラクタ
TurnSystem::~TurnSystem() {}

//初期化
bool TurnSystem::Init(const std::string& stage_name) {
  //ステージデータの拡張子は.txt
  auto stage_path = util::Path::GetInstance()->exe() / "assets" / "stage" /
                    (stage_name + ".txt");

  if (!physics_field_.Init()) {
    return false;
  }
  if (!enemy_manager_.Initilaize()) {
    return false;
  }

  if (!stage_generator_.LoadStage(stage_path, stage_name, &physics_field_,
                                  &desks_, &obstacles_, &player_,
                                  &enemy_manager_)) {
    return false;
  }

  physics_field_.SetPlayer(player_.GetCollisionRef());

  player_.SetMediator(this);
  for (auto&& desk : desks_) {
    physics_field_.AddDesk(desk.GetCollisionRef());
    desk.SetMediator(this);
  }
  for (auto&& obs : obstacles_) {
    physics_field_.AddObstacle(obs.GetCollisionRef());
    obs.SetMediator(this);
  }

  //カメラの初期化
  {
    const math::Vector3 camera_position = math::Vector3(0, 50.0f, -50.0f);
    const math::Quaternion camera_rotation =
        math::Quaternion::FromEular(math::util::DEG_2_RAD * 45.0f, 0.0f, 0.0f);
    const math::IntVector2 screen_size =
        game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
    const float aspect_ratio = screen_size.x * 1.0f / screen_size.y;
    if (!main_camera_.Init(L"MainCamera", &player_,
                           math::Vector3(0.0f, 30.0f, -30.0f), camera_rotation,
                           math::util::DEG_2_RAD * 50.0f, aspect_ratio,
                           math::Vector3::kUpVector, 0.1f, 300.0f)) {
      return false;
    }
  }

  return true;
}

bool TurnSystem::Update() {
  countdown_timer_.Update();
  player_.Update();

  const std::unordered_map<Mode, std::function<bool()>> switcher = {
      {Mode::PLAYER_MOVE_READY, [&]() { return PlayerMoveReady(); }},
      {Mode::PLAYER_MOVING, [&]() { return PlayerMoving(); }},
      {Mode::PLAYER_SKILL_AFTER_MOVED,
       [&]() { return PlayerSkillAfterModed(); }},
      {Mode::ENEMY_MOVING, [&]() { return EnemyMove(); }},
      {Mode::ENEMY_MOVE_END, [&]() { return EnemyMoveEnd(); }},
  };
  if (!switcher.at(current_mode_)()) {
    return false;
  }

  //物理処理の適用
  if (!physics_field_.Update(current_mode_, player_.GetVelocity(),
                             player_.GetIsMove(), player_.GetImpulse(),
                             player_.GetPower(), enemy_manager_.GetVelocities(),
                             !enemy_manager_.LastEnemyMoveEnd())) {
    return false;
  }

  //物理処理の結果を各オブジェクトに更新させる
  player_.SetPosition(physics_field_.GetPlayerOBB().GetPosition());
  player_.SetVelocity(physics_field_.GetPlayerVelocity());
  player_.SetRotation(physics_field_.GetPlayerOBB().GetRotation());
  enemy_manager_.SetPosition(physics_field_);
  enemy_manager_.SetVelocity(physics_field_);

  if (ImGui::Begin("Camera")) {
    //カメラ座標
    math::Vector3 camera_position = main_camera_.GetPosition();
    ImGui::SliderFloat3("Position", &camera_position.x, -100.0f, 100.0f);
    main_camera_.SetPosition(camera_position);
    //カメラ回転角
    math::Vector3 camera_rotation =
        math::Quaternion::ToEular(main_camera_.GetRotation()) *
        math::util::RAD_2_DEG;
    ImGui::SliderFloat3("Rotation", &camera_rotation.x, -180.0f, 180.0f);
    main_camera_.SetRotation(
        math::Quaternion::FromEular(camera_rotation * math::util::DEG_2_RAD));

    //カメラの上方向ベクトルを変更する
    if (ImGui::Button("X_UP")) {
      main_camera_.SetUpVector(math::Vector3::kRightVector);
    }
    if (ImGui::Button("Y_UP")) {
      main_camera_.SetUpVector(math::Vector3::kUpVector);
    }
    if (ImGui::Button("Z_UP")) {
      main_camera_.SetUpVector(math::Vector3::kForwardVector);
    }
    float fov = main_camera_.GetFov() * math::util::RAD_2_DEG;
    ImGui::SliderFloat("FOV", &fov, 0.01f, 90.0f);
    main_camera_.SetFov(fov * math::util::DEG_2_RAD);

    if (ImGui::Button("BackCamera")) {
      main_camera_.SetPosition(math::Vector3(0, 50.0f, -50.0f));
      main_camera_.SetRotation(math::Quaternion::FromEular(
          math::util::DEG_2_RAD * 45.0f, 0.0f, 0.0f));
      main_camera_.SetUpVector(math::Vector3::kUpVector);
    }
    if (ImGui::Button("RightCamera")) {
      main_camera_.SetPosition(math::Vector3(50.0f, 5.0f, 0));
      main_camera_.SetRotation(math::Quaternion::FromEular(
          0.0f, math::util::DEG_2_RAD * -90.0f, 0.0f));
      main_camera_.SetUpVector(math::Vector3::kUpVector);
    }
    if (ImGui::Button("UpCamera")) {
      main_camera_.SetPosition(math::Vector3(0, 100.0f, 0));
      main_camera_.SetRotation(math::Quaternion::FromEular(
          math::util::DEG_2_RAD * 90.0f, 0.0f, 0.0f));
      main_camera_.SetUpVector(math::Vector3::kForwardVector);
    }
  }
  ImGui::End();

  return true;
}

//プレイヤーの移動準備
bool TurnSystem::PlayerMoveReady() {
  //プレイヤーの速度更新は入力を受け取って処理する
  player_.SetVelocity();
  player_.SetImpulse();
  return true;
}

//プレイヤーの移動中処理
bool TurnSystem::PlayerMoving() { return true; }

bool TurnSystem::PlayerSkillAfterModed() {
  current_mode_ = Mode::ENEMY_MOVING;
  return true;
}

//敵の移動処理
bool TurnSystem::EnemyMove() {
  MY_LOG(L"EnemyMove");
  enemy_manager_.Update(nullptr);
  enemy_manager_.SetPlayer(player_.GetCollisionRef());
  if (enemy_manager_.GetEnemiesSize() == 0 ||
      enemy_manager_.LastEnemyMoveEnd()) {
    current_mode_ = Mode::ENEMY_MOVE_END;
    physics_field_.ResetEnemyMove();
    AddCurrentTurn();
  }
  return true;
}

//敵の移動終了時処理
bool TurnSystem::EnemyMoveEnd() {
  current_mode_ = Mode::PLAYER_MOVE_READY;
  return true;
}

//描画
void TurnSystem::Draw() {
  main_camera_.RenderStart();
  player_.Draw();
  for (auto&& desk : desks_) {
    desk.Draw();
  }
  enemy_manager_.Draw();
  for (auto&& obs : obstacles_) {
    obs.Draw();
  }
}

//デバッグ描画
void TurnSystem::DebugDraw() {
  auto& command_list = game::GameDevice::GetInstance()
                           ->GetDevice()
                           .GetCurrentFrameResource()
                           ->GetCommandList();

  player_.GetCollisionRef().DebugDraw(command_list);
  for (auto&& desk : desks_) {
    desk.GetCollisionRef().DebugDraw(command_list);
  }
  enemy_manager_.DebugDraw(command_list);
  for (auto&& obs : obstacles_) {
    obs.GetCollisionRef().DebugDraw(command_list);
  }
}

//ターン数の増加
void TurnSystem::AddCurrentTurn() { current_turn_++; }

//現在のターン数を取得
i32 TurnSystem::GetCurrentTurn() { return current_turn_; }

//プレイヤーの移動開始時処理
void TurnSystem::PlayerMoveStartEvent() { current_mode_ = Mode::PLAYER_MOVING; }

//プレイヤーの移動終了時処理
void TurnSystem::PlayerMoveEndEvent() {
  player_.ResetMoveEnd();
  // 0.1秒後にモードを切り替える
  countdown_timer_.Init(
      0.1f, [&]() { current_mode_ = Mode::PLAYER_SKILL_AFTER_MOVED; });
}

//プレイヤーのスキル発動時処理
void TurnSystem::PlayerSkillActivate() {}

//プレイヤーのスキル発動終了時処理
void TurnSystem::PlayerSkillDeactivate() {}

player::Player* TurnSystem::GetPlayer() { return &player_; }

std::vector<enemy::Enemy*> TurnSystem::GetEnemies() {
  return enemy_manager_.GetEnemyPointers();
}

}  // namespace system
}  // namespace legend