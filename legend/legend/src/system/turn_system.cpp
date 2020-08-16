#include "src/system/turn_system.h"

namespace {
std::vector<legend::ui::UIComponent*> components_;
std::vector<std::string> ui_image_names_;
}  // namespace

namespace legend {
namespace system {

//コンストラクタ
TurnSystem::TurnSystem()
    : current_turn_(0), current_camera_(camera_mode::Sub1) {}

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

  if (!stage_generator_.LoadStage(stage_path, stage_name, &desks_, &obstacles_,
                                  &player_)) {
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
  for (auto&& enemy_parameter :
       stage_generator_.GetEnemyParameters(current_turn_)) {
    enemy_manager_.Add(enemy_parameter, physics_field_);
  }

  if (!InitCameras()) {
    return false;
  }

  {
    search_manager_.Initialize(&player_.GetCollisionRef());
    search_manager_.Add({
        math::Vector3(1.0f, 0.0f, 1.0f) * 10.0f,
        math::Vector3(-1.0f, 0.0f, 1.0f) * 10.0f,
        math::Vector3(1.0f, 0.0f, -1.0f) * 10.0f,
        math::Vector3(-1.0f, 0.0f, -1.0f) * 10.0f,
    });
    search_manager_.SetBranch(0, {1, 2, 3});
    search_manager_.SetBranch(1, {0, 2, 3});
    search_manager_.SetBranch(2, {0, 1, 3});
    search_manager_.SetBranch(3, {0, 1, 2});
  }

  std::ifstream ifs(util::Path::GetInstance()->exe() / "assets" / "parameters" /
                    "main_ui.txt");
  std::string line;
  while (std::getline(ifs, line)) {
    auto split = util::string_util::StringSplit(line, ',');
    MY_ASSERTION(split.size() == 4, L"フォーマットが不正です。");
    std::string name = split[0];
    std::wstring w_name = util::string_util::String_2_WString(name + ".png");
    float x = std::stof(split[1]);
    float y = std::stof(split[2]);
    float z = std::stof(split[3]);

    auto image = std::make_unique<ui::Image>();
    if (!image->Init(w_name)) {
      return false;
    }
    image->SetPosition(math::Vector2(x, y));
    components_.emplace_back(ui_board_.AddComponent(std::move(image)));
    ui_image_names_.emplace_back(name);
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
    if (ImGui::Button("Main")) {
      current_camera_ = camera_mode::Main;
    }
    if (ImGui::Button("Sub1")) {
      current_camera_ = camera_mode::Sub1;
    }
  }
  ImGui::End();

  if (ImGui::Begin("UI")) {
    const u32 size = static_cast<u32>(components_.size());
    for (u32 i = 0; i < size; i++) {
      std::stringstream ss;
      ss << "Image:" << i;
      ImGui::Text(ss.str().c_str());

      math::Vector2 pos = components_[i]->GetPosition();
      float field[2] = {pos.x, pos.y};
      ss.str("");
      ss.clear(std::stringstream::goodbit);
      ss << "Position:" << i;
      ImGui::SliderFloat2(ss.str().c_str(), field, 0.0f, 2000.0f);
      components_[i]->SetPosition(math::Vector2(field[0], field[1]));

      float z = components_[i]->GetZOrder();
      ss.str("");
      ss.clear(std::stringstream::goodbit);
      ss << "Z_Order:" << i;
      ImGui::SliderFloat(ss.str().c_str(), &z, 0.0f, 1.0f);
      components_[i]->SetZOrder(z);
    }

    if (ImGui::Button("Apply")) {
      // std::ofstream ofs(std::filesystem::path("../../../../../") / "legend" /
      //                      "assets" / "parameters" / "main_ui.txt",
      //                  std::ios::out);
      std::ofstream ofs(
          std::filesystem::path("assets") / "parameters" / "main_ui.txt",
          std::ios::out);
      const u32 size = static_cast<u32>(components_.size());
      for (u32 i = 0; i < size; i++) {
        ofs << ui_image_names_[i] << "," << components_[i]->GetPosition().x
            << "," << components_[i]->GetPosition().y << ","
            << components_[i]->GetZOrder() << "\n";
      }
      ofs.flush();
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
  //MY_LOG(L"EnemyMove");
  enemy_manager_.Update(&search_manager_);
  enemy_manager_.SetPlayer(player_.GetCollisionRef());
  if (enemy_manager_.GetEnemiesSize() == 0 ||
      enemy_manager_.LastEnemyMoveEnd()) {
    current_mode_ = Mode::ENEMY_MOVE_END;
    physics_field_.ResetEnemyMove();
    enemy_manager_.ResetEnemyMove();
    AddCurrentTurn();
  }
  return true;
}

//敵の移動終了時処理
bool TurnSystem::EnemyMoveEnd() {
  for (auto&& enemy_parameter :
       stage_generator_.GetEnemyParameters(current_turn_ + 1)) {
    enemy_manager_.Add(enemy_parameter, physics_field_);
  }

  current_mode_ = Mode::PLAYER_MOVE_READY;
  return true;
}

//カメラの初期化
bool TurnSystem::InitCameras() {
  const math::IntVector2 screen_size =
      game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
  const float aspect_ratio = screen_size.x * 1.0f / screen_size.y;

  auto InitMainCamera = [&]() {
    const math::Quaternion camera_rotation =
        math::Quaternion::FromEular(math::util::DEG_2_RAD * 45.0f, 0.0f, 0.0f);
    auto main_camera = std::make_unique<camera::FollowCamera>();
    if (!main_camera->Init(L"MainCamera", &player_,
                           math::Vector3(0.0f, 30.0f, -30.0f), camera_rotation,
                           math::util::DEG_2_RAD * 50.0f, aspect_ratio,
                           math::Vector3::kUpVector, 0.1f, 300.0f)) {
      return false;
    }
    cameras_[camera_mode::Main] = std::move(main_camera);
    return true;
  };

  auto InitSub1Camera = [&]() {
    auto camera = std::make_unique<camera::PerspectiveCamera>();
    if (!camera->Init(L"Sub1Camera", math::Vector3(0.0f, 100.0f, 0.0f),
                      math::Quaternion::FromEular(90.0f * math::util::DEG_2_RAD,
                                                  0.0f, 0.0f),
                      50.0f * math::util::DEG_2_RAD, aspect_ratio,
                      math::Vector3::kForwardVector, 0.1f, 300.0f)) {
      return false;
    }

    cameras_[camera_mode::Sub1] = std::move(camera);
    return true;
  };

  if (!InitMainCamera()) return false;
  if (!InitSub1Camera()) return false;

  return true;
}

//描画
void TurnSystem::Draw() {
  cameras_[current_camera_]->RenderStart();
  player_.Draw();
  for (auto&& desk : desks_) {
    desk.Draw();
  }
  enemy_manager_.Draw();
  for (auto&& obs : obstacles_) {
    obs.Draw();
  }
  ui_board_.Draw();
}

//デバッグ描画
void TurnSystem::DebugDraw() {
  // auto& command_list = game::GameDevice::GetInstance()
  //                         ->GetDevice()
  //                         .GetCurrentFrameResource()
  //                         ->GetCommandList();

  // player_.GetCollisionRef().DebugDraw(command_list);
  // for (auto&& desk : desks_) {
  //  desk.GetCollisionRef().DebugDraw(command_list);
  //}
  // enemy_manager_.DebugDraw(command_list);
  // search_manager_.DebugDraw(command_list);
  // for (auto&& obs : obstacles_) {
  //  obs.GetCollisionRef().DebugDraw(command_list);
  //}
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