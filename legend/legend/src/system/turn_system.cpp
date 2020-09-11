#include "src/system/turn_system.h"

#include "src/directx/shader/shader_register_id.h"
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

//�R���X�g���N�^
TurnSystem::TurnSystem() : current_turn_(0) {}

//�f�X�g���N�^
TurnSystem::~TurnSystem() { /*static_objects_.clear();*/
}

//������
bool TurnSystem::Init(const std::string& stage_name) {
  //�A�N�^�[�Ǘ��N���X�̏�����
  actor_manager_.Init(stage_name, this);

  //�^�[���ύX���o�p�N���X
  turn_change_.Init(this);

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

    const std::set<std::wstring> NO_RENDER_IF_ENEMY_TUAN_UI_NAMES = {
        util::resource::resource_names::texture::UI_POWERGAUGE_FRAME,
        util::resource::resource_names::texture::UI_POWERGAUGE,
        util::resource::resource_names::texture::UI_POWERGAUGE_BG,
        util::resource::resource_names::texture::UI_POWERGAUGE_STICK,
    };
    //�G�̃^�[���ł͕\�����Ȃ�UI�����X�g�ɐς�
    if (NO_RENDER_IF_ENEMY_TUAN_UI_NAMES.find(w_name) !=
        NO_RENDER_IF_ENEMY_TUAN_UI_NAMES.end()) {
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

  if (!light_cb_.Init(
          device,
          device.GetLocalHandle(
              directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY),
          L"LightConstantBuffer")) {
    return false;
  }
  light_cb_.GetStagingRef().light_position =
      math::Vector4(3000.0f, 2000.0f, 500.0f, 1.0f);
  light_cb_.GetStagingRef().light_color = util::Color4(1.0f, 1.0f, 1.0f, 1.0f);
  light_cb_.UpdateStaging();

  current_turn_ = 0;
  view_turn_ = 0;

  return true;
}

bool TurnSystem::Update() {
  if (ImGui::Begin("TurnSystem")) {
    ImGui::Text("CurrntTurn : %d", current_turn_);
    ImGui::Text("ViewTurn : %d", view_turn_);
    if (ImGui::Button("AddTurn")) {
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
      {Mode::PLAYER_END_ADD_SKILL, [&]() { return AddSkill(); }},
      {Mode::ENEMY_END_ADD_SKILL, [&]() { return AddSkill(); }},
      {Mode::TO_PLAYER_TURN_, [&]() { return turn_change_.Update(); }},
      {Mode::TO_ENEMY_TURN_, [&]() { return turn_change_.Update(); }},
      {Mode::ENEMY_GENERATE, [&]() { return EnemyGenerate(); }}};
  if (!switcher.at(current_mode_)()) {
    return false;
  }

  {
    //�v���C���[�̋�����Ԃ�UI���l�ɕϊ�����
    const i32 str =
        CalcPlayerStrengthToPrintNumber(*actor_manager_.GetPlayer());
    const i32 third_digit = str / 100;
    numbers_[number_id::DIGIT_3]->SetNumber(third_digit);
    numbers_[number_id::DIGIT_3]->SetEnable(third_digit != 0);
    const i32 second_digit = str / 10 % 10;
    numbers_[number_id::DIGIT_2]->SetNumber(second_digit);
    numbers_[number_id::DIGIT_2]->SetEnable(
        !(second_digit == 0 && third_digit == 0));
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
  auto* camera_manager = actor_manager_.GetCameraManager();
  if (input.GetCommand(input::input_code::CAMERA_CHANGE) &&
      !actor_manager_.GetPlayer()->GetSkillSelect()) {
    auto SwitchCamera = [&]() {
      if (camera_manager->GetCameraMode() ==
          camera::camera_mode::PLAYER_LOOKAT) {
        camera_manager->SetCameraMode(camera::camera_mode::BIRDS_EYE_VIEW);
        ChangeUIRenderEnable(false);
      } else {
        camera_manager->SetCameraMode(camera::camera_mode::PLAYER_LOOKAT);
        ChangeUIRenderEnable(true);
      }
    };
    SwitchCamera();
  }

  //���C���J�����̏�Ԃ���Ȃ��ƈړ��ł��Ȃ��悤�ɂ���
  if (camera_manager->GetCameraMode() == camera::camera_mode::PLAYER_LOOKAT) {
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
    before_mode_ = current_mode_;
    current_mode_ = Mode::PLAYER_SKILL_AFTER_MOVED;
    return true;
  }
  if (!actor_manager_.IsAllActorStop()) {
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
  //audio.Start(util::resource::resource_names::audio::PLAYER_TURN_END, 1.0f);

  before_mode_ = current_mode_;
  current_mode_ = Mode::PLAYER_END_ADD_SKILL;
  return true;
}

bool TurnSystem::AddSkill() {
  actor_manager_.GetCameraManager()->SetUpdateEnable(false);
  actor_manager_.GetPlayer()->EquipmentUpdate();
  return true;
}

//�G�̈ړ�����
bool TurnSystem::EnemyMove() {
  actor_manager_.EnemyManagerUpdate();
  if (actor_manager_.GetEnemyManager()->LastEnemyMoveEnd()) {
    before_mode_ = current_mode_;
    current_mode_ = Mode::ENEMY_MOVE_END;
    actor_manager_.GetEnemyManager()->ResetEnemyMove();
  }
  return true;
}

//�G�̈ړ��I��������
bool TurnSystem::EnemyMoveEnd() {
  if (actor_manager_.GetPlayer()->SkillUpdateTurnEnd()) {
    return true;
  }

  before_mode_ = current_mode_;
  current_mode_ = Mode::ENEMY_END_ADD_SKILL;
  return true;
}

bool TurnSystem::EnemyMoveProducing() {
  if (actor_manager_.IsAllActorStop() || !actor_manager_.GetEnemyManager()->IsGameClear()) {
    ToPlayerTurn();
  }

  return true;
}

bool TurnSystem::BossMoveProducing() {
  if (actor_manager_.IsAllActorStop() || !actor_manager_.GetEnemyManager()->IsGameClear()) {
    ToPlayerTurn();
  }

  return true;
}

bool TurnSystem::EnemyGenerate() {
  //�^�[���̍X�V
  AddCurrentTurn();

  //�G�̐���
  if (!actor_manager_.GenerateActors(current_turn_)) {
    MY_LOG(L"�}�b�v�I�u�W�F�N�g�̐����Ɏ��s���܂����B");
    return false;
  }

  if (actor_manager_.GetEnemiesSize() > 0 || actor_manager_.IsBossGenerated()) {
    current_mode_ = Mode::ENEMY_PRODUCTION;
  }

  return true;
}

void TurnSystem::ChangeUIRenderEnable(bool enabler) {
  ui_board_.SetRenderEnable(enabler);
  actor_manager_.GetPlayer()->SkillUIChangeEnable(enabler);
}

bool TurnSystem::ToPlayerTurn() {
  current_mode_ = Mode::TO_PLAYER_TURN_;
  view_turn_++;

  if (current_turn_ <= 0) {
    current_mode_ = Mode::PLAYER_MOVE_READY;
    return true;
  }

  return turn_change_.ChangeStart(Mode::PLAYER_MOVE_READY);
}

bool TurnSystem::ToEnemyTurn() {
  current_mode_ = Mode::TO_ENEMY_TURN_;
  return turn_change_.ChangeStart(Mode::ENEMY_MOVING);
}

i32 TurnSystem::GetBossGenerateTurn() {
  return actor_manager_.GetBossGenerateTurn();
}

//�`��
void TurnSystem::Draw() {
  //�`��̗���
  // 1.Differed-Rendering�p��G-Buffer�����̂��߁A�ʏ�`�惂�f����`�悷��
  // 2.Differed-Rendering�`����o�b�N�o�b�t�@�ɍs��
  // 3.���̑��I�u�W�F�N�g�̕`�������

  using directx::render_target::DepthStencilTargetID;
  using directx::render_target::RenderTargetID;
  namespace TextureRegisterID = directx::shader::TextureRegisterID;
  namespace ConstantBufferRegisterID =
      directx::shader::ConstantBufferRegisterID;

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
  auto& render_resource_manager = device.GetRenderResourceManager();
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  light_cb_.RegisterHandle(device, ConstantBufferRegisterID::LIGHT);

  // Differed-Rendering�pG-Buffer����
  actor_manager_.DrawDifferedRenderingObject(command_list);

  //���̑��I�u�W�F�N�g�`��
  game::GameDevice::GetInstance()->GetParticleCommandList().RenderParticle(
      command_list);

  render_resource_manager.SetRenderTargets(command_list,
                                           RenderTargetID::BACK_BUFFER, false,
                                           DepthStencilTargetID::NONE, false);
  actor_manager_.DrawAlphaObject(command_list);

  render_resource_manager.SetRenderTargets(
      command_list, RenderTargetID::BACK_BUFFER, false,
      DepthStencilTargetID::DEPTH_ONLY, false);
  actor_manager_.Draw2D(command_list);

  const bool is_player_turn = current_mode_ == Mode::PLAYER_MOVING ||
                              current_mode_ == Mode::PLAYER_MOVE_READY;
  for (auto&& ui : no_render_if_enemy_turn_uis_) {
    ui->SetEnable(is_player_turn);
  }

  ui_board_.Draw();
  fade_.Draw();
  turn_change_.Draw();

  //�X�v���C�g�͍Ō�ɕ`�惊�X�g�ɂ�����̂��܂Ƃ߂ĕ`�悷��
  game::GameDevice::GetInstance()->GetSpriteRenderer().DrawItems(command_list);

  actor_manager_.DrawEnd();
}

//�f�o�b�O�`��
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
  //�v���C���[�����S�������A�G�̃{�X�����S�����炻�̏���Ԃ�
  return system::GameDataStorage::ResultData{
      end_type, CalcPlayerStrengthToPrintNumber(*actor_manager_.GetPlayer()),
      view_turn_};
}

void TurnSystem::SetTurnMode(Mode mode) {
  before_mode_ = current_mode_;
  current_mode_ = mode;
}

Mode TurnSystem::GetCurrentMode() { return current_mode_; }

Mode TurnSystem::GetBeforeMode() { return before_mode_; }
//�^�[�����̑���
void TurnSystem::AddCurrentTurn() { current_turn_++; }

//���݂̃^�[�������擾
i32 TurnSystem::GetCurrentTurn() { return current_turn_; }

i32 TurnSystem::GetViewTurn() { return view_turn_; }

}  // namespace system
}  // namespace legend