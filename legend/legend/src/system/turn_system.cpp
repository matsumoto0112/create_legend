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

//�R���X�g���N�^
TurnSystem::TurnSystem()
    : current_turn_(0), current_camera_(camera_mode::Sub1) {}

//�f�X�g���N�^
TurnSystem::~TurnSystem() { /*static_objects_.clear();*/
}

//������
bool TurnSystem::Init(const std::string& stage_name) {
  //�A�N�^�[�Ǘ��N���X�̏�����
  actor_manager_.Init(stage_name, this);

  if (!InitCameras()) {
    return false;
  }

  // UI�����擾
  std::vector<u8> data =
      game::GameDevice::GetInstance()->GetResource().GetArchiveLoader().Load(
          std::filesystem::path("parameters") / "main_ui.txt");

  //������Ƃ��ĉ��߂��A�e�s�ɕ������邽��stream���g�p����
  std::string str(data.begin(), data.end());
  std::stringstream ss(str);
  std::string line;

  while (std::getline(ss, line, '\n')) {
    //.txt���o�C�i�����[�h�ŊJ���Ă��邽�߁A���s������\r\n�ŕ\������Ă���
    //\n�ŕ�������Ƃ��A�Ō�̕�����\r�ɂȂ��Ă���Ɨ]���ȃf�[�^���܂܂�邽�߂�����������
    if (line.back() == '\r') {
      line = line.substr(0, line.size() - 1);
    }

    auto split = util::string_util::StringSplit(line, ',');
    MY_ASSERTION(split.size() == ui_format::MAX, L"�t�H�[�}�b�g���s���ł��B");
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
    MY_ASSERTION(comp, L"�s����ID�����͂���܂���");
    comp->SetPosition(math::Vector2(x, y));
    comp->SetZOrder(z);
    components_.emplace_back(comp);
    input_lines_.emplace_back(split);

    //�G�̃^�[���ł͕\�����Ȃ�UI�����X�g�ɐς�
    if (w_name ==
            util::resource::resource_names::texture::UI_POWERGAUGE_FRAME ||
        w_name == util::resource::resource_names::texture::UI_POWERGAUGE) {
      no_render_if_enemy_turn_uis_.emplace_back(comp);
    }
  }
  MY_ASSERTION(
      gauges_.size() == gauge_id::MAX,
      L"main_ui.txt��UI��`���s���ł��B�Q�[�WUI�̌�����`�ƈႢ�܂��B");
  MY_ASSERTION(
      numbers_.size() == number_id::MAX,
      L"main_ui.txt��UI��`���s���ł��B���lUI�̌�����`�ƈႢ�܂��B");

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

  actor_manager_.Update();

  UpdateCamera();
  const std::unordered_map<Mode, std::function<bool()>> switcher = {
      {Mode::PLAYER_MOVE_READY, [&]() { return PlayerMoveReady(); }},
      {Mode::PLAYER_MOVING, [&]() { return PlayerMoving(); }},
      {Mode::PLAYER_MOVE_END, [&]() { return WaitEnemyMoveStart(); }},
      {Mode::PLAYER_SKILL_AFTER_MOVED,
       [&]() { return PlayerSkillAfterMoved(); }},
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
    //�v���C���[�̋�����Ԃ�UI���l�ɕϊ�����
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

//�v���C���[�̈ړ�����
bool TurnSystem::PlayerMoveReady() {
  auto& input = game::GameDevice::GetInstance()->GetInput();
  if (input.GetCommand(input::input_code::CAMERA_CHANGE) &&
      !actor_manager_.GetPlayer()->GetSkillSelect()) {
    if (current_camera_ == camera_mode::Main) {
      current_camera_ = camera_mode::Sub1;
    } else if (current_camera_ == camera_mode::Sub1) {
      current_camera_ = camera_mode::Main;
    }
  }

  //���C���J�����̏�Ԃ���Ȃ��ƈړ��ł��Ȃ��悤�ɂ���
  if (current_camera_ == camera_mode::Main) {
    //�v���C���[�̑��x�X�V�͓��͂��󂯎���ď�������
    if (!actor_manager_.GetPlayer()->GetSkillSelect())
      actor_manager_.GetPlayer()->CheckImpulse();
    else
      actor_manager_.GetPlayer()->SkillUpdate();
  } else {
    //����ȊO�̎��̓v���C���[�̈ړ����͏�Ԃ𖳗͉�����K�v������
  }

  return true;
}

//�v���C���[�̈ړ�������
bool TurnSystem::PlayerMoving() { return true; }

bool TurnSystem::WaitEnemyMoveStart() {
  if (actor_manager_.GetEnemiesSize() == 0) {
    current_mode_ = Mode::PLAYER_SKILL_AFTER_MOVED;
    return true;
  }
  if (!actor_manager_.IsAllEnemeyStop()) {
    return true;
  }
  current_mode_ = Mode::PLAYER_SKILL_AFTER_MOVED;
  return true;
}

bool TurnSystem::PlayerSkillAfterMoved() {
  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();
  audio.Start(util::resource::resource_names::audio::PLAYER_TURN_END, 1.0f);

  current_mode_ = Mode::ENEMY_MOVING;
  return true;
}

//�G�̈ړ�����
bool TurnSystem::EnemyMove() {
  actor_manager_.EnemyManagerUpdate();
  if (actor_manager_.GetEnemyManager()->LastEnemyMoveEnd()) {
    current_mode_ = Mode::ENEMY_MOVE_END;
    actor_manager_.GetEnemyManager()->ResetEnemyMove();
  }
  return true;
}

//�G�̈ړ��I��������
bool TurnSystem::EnemyMoveEnd() {
  AddCurrentTurn();

  if (!actor_manager_.GenerateActors(current_turn_)) {
    return false;
  }

  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();
  audio.Start(audio_name::ENEMY_TURN_END, 1.0f);

  current_mode_ = Mode::PLAYER_MOVE_READY;
  return true;
}

//�J�����̏�����
bool TurnSystem::InitCameras() {
  const math::IntVector2 screen_size =
      game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
  const float aspect_ratio = screen_size.x * 1.0f / screen_size.y;

  auto InitMainCamera = [&]() {
    const math::Quaternion camera_rotation =
        math::Quaternion::FromEular(math::util::DEG_2_RAD * 45.0f, 0.0f, 0.0f);
    auto main_camera = std::make_unique<camera::LookAtTargetCamera>();
    if (!main_camera->Init(L"MainCamera", actor_manager_.GetPlayer(), 20.0f,
                           30.0f, math::util::DEG_2_RAD * 50.0f, aspect_ratio,
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
  current_camera_ = camera_mode::Main;
  return true;
}

void TurnSystem::UpdateCamera() {
  //���C���J�����̉�]����
  auto& input = game::GameDevice::GetInstance()->GetInput();
  float theta = player_follow_lookat_camera_->GetTheta();

  const float delta_time =
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();

  //�E�R���g���[���̓��͂��擾����
  const math::Vector2 right_input = input.GetGamepad()->GetStickRight();
  constexpr float POWER = 1.0f;
  theta += right_input.x * POWER * delta_time;
  player_follow_lookat_camera_->SetTheta(theta);

  //�T�u�J����1�̓v���C���[�̓�����ړ�����
  if (auto camera = dynamic_cast<camera::PerspectiveCamera*>(
          cameras_[camera_mode::Sub1].get());
      camera) {
    const math::Vector3 player_position =
        actor_manager_.GetPlayer()->GetPosition();
    camera->SetPosition(math::Vector3(
        player_position.x, camera->GetPosition().y, player_position.z));
  }
}
//�`��
void TurnSystem::Draw() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
  auto& render_resource_manager = device.GetRenderResourceManager();

  //�V���h�E�}�b�v�p�`��
  render_resource_manager.SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::NONE, false,
      directx::render_target::DepthStencilTargetID::SHADOW_MAP, true);

  actor_manager_.DrawDifferedRenderingObject(cameras_[current_camera_].get(), command_list);


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

  camera_cb_.GetStagingRef().camera_position =
      player_follow_lookat_camera_->GetPosition();
  camera_cb_.UpdateStaging();
  camera_cb_.RegisterHandle(device, 6);
  light_cb_.RegisterHandle(device, 7);

  device.GetHeapManager().SetHeapTableToGraphicsCommandList(device,
                                                            command_list);

  vertex_buffer_.SetGraphicsCommandList(command_list);
  index_buffer_.SetGraphicsCommandList(command_list);
  index_buffer_.Draw(command_list);

  render_resource_manager.SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::BACK_BUFFER, false,
      directx::render_target::DepthStencilTargetID::DEPTH_ONLY, false);

  actor_manager_.DrawAlphaObject(command_list);
  actor_manager_.Draw2D(command_list);

  const bool is_player_turn = current_mode_ == Mode::PLAYER_MOVING ||
                              current_mode_ == Mode::PLAYER_MOVE_READY;
  for (auto&& ui : no_render_if_enemy_turn_uis_) {
    ui->SetEnable(is_player_turn);
  }

  ui_board_.Draw();
  fade_.Draw();

  //�X�v���C�g�͍Ō�ɕ`�惊�X�g�ɂ�����̂��܂Ƃ߂ĕ`�悷��
  game::GameDevice::GetInstance()->GetSpriteRenderer().DrawItems(command_list);

  actor_manager_.DrawEnd();
}

//�f�o�b�O�`��
void TurnSystem::DebugDraw() {
  cameras_[current_camera_]->RenderStart();
  actor_manager_.DebugDraw(cameras_[current_camera_].get());
}

bool legend::system::TurnSystem::IsGameEnd() const { return is_scene_all_end_; }

system::GameDataStorage::GameData legend::system::TurnSystem::GetResult()
    const {
  const system::GameDataStorage::GameEndType end_type = [&]() {
    if (actor_manager_.IsGameClear())
      return system::GameDataStorage::GameEndType::BOSS_KILLED;
    else
      return system::GameDataStorage::GameEndType::PLAYER_DEAD;
  }();
  //�v���C���[�����S�������A�G�̃{�X�����S�����炻�̏���Ԃ�
  return system::GameDataStorage::GameData{
      end_type, CalcPlayerStrengthToPrintNumber(*actor_manager_.GetPlayer()), current_turn_ + 1};
}
void TurnSystem::SetTurnMode(Mode mode)
{
    current_mode_ = mode;
}
void TurnSystem::SetCameraMode(camera_mode::Enum mode)
{
    current_camera_ = mode;
}
camera::LookAtTargetCamera* TurnSystem::GetPlayerFollowLookatCamera()
{
    return player_follow_lookat_camera_;
}
//�^�[�����̑���
void TurnSystem::AddCurrentTurn() { current_turn_++; }

//���݂̃^�[�������擾
i32 TurnSystem::GetCurrentTurn() { return current_turn_; }

}  // namespace system
}  // namespace legend