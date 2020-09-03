#include "src/system/turn_system.h"

#include "src/ui/number.h"
#include "src/ui/quarter_gauge.h"
#include "src/util/resource/resource_names.h"

namespace {
legend::i32 CalcPlayerStrengthToPrintNumber(
    const legend::player::Player& player) {
  const float str = player.GetStrength();
  return static_cast<legend::i32>(str * 100);
}

struct Camera {
  legend::math::Vector3 camera_position;
};
legend::directx::buffer::ConstantBuffer<Camera> camera_cb_;

struct Light {
  legend::math::Vector3 position;
};
struct LightCBStruct {
  legend::math::Matrix4x4 view;
  legend::math::Matrix4x4 proj;
};
legend::directx::buffer::ConstantBuffer<LightCBStruct> light_cb_;
}  // namespace

namespace legend {
namespace system {
namespace audio_name = util::resource::resource_names::audio;

//コンストラクタ
TurnSystem::TurnSystem() : current_turn_(0) {}

//デストラクタ
TurnSystem::~TurnSystem() { /*static_objects_.clear();*/
}

//初期化
bool TurnSystem::Init(const std::string& stage_name) {
  //アクター管理クラスの初期化
  actor_manager_.Init(stage_name, this);

  //ターン変更演出用クラス
  turn_change_.Init(this);

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

    //敵のターンでは表示しないUIをリストに積む
    if (w_name ==
            util::resource::resource_names::texture::UI_POWERGAUGE_FRAME ||
        w_name == util::resource::resource_names::texture::UI_POWERGAUGE) {
      no_render_if_enemy_turn_uis_.emplace_back(comp);
    }
  }
  MY_ASSERTION(
      gauges_.size() == gauge_id::MAX,
      L"main_ui.txtのUI定義が不正です。ゲージUIの個数が定義と違います。");
  MY_ASSERTION(
      numbers_.size() == number_id::MAX,
      L"main_ui.txtのUI定義が不正です。数値UIの個数が定義と違います。");

  fade_.Init(util::resource::resource_names::texture::FADE_IMAGE);
  fade_.StartFadeIn(1.0f);
  is_scene_all_end_ = false;
  is_scene_end_fade_start_ = false;

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  {
    const std::vector<directx::Sprite> vertices = {
        {{-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{1.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
        {{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
    };
    const u32 size = static_cast<u32>(vertices.size());
    vertex_buffer_.Init(device, sizeof(directx::Sprite), size, L"VB");
    vertex_buffer_.WriteBufferResource(vertices.data());
  }
  {
    const std::vector<u16> indices = {0, 1, 2, 0, 2, 3};
    const u32 size = static_cast<u32>(indices.size());
    index_buffer_.Init(device, sizeof(u16), size,
                       directx::PrimitiveTopology::TRIANGLE_LIST, L"IB");
    index_buffer_.WriteBufferResource(indices.data());
  }

  camera_cb_.Init(
      device,
      device.GetLocalHandle(
          directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY),
      L"CB");

  light_cb_.Init(
      device,
      device.GetLocalHandle(
          directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY),
      L"L_CB");

  current_turn_ = 0;

  return true;
}

bool TurnSystem::Update() {
  if (ImGui::Begin("TurnSystem")) {
    ImGui::Text("CurrntTurn : %d", current_turn_);
    if (ImGui::Button("AddTurn")) {
      // AddCurrentTurn();
      current_mode_ = Mode::ENEMY_MOVE_END;
    }
  }

  ImGui::End();

  actor_manager_.Update();

  const std::unordered_map<Mode, std::function<bool()>> switcher = {
      {Mode::PLAYER_MOVE_READY, [&]() { return PlayerMoveReady(); }},
      {Mode::PLAYER_MOVING, [&]() { return PlayerMoving(); }},
      {Mode::PLAYER_MOVE_END, [&]() { return WaitEnemyMoveStart(); }},
      {Mode::PLAYER_SKILL_AFTER_MOVED,
       [&]() { return PlayerSkillAfterMoved(); }},
      {Mode::ENEMY_MOVING, [&]() { return EnemyMove(); }},
      {Mode::ENEMY_MOVE_END, [&]() { return EnemyMoveEnd(); }},
      {Mode::ENEMY_PRODUCTION, [&]() { return EnemyMoveProducing(); }},
      {Mode::BOSS_PRODUCTION, [&]() { return BossMoveProducing(); }},
      {Mode::PLAYER_ADD_SKILL, [&]() { return AddSkill(); }},
      {Mode::TURN_CHANGE, [&]() { return turn_change_.Update(); }}};
  if (!switcher.at(current_mode_)()) {
    return false;
  }

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
      ImGui::SliderFloat2(ss.str().c_str(), field, -400.0f, 2000.0f);
      components_[i]->SetPosition(math::Vector2(field[0], field[1]));

      float z = components_[i]->GetZOrder();
      ss.str("");
      ss.clear(std::stringstream::goodbit);
      ss << "Z_Order:" << i;
      ImGui::SliderFloat(ss.str().c_str(), &z, 0.0f, 1.0f);
      components_[i]->SetZOrder(z);
    }

    if (ImGui::Button("Apply")) {
      std::ofstream ofs(
          std::filesystem::path("assets") / "parameters" / "main_ui.txt",
          std::ios::out);
      ofs.clear();
      const u32 size = static_cast<u32>(components_.size());
      for (u32 i = 0; i < size; i++) {
        input_lines_[i][ui_format::X] =
            std::to_string(components_[i]->GetPosition().x);
        input_lines_[i][ui_format::Y] =
            std::to_string(components_[i]->GetPosition().y);
        input_lines_[i][ui_format::Z] =
            std::to_string(components_[i]->GetZOrder());
        for (auto&& s : input_lines_[i]) {
          ofs << s << ",";
        }
        ofs << "\n";
      }
      ofs.flush();
    }
  }
  ImGui::End();

  {
    //プレイヤーの強化状態をUI数値に変換する
    const i32 str =
        CalcPlayerStrengthToPrintNumber(*actor_manager_.GetPlayer());
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

  gauges_[gauge_id::PLAYER_CHARGE_POWER]->SetValue(
      actor_manager_.GetPlayer()->GetImpulse());

  fade_.Update();
  if (is_scene_end_fade_start_) {
    if (fade_.IsEnd()) {
      is_scene_all_end_ = true;
    }
  } else {
    if (actor_manager_.GetPlayer()->GetPlayerDeathFlag()) {
      is_scene_end_fade_start_ = true;
      fade_.StartFadeOut(1.0f);
    }
    if (actor_manager_.IsGameClear()) {
      is_scene_end_fade_start_ = true;
      fade_.StartFadeOut(1.0f);
    }
  }

  return true;
}

//プレイヤーの移動準備
bool TurnSystem::PlayerMoveReady() {
  auto& input = game::GameDevice::GetInstance()->GetInput();
  auto& camera_manager = actor_manager_.GetCameraManager();
  if (input.GetCommand(input::input_code::CAMERA_CHANGE) &&
      !actor_manager_.GetPlayer()->GetSkillSelect()) {
    auto SwitchCamera = [&]() {
      if (camera_manager.GetCameraMode() ==
          camera::camera_mode::PLAYER_LOOKAT) {
        camera_manager.SetCameraMode(camera::camera_mode::BIRDS_EYE_VIEW);
      } else {
        camera_manager.SetCameraMode(camera::camera_mode::PLAYER_LOOKAT);
      }
    };
    SwitchCamera();
  }

  //メインカメラの状態じゃないと移動できないようにする
  if (camera_manager.GetCameraMode() == camera::camera_mode::PLAYER_LOOKAT) {
    //プレイヤーの速度更新は入力を受け取って処理する
    if (!actor_manager_.GetPlayer()->GetSkillSelect())
      actor_manager_.GetPlayer()->CheckImpulse();
    else
      actor_manager_.GetPlayer()->SkillUpdate();
  } else {
    //それ以外の時はプレイヤーの移動入力状態を無力化する必要がある
  }

  return true;
}

//プレイヤーの移動中処理
bool TurnSystem::PlayerMoving() { return true; }

bool TurnSystem::WaitEnemyMoveStart() {
  if (actor_manager_.GetEnemiesSize() == 0) {
    before_mode_ = current_mode_;
    current_mode_ = Mode::PLAYER_SKILL_AFTER_MOVED;
    return true;
  }
  if (!actor_manager_.IsAllEnemeyStop()) {
    return true;
  }
  before_mode_ = current_mode_;
  current_mode_ = Mode::PLAYER_SKILL_AFTER_MOVED;
  return true;
}

bool TurnSystem::PlayerSkillAfterMoved() {
  if (actor_manager_.GetPlayer()->SkillUpdateTurnEnd()) {
    return true;
  }

  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();
  audio.Start(util::resource::resource_names::audio::PLAYER_TURN_END, 1.0f);

  before_mode_ = current_mode_;
  current_mode_ = Mode::PLAYER_ADD_SKILL;
  return true;
}

bool TurnSystem::AddSkill() {
  actor_manager_.GetPlayer()->EquipmentUpdate();
  return true;
}

//敵の移動処理
bool TurnSystem::EnemyMove() {
  actor_manager_.EnemyManagerUpdate();
  if (actor_manager_.GetEnemyManager()->LastEnemyMoveEnd()) {
    before_mode_ = current_mode_;
    current_mode_ = Mode::ENEMY_MOVE_END;
    actor_manager_.GetEnemyManager()->ResetEnemyMove();
  }
  return true;
}

//敵の移動終了時処理
bool TurnSystem::EnemyMoveEnd() {
  if (actor_manager_.GetPlayer()->SkillUpdateTurnEnd()) {
    return true;
  }

  //ターンの更新
  AddCurrentTurn();

  //敵の生成
  if (!actor_manager_.GenerateActors(current_turn_)) {
    return false;
  }

  //敵が存在しているか
  if (actor_manager_.GetEnemiesSize() > 0 || actor_manager_.IsBossGenerated()) {
    before_mode_ = current_mode_;
    current_mode_ = Mode::PLAYER_ADD_SKILL;
    return true;
  }

  before_mode_ = current_mode_;
  current_mode_ = Mode::PLAYER_ADD_SKILL;
  return true;
}

bool TurnSystem::EnemyMoveProducing() {
  if (actor_manager_.IsAllEnemeyStop()) {
    ToPlayerTurn();
  }

  return true;
}

bool TurnSystem::BossMoveProducing() {
  ToPlayerTurn();

  return true;
}

bool TurnSystem::ToPlayerTurn() {
  current_mode_ = Mode::TURN_CHANGE;
  return turn_change_.ChangeStart(Mode::PLAYER_MOVE_READY);
}

bool TurnSystem::ToEnemyTurn() {
  current_mode_ = Mode::TURN_CHANGE;
  return turn_change_.ChangeStart(Mode::ENEMY_MOVING);
}

//描画
void TurnSystem::Draw() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
  auto& render_resource_manager = device.GetRenderResourceManager();

  //シャドウマップ用描画
  render_resource_manager.SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::NONE, false,
      directx::render_target::DepthStencilTargetID::SHADOW_MAP, true);

  actor_manager_.DrawDifferedRenderingObject(command_list);

  render_resource_manager.SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::BACK_BUFFER, true,
      directx::render_target::DepthStencilTargetID::NONE, true);

  auto& resource = game::GameDevice::GetInstance()->GetResource();
  resource.GetPipeline()
      .Get(util::resource::resource_names::pipeline::DIFFERED_RENDERING)
      ->SetCommandList(command_list);
  render_resource_manager.UseAsSRV(
      device, command_list,
      directx::render_target::RenderTargetID::DIFFERED_RENDERING_PRE, 0);
  render_resource_manager.UseAsSRV(
      device, command_list,
      directx::render_target::RenderTargetID::DIFFERED_RENDERING_PRE, 1);
  render_resource_manager.UseAsSRV(
      device, command_list,
      directx::render_target::RenderTargetID::DIFFERED_RENDERING_PRE, 2);
  render_resource_manager.UseAsSRV(
      device, command_list,
      directx::render_target::DepthStencilTargetID::SHADOW_MAP, 3);

  // camera_cb_.GetStagingRef().camera_position =
  //    camera_manager_.GetCurrentCamera()->GetPosition();
  // camera_cb_.UpdateStaging();
  // camera_cb_.RegisterHandle(device, 6);
  light_cb_.RegisterHandle(device, 7);

  device.GetHeapManager().SetHeapTableToGraphicsCommandList(device,
                                                            command_list);

  vertex_buffer_.SetGraphicsCommandList(command_list);
  index_buffer_.SetGraphicsCommandList(command_list);
  index_buffer_.Draw(command_list);

  game::GameDevice::GetInstance()->GetParticleCommandList().RenderParticle(
      command_list);

  render_resource_manager.SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::BACK_BUFFER, false,
      directx::render_target::DepthStencilTargetID::DEPTH_ONLY, true);

  actor_manager_.DrawAlphaObject(command_list);
  actor_manager_.Draw2D(command_list);

  const bool is_player_turn = current_mode_ == Mode::PLAYER_MOVING ||
                              current_mode_ == Mode::PLAYER_MOVE_READY;
  for (auto&& ui : no_render_if_enemy_turn_uis_) {
    ui->SetEnable(is_player_turn);
  }

  ui_board_.Draw();
  fade_.Draw();
  turn_change_.Draw();

  //スプライトは最後に描画リストにあるものをまとめて描画する
  game::GameDevice::GetInstance()->GetSpriteRenderer().DrawItems(command_list);

  actor_manager_.DrawEnd();
}

//デバッグ描画
void TurnSystem::DebugDraw() { actor_manager_.DebugDraw(); }

bool legend::system::TurnSystem::IsGameEnd() const { return is_scene_all_end_; }

system::GameDataStorage::ResultData legend::system::TurnSystem::GetResult()
    const {
  const system::GameDataStorage::GameEndType end_type = [&]() {
    if (actor_manager_.IsGameClear())
      return system::GameDataStorage::GameEndType::BOSS_KILLED;
    else
      return system::GameDataStorage::GameEndType::PLAYER_DEAD;
  }();
  //プレイヤーが死亡したか、敵のボスが死亡したらその情報を返す
  return system::GameDataStorage::ResultData{
      end_type, CalcPlayerStrengthToPrintNumber(*actor_manager_.GetPlayer()),
      current_turn_ + 1};
}

void TurnSystem::SetTurnMode(Mode mode) {
  before_mode_ = current_mode_;
  current_mode_ = mode;
}

Mode TurnSystem::GetCurrentMode() { return current_mode_; }

Mode TurnSystem::GetBeforeMode() { return before_mode_; }
//ターン数の増加
void TurnSystem::AddCurrentTurn() { current_turn_++; }

//現在のターン数を取得
i32 TurnSystem::GetCurrentTurn() { return current_turn_; }

}  // namespace system
}  // namespace legend