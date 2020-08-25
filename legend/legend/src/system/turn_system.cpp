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
TurnSystem::~TurnSystem() { static_objects_.clear(); }

//������
bool TurnSystem::Init(const std::string& stage_name) {
  //�X�e�[�W�f�[�^�̊g���q��.txt
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

  //�X�e�[�W�f�[�^�̓ǂݍ���
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

    // for (auto&& param : obstacles) {
    //  auto obj = std::make_unique<object::Obstacle>();
    //  if (!obj->Init(this, param)) {
    //    return false;
    //  }
    //  static_objects_.emplace_back(std::move(obj));
    //}

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

    //�T���f�[�^�̊g���q��.txt
    auto search_path = util::Path::GetInstance()->exe() / "assets" / "stage" /
                       (stage_name + "_searchData" + ".txt");
    search_manager_.Make(search_path);
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

  fade_.Update();
  if (is_scene_end_fade_start_) {
    if (fade_.IsEnd()) {
      is_scene_all_end_ = true;
    }
  } else {
    if (player_->GetPlayerDeathFlag()) {
      is_scene_end_fade_start_ = true;
      fade_.StartFadeOut(1.0f);
    }
    if (enemy_manager_.IsGameClear()) {
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
      !player_->GetSkillSelect()) {
    if (current_camera_ == camera_mode::Main) {
      current_camera_ = camera_mode::Sub1;
    } else if (current_camera_ == camera_mode::Sub1) {
      current_camera_ = camera_mode::Main;
    }
  }

  //���C���J�����̏�Ԃ���Ȃ��ƈړ��ł��Ȃ��悤�ɂ���
  if (current_camera_ == camera_mode::Main) {
    //�v���C���[�̑��x�X�V�͓��͂��󂯎���ď�������
    if (!player_->GetSkillSelect())
      player_->CheckImpulse();
    else
      player_->SkillUpdate();
  } else {
    //����ȊO�̎��̓v���C���[�̈ړ����͏�Ԃ𖳗͉�����K�v������
  }

  return true;
}

//�v���C���[�̈ړ�������
bool TurnSystem::PlayerMoving() { return true; }

bool TurnSystem::PlayerSkillAfterModed() {
  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();
  audio.Start(util::resource::resource_names::audio::PLAYER_TURN_END, 1.0f);

  current_mode_ = Mode::ENEMY_MOVING;
  return true;
}

//�G�̈ړ�����
bool TurnSystem::EnemyMove() {
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

//�G�̈ړ��I��������
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

//�J�����̏�����
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
  current_camera_ = camera_mode::Main;
  return true;
}

void legend::system::TurnSystem::AddFragment(
    std::unique_ptr<object::Fragment> fragment) {
  fragments_.emplace_back(std::move(fragment));
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
    const math::Vector3 player_position = player_->GetPosition();
    camera->SetPosition(math::Vector3(
        player_position.x, camera->GetPosition().y, player_position.z));
  }
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

//�`��
void TurnSystem::Draw() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
  auto& render_resource_manager = device.GetRenderResourceManager();

  //�V���h�E�}�b�v�p�`��
  render_resource_manager.SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::NONE, false,
      directx::render_target::DepthStencilTargetID::SHADOW_MAP, true);

  cameras_[current_camera_]->RenderStart();

  actor_render_command_list_.Push(player_.get());
  for (auto&& obj : static_objects_) {
    actor_render_command_list_.Push(obj.get());
  }
  // for (auto&& fragment : fragments_) {
  //  actor_render_command_list_.Push(fragment.get());
  //}
  // for (auto&& item_box : item_boxes_) {
  //  actor_render_command_list_.Push(item_box.get());
  //}

  // player_->Draw();
  // for (auto&& obj : static_objects_) {
  //  obj->Draw();
  //}
  // for (auto&& fragment : fragments_) {
  //  fragment->Draw();
  //}
  // for (auto&& item_box : item_boxes_) {
  //  item_box->Draw();
  //}
  // enemy_manager_.Draw();

  math::Vector3 light_pos = math::Vector3(50, 80, 20);
  light_cb_.GetStagingRef().view = math::Matrix4x4::CreateView(
      light_pos, math::Vector3::kZeroVector, math::Vector3::kUpVector);
  light_cb_.GetStagingRef().proj =
      player_follow_lookat_camera_->GetProjection();
  light_cb_.UpdateStaging();
  light_cb_.RegisterHandle(device, 2);
  actor_render_command_list_.ShadowPass();

  render_resource_manager.SetRenderTargets(
      command_list,
      directx::render_target::RenderTargetID::DIFFERED_RENDERING_PRE, true,
      directx::render_target::DepthStencilTargetID::DEPTH_ONLY, true);
  actor_render_command_list_.RenderPass();

  // cameras_[current_camera_]->RenderStart();
  // player_->Draw();
  // for (auto&& obj : static_objects_) {
  //  obj->Draw();
  //}
  // for (auto&& fragment : fragments_) {
  //  fragment->Draw();
  //}
  // for (auto&& item_box : item_boxes_) {
  //  item_box->Draw();
  //}
  // enemy_manager_.Draw();

  render_resource_manager.SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::BACK_BUFFER, true,
      directx::render_target::DepthStencilTargetID::NONE, true);

  auto& resource = game::GameDevice::GetInstance()->GetResource();
  resource.GetPipeline()
      .Get(util::resource::resource_names::pipeline::DIFFERED_RENDERING)
      ->SetCommandList(command_list);
  render_resource_manager.UseAsSRV(
      device, directx::render_target::RenderTargetID::DIFFERED_RENDERING_PRE,
      0);
  render_resource_manager.UseAsSRV(
      device, directx::render_target::RenderTargetID::DIFFERED_RENDERING_PRE,
      1);
  render_resource_manager.UseAsSRV(
      device, directx::render_target::RenderTargetID::DIFFERED_RENDERING_PRE,
      2);
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
      directx::render_target::DepthStencilTargetID::DEPTH_ONLY, true);

  // for (auto&& graffiti : graffities_) {
  //  graffiti->Draw(command_list);
  //}

  // ui_board_.Draw();
  // fade_.Draw();

  ////�X�v���C�g�͍Ō�ɕ`�惊�X�g�ɂ�����̂��܂Ƃ߂ĕ`�悷��
  // game::GameDevice::GetInstance()->GetSpriteRenderer().DrawItems(command_list);

  actor_render_command_list_.Clear();
}

//�f�o�b�O�`��
void TurnSystem::DebugDraw() {
  // cameras_[current_camera_]->RenderStart();
  // search_manager_.DebugDraw(&physics_field_);
  // physics_field_.DebugDraw(cameras_[current_camera_].get());
}

bool legend::system::TurnSystem::IsGameEnd() const { return is_scene_all_end_; }

system::GameDataStorage::GameData legend::system::TurnSystem::GetResult()
    const {
  const system::GameDataStorage::GameEndType end_type = [&]() {
    if (enemy_manager_.IsGameClear())
      return system::GameDataStorage::GameEndType::BOSS_KILLED;
    else
      return system::GameDataStorage::GameEndType::PLAYER_DEAD;
  }();
  //�v���C���[�����S�������A�G�̃{�X�����S�����炻�̏���Ԃ�
  return system::GameDataStorage::GameData{
      end_type, CalcPlayerStrengthToPrintNumber(*player_), current_turn_};
}

//�^�[�����̑���
void TurnSystem::AddCurrentTurn() { current_turn_++; }

//���݂̃^�[�������擾
i32 TurnSystem::GetCurrentTurn() { return current_turn_; }

//�v���C���[�̈ړ��J�n������
void TurnSystem::PlayerMoveStartEvent() {
  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();
  current_mode_ = Mode::PLAYER_MOVING;
  current_camera_ = camera_mode::Sub1;
}

//�v���C���[�̈ړ��I��������
void TurnSystem::PlayerMoveEndEvent() {
  // 0.1�b��Ƀ��[�h��؂�ւ���
  countdown_timer_.Init(0.1f, [&]() {
    current_mode_ = Mode::PLAYER_SKILL_AFTER_MOVED;
    current_camera_ = camera_mode::Main;
  });
}

//�v���C���[�̃X�L������������
void TurnSystem::PlayerSkillActivate() {}

//�v���C���[�̃X�L�������I��������
void TurnSystem::PlayerSkillDeactivate() {}

player::Player* TurnSystem::GetPlayer() { return player_.get(); }

std::vector<enemy::Enemy*> TurnSystem::GetEnemies() {
  return enemy_manager_.GetEnemyPointers();
}

}  // namespace system
}  // namespace legend