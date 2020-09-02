#include "src/scenes/title.h"

#include "src/game/game_device.h"
#include "src/ui/image.h"
#include "src/util/resource/resource_names.h"

namespace {
constexpr const wchar_t* STAGE_LIST[] = {
    legend::util::resource::resource_names::texture::RESULT_GAMEOVER,
    legend::util::resource::resource_names::texture::TITLE_LOGO,
    legend::util::resource::resource_names::texture::RESULT_PLAYERPOWER_TEXT_1,
    legend::util::resource::resource_names::texture::RESULT_PLAYERPOWER_TEXT_2,
    legend::util::resource::resource_names::texture::RESULT_STAGECLEAR,
};

constexpr float Lerp(float a, float b, float t) {
  t = legend::math::util::Clamp(t, 0.0f, 1.0f);
  return b * t + a * (1.0f - t);
}
}  // namespace

namespace legend {
namespace scenes {

//コンストラクタ
Title::Title(ISceneChange* scene_change) : Scene(scene_change) {}

//初期化
bool Title::Initialize() {
  current_phase_ = Phase::TITLE;
  stage_select_move_direction_ = StageSelectMoveDirection::NONE;
  current_select_stage_item_id_ = util::ModInt(0, MAX_STAGE_ITEM_COUNT);

  game::GameDevice::GetInstance()->GetDevice().GetHeapManager().ResetLocalHeap(
      directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY);

  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();

  bgm_key_ =
      audio.Start(util::resource::resource_names::audio::BGM_TITLE, 1.0f, true);

  const auto screen_size =
      game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
  {
    auto image = std::make_unique<ui::Image>();
    if (!image->Init(
            util::resource::resource_names::texture::TITLE_LOGO,
            directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY)) {
      return false;
    }
    const float x =
        screen_size.x * 0.5f - image->GetSprite().GetContentSize().x * 0.5f;
    const float y = image->GetSprite().GetContentSize().y * 0.5f;
    image->SetPosition(math::Vector2(x, y));
    image->SetZOrder(0.5f);
    ui::UIComponent* comp = board_.AddComponent(std::move(image));
    title_images_.emplace_back(comp);
  }
  {
    auto image = std::make_unique<ui::Image>();
    if (!image->Init(
            util::resource::resource_names::texture::TITLE_PUSHBUTTON_GUIDE,
            directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY)) {
      return false;
    }
    const float x =
        screen_size.x * 0.5f - image->GetSprite().GetContentSize().x * 0.5f;
    const float y = image->GetSprite().GetContentSize().y * 0.5f + 400.0f;
    image->SetPosition(math::Vector2(x, y));
    image->SetZOrder(0.5f);
    ui::UIComponent* comp = board_.AddComponent(std::move(image));
    title_images_.emplace_back(comp);
  }

  {
    const u64 size = _countof(STAGE_LIST);
    for (u64 i = 0; i < size; i++) {
      auto image = std::make_unique<ui::Image>();
      if (!image->Init(STAGE_LIST[i], directx::descriptor_heap::heap_parameter::
                                          LocalHeapID::ONE_PLAY)) {
        return false;
      }
      //画面の中心に置いたときのx座標
      const float base_x =
          screen_size.x * 0.5f - image->GetSprite().GetContentSize().x * 0.5f;
      //画像の座標は基底の位置からスクリーン座標分ずらす
      const float x = base_x + screen_size.x * i;
      const float y = image->GetSprite().GetContentSize().y * 0.5f + 200.0f;
      image->SetPosition(math::Vector2(x, y));
      image->SetZOrder(0.2f);
      image->SetEnable(false);
      ui::UIComponent* comp = board_.AddComponent(std::move(image));
      stage_images_.emplace_back(comp, base_x,
                                 static_cast<float>(i) * screen_size.x);
    }
  }

  fade_.Init(util::resource::resource_names::texture::FADE_IMAGE);
  fade_.StartFadeIn(1.0f);
  is_scene_end_ = false;

  return true;
}

//更新
bool Title::Update() {
  auto& input = game::GameDevice::GetInstance()->GetInput();
  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();

  auto UpdateTitle = [&]() {
    //決定キーでフェードを開始し、シーンを終了する
    if (input.GetCommand(input::input_code::Decide)) {
      audio.Start(util::resource::resource_names::audio::TITLE_DECISION, 1.0f);
      current_phase_ = Phase::STAGE_SELECT;
      for (auto&& im : title_images_) {
        im->SetEnable(false);
      }
      for (auto&& st : stage_images_) {
        st.component->SetEnable(true);
      }
    }
  };

  auto GetLerpT = [&]() {
    return 1.0f -
           (stage_move_select_timer_.CurrentTime() / STAGE_ITEM_MOVE_TIME);
  };

  auto UpdateStageItems = [&](float t) {
    for (auto&& st : stage_images_) {
      const math::Vector2 prev_pos = st.component->GetPosition();
      const float x = Lerp(st.prev_x, st.next_x, t) + st.base_x;
      st.component->SetPosition(math::Vector2(x, prev_pos.y));
    }
  };

  auto SetupStageUpdate = [&]() {
    const auto screen_size =
        game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
    stage_move_select_timer_.Init(STAGE_ITEM_MOVE_TIME);
    for (i32 i = 0; i < MAX_STAGE_ITEM_COUNT; i++) {
      auto& st = stage_images_[i];
      st.prev_x = st.next_x;
      st.next_x =
          screen_size.x * 1.0f * (i - current_select_stage_item_id_.Get());
    }
  };

  auto UpdateStageSelect = [&]() {
    const auto screen_size =
        game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
    const bool stage_image_moving =
        stage_select_move_direction_ == StageSelectMoveDirection::LEFT ||
        stage_select_move_direction_ == StageSelectMoveDirection::RIGHT;

    if (stage_image_moving) {
      if (stage_move_select_timer_.Update()) {
        stage_select_move_direction_ = StageSelectMoveDirection::NONE;
      }
      UpdateStageItems(GetLerpT());

      return;
    }

    //決定キーでフェードを開始し、シーンを終了する
    if (const bool input_decide = input.GetCommand(input::input_code::Decide);
        input_decide) {
      fade_.StartFadeOut(1.0f);
      is_scene_end_ = true;
      audio.Start(util::resource::resource_names::audio::TITLE_DECISION, 1.0f);
      current_phase_ = Phase::END;
    } else if (const bool input_right = input.GetHorizontal() > 0.0f;
               input_right) {
      current_select_stage_item_id_++;
      stage_select_move_direction_ = StageSelectMoveDirection::RIGHT;
      SetupStageUpdate();
    } else if (const bool input_left = input.GetHorizontal() < 0.0f;
               input_left) {
      current_select_stage_item_id_--;
      stage_select_move_direction_ = StageSelectMoveDirection::LEFT;
      SetupStageUpdate();
    }
  };

  auto UpdateFade = [&]() {
    //シーンが終了しているならフェード処理をする
    if (is_scene_end_) {
      fade_.Update();
      //フェードまで終了したら次のシーンに向かう
      if (fade_.IsEnd()) {
        scene_change_->ChangeScene(SceneType::MAIN_SCENE_1);
      }
    }
  };

  switch (current_phase_) {
    case Phase::TITLE:
      UpdateTitle();
      break;
    case Phase::STAGE_SELECT:
      UpdateStageSelect();
      break;
    case Phase::END:
      UpdateFade();
      break;
  }

  fade_.Update();
  return true;
}

//描画
void Title::Draw() {
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

void Title::Finalize() {
  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();
  audio.Stop(bgm_key_);

  game::GameDevice::GetInstance()->GetDevice().WaitExecute();
}

}  // namespace scenes
}  // namespace legend