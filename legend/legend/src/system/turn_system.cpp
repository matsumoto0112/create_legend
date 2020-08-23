#include "src/system/turn_system.h"

#include "src/camera/lookat_target_camera.h"
#include "src/ui/number.h"
#include "src/ui/quarter_gauge.h"
#include "src/util/resource/resource_names.h"

namespace {
legend::i32 CalcPlayerStrengthToPrintNumber(
    const legend::player::Player& player) {
  const float str = player.GetStrength();
  return static_cast<legend::i32>(str * 100);
}
}  // namespace

namespace legend {
namespace system {
namespace audio_name = util::resource::resource_names::audio;

//コンストラクタ
TurnSystem::TurnSystem()
    : current_turn_(0), current_camera_(camera_mode::Sub1) {}

//デストラクタ
TurnSystem::~TurnSystem() { static_objects_.clear(); }

//初期化
bool TurnSystem::Init(const std::string& stage_name) {
  //ステージデータの拡張子は.txt
  auto stage_path = util::Path::GetInstance()->exe() / "assets" / "stage" /
                    (stage_name + ".txt");

  if (!physics_field_.Init()) {
    return false;
  }

  directx::device::CommandList command_list;
  if (!command_list.Init(
          game::GameDevice::GetInstance()->GetDevice(),
          D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT)) {
    return false;
  }

  //ステージデータの読み込み
  {
    player::Player::InitializeParameter player;
    std::vector<object::Desk::InitializeParameter> desks;
    std::vector<object::Obstacle::InitializeParameter> obstacles;
    std::vector<object::GraffitiInitializeParameter> graffities;
    std::vector<skill::SkillItemBox::InitializeParameter> item_boxes;
    if (!stage_generator_.LoadStage(stage_path, stage_name, player, desks,
                                    obstacles, graffities, item_boxes)) {
      return false;
    }

    player_ = std::make_unique<player::Player>();
    if (!player_->Init(this, player)) {
      return false;
    }
    for (auto&& param : desks) {
      auto obj = std::make_unique<object::Desk>();
      if (!obj->Init(this, param)) {
        return false;
      }
      static_objects_.emplace_back(std::move(obj));
    }

    for (auto&& param : obstacles) {
      auto obj = std::make_unique<object::Obstacle>();
      if (!obj->Init(this, param)) {
        return false;
      }
      static_objects_.emplace_back(std::move(obj));
    }

    for (auto&& param : graffities) {
      auto graf = std::make_unique<object::Graffiti>();
      if (!graf->Init(this, param, command_list)) {
        return false;
      }
      graffities_.emplace_back(std::move(graf));
    }

    for (auto&& param : item_boxes) {
      auto obj = std::make_unique<skill::SkillItemBox>();
      std::shared_ptr<skill::Skill> skill =
          std::make_shared<skill::SkillPencil>();
      if (!obj->Init(this, param, skill)) {
        return false;
      }
      item_boxes_.emplace_back(std::move(obj));
    }
  }

  if (!enemy_manager_.Initilaize(this)) {
    return false;
  }

  for (auto&& enemy_parameter :
       stage_generator_.GetEnemyParameters(current_turn_)) {
    enemy_manager_.Add(enemy_parameter);
  }

  if (!InitCameras()) {
    return false;
  }

  command_list.Close();
  game::GameDevice::GetInstance()->GetDevice().ExecuteCommandList(
      {command_list});
  game::GameDevice::GetInstance()->GetDevice().WaitExecute();

  {
    search_manager_.Initialize(this);

    //探索データの拡張子は.txt
    auto search_path = util::Path::GetInstance()->exe() / "assets" / "stage" /
                       (stage_name + "_searchData" + ".txt");
    search_manager_.Make(search_path);
  }

  // UI情報を取得
  std::vector<u8> data =
      game::GameDevice::GetInstance()->GetResource().GetArchiveLoader().Load(
          std::filesystem::path("parameters") / "main_ui.txt");

  //文字列として解釈し、各行に分割するためstreamを使用する
  std::string str(data.begin(), data.end());
  std::stringstream ss(str);
  std::string line;

  while (std::getline(ss, line, '\n')) {
    //.txtをバイナリモードで開いているため、改行文字が\r\nで表現されている
    //\nで分割するとき、最後の文字が\rになっていると余分なデータが含まれるためそれを回避する
    if (line.back() == '\r') {
      line = line.substr(0, line.size() - 1);
    }

    auto split = util::string_util::StringSplit(line, ',');
    MY_ASSERTION(split.size() == ui_format::MAX, L"フォーマットが不正です。");
    std::string name = split[ui_format::NAME];
    std::wstring w_name = util::string_util::String_2_WString(name + ".png");
    float x = std::stof(split[ui_format::X]);
    float y = std::stof(split[ui_format::Y]);
    float z = std::stof(split[ui_format::Z]);

    ui::UIComponent* comp = nullptr;
    if (split[ui_format::ID] == "0") {
      auto image = std::make_unique<ui::Image>();
      if (!image->Init(w_name)) {
        return false;
      }
      comp = ui_board_.AddComponent(std::move(image));
    } else if (split[ui_format::ID] == "1") {
      auto gauge = std::make_unique<ui::Gauge>();
      if (!gauge->Init(w_name)) {
        return false;
      }
      comp = ui_board_.AddComponent(std::move(gauge));
      gauges_.emplace_back(static_cast<ui::Gauge*>(comp));
    } else if (split[ui_format::ID] == "2") {
      auto gauge = std::make_unique<ui::QuarterGauge>();
      if (!gauge->Init(w_name)) {
        return false;
      }
      comp = ui_board_.AddComponent(std::move(gauge));
      gauges_.emplace_back(static_cast<ui::Gauge*>(comp));
    } else if (split[ui_format::ID] == "3") {
      auto num = std::make_unique<ui::Number>();
      if (!num->Init(w_name)) {
        return false;
      }
      comp = ui_board_.AddComponent(std::move(num));
      numbers_.emplace_back(static_cast<ui::Number*>(comp));
    }
    MY_ASSERTION(comp, L"不正なIDが入力されました");
    comp->SetPosition(math::Vector2(x, y));
    comp->SetZOrder(z);
    components_.emplace_back(comp);
    input_lines_.emplace_back(split);
  }
  MY_ASSERTION(
      gauges_.size() == gauge_id::MAX,
      L"main_ui.txtのUI定義が不正です。ゲージUIの個数が定義と違います。");
  MY_ASSERTION(
      numbers_.size() == number_id::MAX,
      L"main_ui.txtのUI定義が不正です。数値UIの個数が定義と違います。");

  return true;
}

bool TurnSystem::Update() {
  countdown_timer_.Update();
  player_->Update();
  UpdateCamera();

  for (auto&& graf : graffities_) {
    if (graf->GetIsHit()) {
      AddFragment(graf->InstanceFragment());
    }
    graf->Update();
  }
  for (auto&& fragment : fragments_) {
    fragment->Update();
  }
  for (auto&& item_box : item_boxes_) {
    item_box->Update();
  }
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

  if (ImGui::Begin("Camera")) {
    if (ImGui::Button("Main")) {
      current_camera_ = camera_mode::Main;
    }
    if (ImGui::Button("Sub1")) {
      current_camera_ = camera_mode::Sub1;
    }
  }
  ImGui::End();

  // if (ImGui::Begin("UI")) {
  //  const u32 size = static_cast<u32>(components_.size());
  //  for (u32 i = 0; i < size; i++) {
  //    std::stringstream ss;
  //    ss << "Image:" << i;
  //    ImGui::Text(ss.str().c_str());

  //    math::Vector2 pos = components_[i]->GetPosition();
  //    float field[2] = {pos.x, pos.y};
  //    ss.str("");
  //    ss.clear(std::stringstream::goodbit);
  //    ss << "Position:" << i;
  //    ImGui::SliderFloat2(ss.str().c_str(), field, -400.0f, 2000.0f);
  //    components_[i]->SetPosition(math::Vector2(field[0], field[1]));

  //    float z = components_[i]->GetZOrder();
  //    ss.str("");
  //    ss.clear(std::stringstream::goodbit);
  //    ss << "Z_Order:" << i;
  //    ImGui::SliderFloat(ss.str().c_str(), &z, 0.0f, 1.0f);
  //    components_[i]->SetZOrder(z);
  //  }

  //  if (ImGui::Button("Apply")) {
  //    std::ofstream ofs(
  //        std::filesystem::path("assets") / "parameters" / "main_ui.txt",
  //        std::ios::out);
  //    ofs.clear();
  //    const u32 size = static_cast<u32>(components_.size());
  //    for (u32 i = 0; i < size; i++) {
  //      input_lines_[i][ui_format::X] =
  //          std::to_string(components_[i]->GetPosition().x);
  //      input_lines_[i][ui_format::Y] =
  //          std::to_string(components_[i]->GetPosition().y);
  //      input_lines_[i][ui_format::Z] =
  //          std::to_string(components_[i]->GetZOrder());
  //      for (auto&& s : input_lines_[i]) {
  //        ofs << s << ",";
  //      }
  //      ofs << "\n";
  //    }
  //    ofs.flush();
  //  }

  //  static int power = 100;
  //  ImGui::SliderInt("PlayerPower", &power, 0, 300);
  //  gauges_[gauge_id::PLAYER_STRENGTHENED_STATE_0]->SetValue(
  //      math::util::Clamp(power * 0.01f, 0.0f, 1.0f));
  //  gauges_[gauge_id::PLAYER_STRENGTHENED_STATE_1]->SetValue(
  //      math::util::Clamp((power * 0.01f) - 1.0f, 0.0f, 1.0f));
  //  gauges_[gauge_id::PLAYER_STRENGTHENED_STATE_2]->SetValue(
  //      math::util::Clamp((power * 0.01f) - 2.0f, 0.0f, 1.0f));
  //  numbers_[number_id::DIGIT_3]->SetNumber(power / 100);
  //  numbers_[number_id::DIGIT_2]->SetNumber(power / 10 % 10);
  //  numbers_[number_id::DIGIT_1]->SetNumber(power % 10);
  //}
  // ImGui::End();

  {
    //プレイヤーの強化状態をUI数値に変換する
    const i32 str = CalcPlayerStrengthToPrintNumber(*player_);
    numbers_[number_id::DIGIT_3]->SetNumber(str / 100);
    numbers_[number_id::DIGIT_2]->SetNumber(str / 10 % 10);
    numbers_[number_id::DIGIT_1]->SetNumber(str % 10);
    gauges_[gauge_id::PLAYER_STRENGTHENED_STATE_0]->SetValue(
        math::util::Clamp(str * 0.01f, 0.0f, 1.0f));
    gauges_[gauge_id::PLAYER_STRENGTHENED_STATE_1]->SetValue(
        math::util::Clamp((str * 0.01f) - 1.0f, 0.0f, 1.0f));
    gauges_[gauge_id::PLAYER_STRENGTHENED_STATE_2]->SetValue(
        math::util::Clamp((str * 0.01f) - 2.0f, 0.0f, 1.0f));
  }

  gauges_[gauge_id::PLAYER_CHARGE_POWER]->SetValue(player_->GetImpulse());

  graffities_.erase(
      std::remove_if(graffities_.begin(), graffities_.end(),
                     [&](auto& it) {
                       return remove_graffiti_list_.find(it.get()) !=
                              remove_graffiti_list_.end();
                     }),
      graffities_.end());
  remove_graffiti_list_.clear();

  fragments_.erase(
      std::remove_if(fragments_.begin(), fragments_.end(),
                     [&](auto& it) {
                       return remove_fragment_list_.find(it.get()) !=
                              remove_fragment_list_.end();
                     }),
      fragments_.end());
  remove_fragment_list_.clear();

  item_boxes_.erase(
      std::remove_if(item_boxes_.begin(), item_boxes_.end(),
                     [&](auto& it) {
                       return remove_item_box_list_.find(it.get()) !=
                              remove_item_box_list_.end();
                     }),
      item_boxes_.end());
  remove_item_box_list_.clear();

  physics_field_.Update();
  return true;
}

//プレイヤーの移動準備
bool TurnSystem::PlayerMoveReady() {
  if (player_->GetSkillSelect()) {
    return true;
  }

  auto& input = game::GameDevice::GetInstance()->GetInput();
  if (input.GetCommand(input::input_code::CAMERA_CHANGE)) {
    if (current_camera_ == camera_mode::Main) {
      current_camera_ = camera_mode::Sub1;
    } else if (current_camera_ == camera_mode::Sub1) {
      current_camera_ = camera_mode::Main;
    }
  }

  //メインカメラの状態じゃないと移動できないようにする
  if (current_camera_ == camera_mode::Main) {
    //プレイヤーの速度更新は入力を受け取って処理する
    player_->CheckImpulse();
  } else {
    //それ以外の時はプレイヤーの移動入力状態を無力化する必要がある
  }

  return true;
}

//プレイヤーの移動中処理
bool TurnSystem::PlayerMoving() { return true; }

bool TurnSystem::PlayerSkillAfterModed() {
  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();
  audio.Start(util::resource::resource_names::audio::PLAYER_TURN_END, 1.0f);

  current_mode_ = Mode::ENEMY_MOVING;
  return true;
}

//敵の移動処理
bool TurnSystem::EnemyMove() {
  MY_LOG(L"EnemyMove");
  enemy_manager_.Update(&search_manager_);
  enemy_manager_.SetPlayer(player_->GetCollider());
  if (enemy_manager_.GetEnemiesSize() == 0 ||
      enemy_manager_.LastEnemyMoveEnd()) {
    current_mode_ = Mode::ENEMY_MOVE_END;
    enemy_manager_.ResetEnemyMove();
    // AddCurrentTurn();
  }
  return true;
}

//敵の移動終了時処理
bool TurnSystem::EnemyMoveEnd() {
  AddCurrentTurn();
  for (auto&& enemy_parameter :
       stage_generator_.GetEnemyParameters(current_turn_)) {
    enemy_manager_.Add(enemy_parameter);
  }
  for (auto&& boss_parameter :
       stage_generator_.GetBossParameters(current_turn_)) {
    enemy_manager_.Add(boss_parameter);
  }

  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();
  audio.Start(audio_name::ENEMY_TURN_END, 1.0f);

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
    auto main_camera = std::make_unique<camera::LookAtTargetCamera>();
    if (!main_camera->Init(L"MainCamera", player_.get(), 20.0f, 30.0f,
                           math::util::DEG_2_RAD * 50.0f, aspect_ratio,
                           math::Vector3::kUpVector, 0.1f, 300.0f)) {
      return false;
    }

    player_follow_lookat_camera_ = main_camera.get();
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

void legend::system::TurnSystem::AddFragment(
    std::unique_ptr<object::Fragment> fragment) {
  fragments_.emplace_back(std::move(fragment));
}

void TurnSystem::UpdateCamera() {
  auto& input = game::GameDevice::GetInstance()->GetInput();
  float theta = player_follow_lookat_camera_->GetTheta();

  const float delta_time =
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();

  //右コントローラの入力を取得する
  const math::Vector2 right_input = input.GetGamepad()->GetStickRight();
  constexpr float POWER = 1.0f;
  theta += right_input.x * POWER * delta_time;
  player_follow_lookat_camera_->SetTheta(theta);
}

float legend::system::TurnSystem::GetMainCameraThetaAngle() const {
  return player_follow_lookat_camera_->GetTheta();
}

void legend::system::TurnSystem::RemoveActor(actor::Actor* actor) {
  if (auto g = dynamic_cast<object::Graffiti*>(actor); g) {
    remove_graffiti_list_.emplace(g);
  }
  if (auto fragment = dynamic_cast<object::Fragment*>(actor); fragment) {
    remove_fragment_list_.emplace(fragment);
  }
  if (auto item_box = dynamic_cast<skill::SkillItemBox*>(actor); item_box) {
    remove_item_box_list_.emplace(item_box);
  }
}

void legend::system::TurnSystem::RemoveCollider(
    std::shared_ptr<bullet::Collider> collider) {
  physics_field_.RemoveCollision(collider);
}

void TurnSystem::AddCollider(std::shared_ptr<bullet::Collider> collider) {
  physics_field_.AddCollision(collider);
}

btCollisionWorld::AllHitsRayResultCallback legend::system::TurnSystem::RayCast(
    const math::Vector3& start, const math::Vector3& end) const {
  return physics_field_.RayCast(start, end);
}

//描画
void TurnSystem::Draw() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();

  cameras_[current_camera_]->RenderStart();
  player_->Draw();
  for (auto&& obj : static_objects_) {
    obj->Draw();
  }
  enemy_manager_.Draw();
  for (auto&& graffiti : graffities_) {
    graffiti->Draw(command_list);
  }
  for (auto&& fragment : fragments_) {
    fragment->Draw();
  }
  for (auto&& item_box : item_boxes_) {
    item_box->Draw();
  }

  ui_board_.Draw();

  //スプライトは最後に描画リストにあるものをまとめて描画する
  game::GameDevice::GetInstance()->GetSpriteRenderer().DrawItems(command_list);
}

//デバッグ描画
void TurnSystem::DebugDraw() {
  cameras_[current_camera_]->RenderStart();
  search_manager_.DebugDraw(&physics_field_);
  physics_field_.DebugDraw(cameras_[current_camera_].get());
}

bool legend::system::TurnSystem::IsGameEnd() const {
  //プレイヤーが死亡したらtrueを返す

  //敵のボスが死亡し、演出まで終了したらtrueを返す
  if (enemy_manager_.IsGameClear()) {
    return true;
  }
  //それ以外の状況ではfalseを返す
  return false;
}

system::GameDataStorage::GameData legend::system::TurnSystem::GetResult()
    const {
  //プレイヤーが死亡したか、敵のボスが死亡したらその情報を返す
  return system::GameDataStorage::GameData{
      system::GameDataStorage::GameEndType::PLAYER_DEAD,
      CalcPlayerStrengthToPrintNumber(*player_), current_turn_};
}

//ターン数の増加
void TurnSystem::AddCurrentTurn() { current_turn_++; }

//現在のターン数を取得
i32 TurnSystem::GetCurrentTurn() { return current_turn_; }

//プレイヤーの移動開始時処理
void TurnSystem::PlayerMoveStartEvent() {
  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();
  current_mode_ = Mode::PLAYER_MOVING;
  current_camera_ = camera_mode::Sub1;
}

//プレイヤーの移動終了時処理
void TurnSystem::PlayerMoveEndEvent() {
  // 0.1秒後にモードを切り替える
  countdown_timer_.Init(
      0.1f, [&]() { current_mode_ = Mode::PLAYER_SKILL_AFTER_MOVED; });
}

//プレイヤーのスキル発動時処理
void TurnSystem::PlayerSkillActivate() {}

//プレイヤーのスキル発動終了時処理
void TurnSystem::PlayerSkillDeactivate() {}

player::Player* TurnSystem::GetPlayer() { return player_.get(); }

std::vector<enemy::Enemy*> TurnSystem::GetEnemies() {
  return enemy_manager_.GetEnemyPointers();
}

}  // namespace system
}  // namespace legend