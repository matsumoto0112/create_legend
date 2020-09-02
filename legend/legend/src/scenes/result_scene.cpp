#include "src/scenes/result_scene.h"

#include "src/game/game_device.h"
#include "src/system/game_data.h"
#include "src/ui/image.h"
#include "src/ui/number.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace scenes {
namespace resource_name = util::resource::resource_names;

//コンストラクタ
ResultScene::ResultScene(ISceneChange* scene_change) : Scene(scene_change) {}

ResultScene::~ResultScene() {}

//初期化
bool ResultScene::Initialize() {
  namespace TextureName = resource_name::texture;

  const auto window_size =
      game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
  const system::GameDataStorage::ResultData data =
      system::GameDataStorage::GetInstance()->GetResultData();
  const auto heap_id =
      directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY;

  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();
  bgm_ = audio.Start(resource_name::audio::BGM_RESULT, 1.0f, true);

  //プレイヤーが死亡したらgameover画像を出す
  if (data.end_type == system::GameDataStorage::GameEndType::PLAYER_DEAD) {
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
    //audio.Start(resource_name::audio::RESULT_GAMEOVER, 1.0f);
  } else {
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
    //audio.Start(resource_name::audio::RESULT_STAGE_CLEAR, 1.0f);
  }

  {  // ターン数表示
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
      const float y = center.y + 150.0f;
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
        center.y + 150.0f - turn_image->GetSprite().GetContentSize().y / 3.0f;
    turn_image->SetPosition(math::Vector2(x, y));
    turn_image->SetZOrder(0.5f);
    board_.AddComponent(std::move(turn_image));
  }

  {  // ポイント表示
    auto player_power_image = std::make_unique<ui::Image>();
    if (!player_power_image->Init(TextureName::RESULT_PLAYERPOWER_TEXT_1,
                                  heap_id)) {
      return false;
    }
    auto center = math::Vector2(window_size.x * 0.75f, 400.0f);
    auto pos =
        center + math::Vector2::kRightVector *
                     -player_power_image->GetSprite().GetContentSize().x / 2.0f;
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
      const float y = center.y + 150.0f;
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
        center.y + 150.0f - power_image->GetSprite().GetContentSize().y / 3.0f;
    power_image->SetPosition(math::Vector2(x, y));
    power_image->SetZOrder(0.5f);
    board_.AddComponent(std::move(power_image));
  }

  fade_.Init(resource_name::texture::FADE_IMAGE);
  fade_.StartFadeIn(1.0f);
  is_scene_end_ = false;
  return true;
}

//更新
bool ResultScene::Update() {
  auto& input = game::GameDevice::GetInstance()->GetInput();
  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();

  //シーンが終了しているならフェード処理をする
  if (is_scene_end_) {
    fade_.Update();
    //フェードまで終了したら次のシーンに向かう
    if (fade_.IsEnd()) {
      scene_change_->ChangeScene(SceneType::TITLE);
    }
    return true;
  }

  //決定キーでフェードを開始し、シーンを終了する
  if (input.GetCommand(input::input_code::Decide)) {
    fade_.StartFadeOut(1.0f);
    is_scene_end_ = true;
    audio.Start(resource_name::audio::RESULT_MOVE_OTHER_SCENE, 1.0f);
  }

  fade_.Update();
  return true;
}

//描画
void ResultScene::Draw() {
  Scene::Draw();

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();

  device.GetRenderResourceManager().SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::BACK_BUFFER, false,
      directx::render_target::DepthStencilTargetID::DEPTH_ONLY, true);
  board_.Draw();

  fade_.Draw();
  game::GameDevice::GetInstance()->GetSpriteRenderer().DrawItems(command_list);
}

void ResultScene::Finalize() {
  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();
  audio.Stop(bgm_);

  game::GameDevice::GetInstance()->GetDevice().WaitExecute();
}

}  // namespace scenes
}  // namespace legend