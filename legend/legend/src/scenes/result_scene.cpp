#include "src/scenes/result_scene.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"
#include "src/ui/image.h"
#include "src/ui/number.h"
#include "src/util/resource/resource_names.h"

namespace {
constexpr float floor_pos = -50.0f;
}

namespace legend {
namespace scenes {
namespace resource_name = util::resource::resource_names;
namespace TextureName = resource_name::texture;

//コンストラクタ
ResultScene::ResultScene(ISceneChange* scene_change) : Scene(scene_change) {}

ResultScene::~ResultScene() {}

//初期化
bool ResultScene::Initialize() {
  const auto window_size =
      game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
  const system::GameDataStorage::ResultData data =
      system::GameDataStorage::GetInstance()->GetResultData();
  const auto heap_id =
      directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY;

  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();
  bgm_ = audio.Start(resource_name::audio::BGM_RESULT, 1.0f, true);

  draw_timer_.Init(1.0f);
  is_draw_ = false;

  start_cacth_timer_.Init(1.0f);

  if (!LoadStageData()) return false;

  if (!back_ground_.Init()) {
    return false;
  }

  //プレイヤーが死亡したら
  if (data.end_type == system::GameDataStorage::GameEndType::PLAYER_DEAD) {
    if (!LoseProductionInit(window_size, heap_id)) return false;
    transforms_[0].SetPosition(math::Vector3(-60, floor_pos, -20));
    float rotate =
        game::GameDevice::GetInstance()->GetRandom().Range(-180.0f, 180.0f);
    transforms_[0].SetRotation(
        math::Quaternion::FromEular(0, rotate * math::util::DEG_2_RAD, 0));
  } else {
    if (!WinProductionInit(window_size, heap_id)) return false;
    transforms_[0].SetPosition(math::Vector3(30, 2, 0));
  }
  transform_cbs_[0].GetStagingRef().world = transforms_[0].CreateWorldMatrix();
  transform_cbs_[0].UpdateStaging();

  if (!Turn(window_size, heap_id, data)) return false;

  if (!PlayerPower(window_size, heap_id, data)) return false;

  fade_.Init(resource_name::texture::FADE_IMAGE);
  fade_.StartFadeIn(1.0f);
  is_scene_end_ = false;
  return true;
}

//更新
bool ResultScene::Update() {
  if (mode_ == ResultMode::END_PRODUCTION) {
    if (!is_draw_) {
      if (!draw_timer_.Update()) {
        return true;
      }
      is_draw_ = true;
    }
  } else if (mode_ == ResultMode::LOSE) {
    is_draw_ = true;
  }

  ProductionUpdate();

  return true;
}

//描画
void ResultScene::Draw() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  auto& render_resource_manager = device.GetRenderResourceManager();
  directx::device::CommandList& command_list =
      device.GetCurrentFrameResource()->GetCommandList();
  render_resource_manager.SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::BACK_BUFFER, true,
      directx::render_target::DepthStencilTargetID::DEPTH_ONLY, true);

  camera_.RenderStart();
  back_ground_.Draw(util::resource::resource_names::pipeline::MODEL_VIEW);
  resource.GetPipeline()
      .Get(util::resource::resource_names::pipeline::MODEL_VIEW)
      ->SetCommandList(command_list);
  for (i32 i = 0; i < transforms_.size(); i++) {
    transform_cbs_[i].GetStagingRef().world =
        transforms_[i].CreateWorldMatrix();
    transform_cbs_[i].UpdateStaging();
    transform_cbs_[i].RegisterHandle(
        device, directx::shader::ConstantBufferRegisterID::TRANSFORM);
    models_[i]->Draw(command_list);
  }

  if (is_draw_) {
    board_.Draw();
  }
  fade_.Draw();
  game::GameDevice::GetInstance()->GetSpriteRenderer().DrawItems(command_list);
}

void ResultScene::Finalize() {
  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();
  audio.Stop(bgm_);

  game::GameDevice::GetInstance()->GetDevice().WaitExecute();
}

bool ResultScene::LoadStageData() {
  const std::string stage_name = "stage_01";
  auto stage_path = util::Path::GetInstance()->exe() / "assets" / "stage" /
                    (stage_name + ".txt");
  stage_generator_.LoadStringStageData(stage_path, stage_name);

  player::Player::InitializeParameter player;
  std::vector<object::Desk::InitializeParameter> desks;
  std::vector<object::Obstacle::InitializeParameter> obstacles;
  std::vector<object::GraffitiInitializeParameter> graffities;
  std::vector<skill::SkillItemBox::InitializeParameter> item_boxes;
  std::vector<enemy::Enemy::InitializeParameter> enemys;
  std::vector<enemy::Boss::InitializeParameter> bosses;
  if (!stage_generator_.GetMapActors(0, player, desks, obstacles, graffities,
                                     item_boxes, enemys, bosses)) {
    return false;
  }

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  {
    util::Transform transform = player.transform;
    transforms_.emplace_back(transform);
    TransformConstantBuffer constant_buffer;
    constant_buffer.Init(
        device,
        device.GetLocalHandle(
            directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID),
        L"Player_Transform");
    constant_buffer.GetStagingRef().world = transform.CreateWorldMatrix();
    constant_buffer.UpdateStaging();
    transform_cbs_.emplace_back(constant_buffer);
    models_.emplace_back(
        std::move(resource.GetModel().Get(resource_name::model::ERASER_01)));
  }
  for (auto&& param : desks) {
    util::Transform transform = param.transform;
    transforms_.emplace_back(transform);
    TransformConstantBuffer constant_buffer;
    constant_buffer.Init(
        device,
        device.GetLocalHandle(
            directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID),
        L"Desk_Transform");
    constant_buffer.GetStagingRef().world = transform.CreateWorldMatrix();
    constant_buffer.UpdateStaging();
    transform_cbs_.emplace_back(constant_buffer);
    models_.emplace_back(
        std::move(resource.GetModel().Get(resource_name::model::DESK)));
  }
  for (auto&& param : obstacles) {
    util::Transform transform;
    transform.SetPosition(param.position);
    transform.SetRotation(param.rotation);
    transform.SetScale(math::Vector3::kUnitVector);
    transforms_.emplace_back(transform);
    TransformConstantBuffer constant_buffer;
    constant_buffer.Init(
        device,
        device.GetLocalHandle(
            directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID),
        L"Obstacle_Transform");
    constant_buffer.GetStagingRef().world = transform.CreateWorldMatrix();
    constant_buffer.UpdateStaging();
    transform_cbs_.emplace_back(constant_buffer);
    models_.emplace_back(std::move(
        resource.GetModel().Get(resource_name::model::FIELD_OBJECT_01)));
  }
  for (auto&& param : enemys) {
    util::Transform transform = param.transform;
    float pos_x =
        game::GameDevice::GetInstance()->GetRandom().Range(100.0f, 120.0f);
    float pos_z =
        game::GameDevice::GetInstance()->GetRandom().Range(-60.0f, 60.0f);
    transform.SetPosition(math::Vector3(pos_x, floor_pos, pos_z));
    float rotate =
        game::GameDevice::GetInstance()->GetRandom().Range(-180.0f, 180.0f);
    transform.SetRotation(
        math::Quaternion::FromEular(0, rotate * math::util::DEG_2_RAD, 0));
    transforms_.emplace_back(transform);
    TransformConstantBuffer constant_buffer;
    constant_buffer.Init(
        device,
        device.GetLocalHandle(
            directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID),
        L"Enemy_Transform");
    constant_buffer.GetStagingRef().world = transform.CreateWorldMatrix();
    constant_buffer.UpdateStaging();
    transform_cbs_.emplace_back(constant_buffer);
    models_.emplace_back(std::move(
        resource.GetModel().Get(resource_name::model::ENEMY_ERASER_01)));
  }

  return true;
}

//勝利演出初期化
bool ResultScene::WinProductionInit(
    math::IntVector2 window_size,
    directx::descriptor_heap::heap_parameter::LocalHeapID heap_id) {
  //演出初期状態
  mode_ = ResultMode::WIN_INITIAL;

  {
    const math::Vector3 pos = math::Vector3(50.0f, 15.0f, 0.0f);
    const math::Quaternion rot = math::Quaternion::FromEular(
        math::Vector3(20.0f, -90.0f, 0.0f) * math::util::DEG_2_RAD);
    const float fov = 50.0f * math::util::DEG_2_RAD;
    const float aspect = 1280.0f / 720.0f;
    if (!camera_.Init(L"MainCamera", pos, rot, fov, aspect,
                      math::Vector3::kUpVector, 0.1f, 1000.0f)) {
      return false;
    }
  }

  auto image = std::make_unique<ui::Image>();
  if (!image->Init(TextureName::RESULT_STAGECLEAR, heap_id)) {
    return false;
  }
  const float x =
      window_size.x * 0.5f - image->GetSprite().GetContentSize().x * 0.5f;
  const float y = 200.0f;
  image->SetPosition(math::Vector2(x, y));
  image->SetZOrder(0.5f);
  board_.AddComponent(std::move(image));
  // audio.Start(resource_name::audio::RESULT_STAGE_CLEAR, 1.0f);

  return true;
}

//敗北演出初期化
bool ResultScene::LoseProductionInit(
    math::IntVector2 window_size,
    directx::descriptor_heap::heap_parameter::LocalHeapID heap_id) {
  //敗北演出
  mode_ = ResultMode::LOSE;

  {
    const math::Vector3 pos = math::Vector3(-60.0f, -25.0f, -45.0f);
    const math::Quaternion rot =
        math::Quaternion::FromEular(45.0f * math::util::DEG_2_RAD, 0.0f, 0.0f);
    const float fov = 50.0f * math::util::DEG_2_RAD;
    const float aspect = 1280.0f / 720.0f;
    if (!camera_.Init(L"MainCamera", pos, rot, fov, aspect,
                      math::Vector3::kUpVector, 0.1f, 1000.0f)) {
      return false;
    }
  }

  auto image = std::make_unique<ui::Image>();
  if (!image->Init(TextureName::RESULT_GAMEOVER, heap_id)) {
    return false;
  }
  const float x =
      window_size.x * 0.5f - image->GetSprite().GetContentSize().x * 0.5f;
  const float y = 200.0f;
  image->SetPosition(math::Vector2(x, y));
  image->SetZOrder(0.5f);
  board_.AddComponent(std::move(image));
  // audio.Start(resource_name::audio::RESULT_GAMEOVER, 1.0f);

  return true;
}

//ターン表示処理
bool ResultScene::Turn(
    math::IntVector2 window_size,
    directx::descriptor_heap::heap_parameter::LocalHeapID heap_id,
    system::GameDataStorage::ResultData data) {
  auto play_turn_image = std::make_unique<ui::Image>();
  if (!play_turn_image->Init(TextureName::RESULT_TOTALTURN_TEXT_1, heap_id)) {
    return false;
  }
  auto center = math::Vector2(window_size.x * 0.25f, 400.0f);
  auto pos = center + math::Vector2::kRightVector *
                          -play_turn_image->GetSprite().GetContentSize().x /
                          2.0f;
  play_turn_image->SetPosition(pos);
  play_turn_image->SetZOrder(0.5f);
  board_.AddComponent(std::move(play_turn_image));

  const i32 clear_turn = data.play_turn;
  std::string line = std::to_string(clear_turn);
  std::vector<std::unique_ptr<ui::Number>> numbers = {};
  float numbers_width = 0.0f;
  float number_space = 8.0f;

  for (i32 index = 0; index < line.size(); index++) {
    auto num = std::make_unique<ui::Number>();
    num->Init(TextureName::UI_NUMBER_1, heap_id);
    numbers_width += num->GetSprite().GetContentSize().x / 10.0f * 0.5f;
    numbers_width += number_space;

    numbers.emplace_back(std::move(num));
  }
  for (i32 index = 0; index < numbers.size(); index++) {
    auto number_size = numbers[index]->GetSprite().GetContentSize().x / 10.0f;
    const float x = center.x + (-numbers_width + number_size);
    const float y = center.y + 100.0f;
    numbers_width -= number_size + number_space;
    auto c = line[index];
    numbers[index]->SetNumber(std::stoi(&c));
    numbers[index]->SetPosition(math::Vector2(x, y));
    numbers[index]->SetZOrder(0.5f);
    board_.AddComponent(std::move(numbers[index]));
  }
  auto turn_image = std::make_unique<ui::Image>();
  if (!turn_image->Init(TextureName::RESULT_TOTALTURN_TEXT_2, heap_id)) {
    return false;
  }
  const float x = center.x + (-numbers_width) +
                  turn_image->GetSprite().GetContentSize().x / 3.0f;
  const float y =
      center.y + 100.0f - turn_image->GetSprite().GetContentSize().y / 3.0f;
  turn_image->SetPosition(math::Vector2(x, y));
  turn_image->SetZOrder(0.5f);
  board_.AddComponent(std::move(turn_image));

  return true;
}

//プレイヤーパワー表示処理
bool ResultScene::PlayerPower(
    math::IntVector2 window_size,
    directx::descriptor_heap::heap_parameter::LocalHeapID heap_id,
    system::GameDataStorage::ResultData data) {
  auto player_power_image = std::make_unique<ui::Image>();
  if (!player_power_image->Init(TextureName::RESULT_PLAYERPOWER_TEXT_1,
                                heap_id)) {
    return false;
  }
  auto center = math::Vector2(window_size.x * 0.75f, 400.0f);
  auto pos = center + math::Vector2::kRightVector *
                          -player_power_image->GetSprite().GetContentSize().x /
                          2.0f;
  player_power_image->SetPosition(pos);
  player_power_image->SetZOrder(0.5f);
  board_.AddComponent(std::move(player_power_image));

  const i32 player_power = data.player_power;
  std::string line = std::to_string(player_power);
  std::vector<std::unique_ptr<ui::Number>> numbers = {};
  float numbers_width = 0.0f;
  float number_space = 8.0f;

  for (i32 index = 0; index < line.size(); index++) {
    auto num = std::make_unique<ui::Number>();
    num->Init(TextureName::UI_NUMBER_1, heap_id);
    numbers_width += num->GetSprite().GetContentSize().x / 10.0f * 0.5f;
    numbers_width += number_space;

    numbers.emplace_back(std::move(num));
  }
  for (i32 index = 0; index < numbers.size(); index++) {
    auto number_size = numbers[index]->GetSprite().GetContentSize().x / 10.0f;
    const float x = center.x + (-numbers_width + number_size);
    const float y = center.y + 100.0f;
    numbers_width -= number_size + number_space;
    auto c = line[index];
    numbers[index]->SetNumber(std::stoi(&c));
    numbers[index]->SetPosition(math::Vector2(x, y));
    numbers[index]->SetZOrder(0.5f);
    board_.AddComponent(std::move(numbers[index]));
  }
  auto power_image = std::make_unique<ui::Image>();
  if (!power_image->Init(TextureName::RESULT_PLAYERPOWER_TEXT_2, heap_id)) {
    return false;
  }
  const float x = center.x + (-numbers_width) +
                  power_image->GetSprite().GetContentSize().x / 3.0f;
  const float y =
      center.y + 100.0f - power_image->GetSprite().GetContentSize().y / 3.0f;
  power_image->SetPosition(math::Vector2(x, y));
  power_image->SetZOrder(0.5f);
  board_.AddComponent(std::move(power_image));

  return true;
}

//演出更新
void ResultScene::ProductionUpdate() {
  auto& input = game::GameDevice::GetInstance()->GetInput();
  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();

  if (mode_ == ResultMode::WIN_INITIAL) {
    if (fade_.IsEnd()) {
      if (!start_cacth_timer_.Update()) return;
      mode_ = ResultMode::CAMERA_CACTH;
    }
  } else if (mode_ == ResultMode::CAMERA_CACTH) {
    const float destination_x = 200.0f;
    math::Vector3 velocity = math::Vector3::kRightVector *
                             game::GameDevice::GetInstance()
                                 ->GetFPSCounter()
                                 .GetDeltaSeconds<float>() *
                             100;
    camera_.SetPosition(camera_.GetPosition() + velocity);
    if (camera_.GetPosition().x >= destination_x ||
        input.GetCommand(input::input_code::Decide)) {
      camera_.SetPosition(math::Vector3(destination_x, camera_.GetPosition().y,
                                        camera_.GetPosition().z));
      mode_ = ResultMode::END_PRODUCTION;
    }
  } else {
    //シーンが終了しているならフェード処理をする
    if (is_scene_end_) {
      fade_.Update();
      //フェードまで終了したら次のシーンに向かう
      if (fade_.IsEnd()) {
        scene_change_->ChangeScene(SceneType::TITLE);
      }
      return;
    }

    //決定キーでフェードを開始し、シーンを終了する
    if (input.GetCommand(input::input_code::Decide)) {
      fade_.StartFadeOut(1.0f);
      is_scene_end_ = true;
      audio.Start(resource_name::audio::RESULT_MOVE_OTHER_SCENE, 1.0f);
    }
  }
  fade_.Update();
}

}  // namespace scenes
}  // namespace legend