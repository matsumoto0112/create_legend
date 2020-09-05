#include "src/scenes/title.h"

#include "src/game/game_device.h"
#include "src/system/game_data.h"
#include "src/ui/image.h"
#include "src/util/resource/resource_names.h"

namespace {
namespace TextureName = legend::util::resource::resource_names::texture;
}  // namespace

namespace legend {
namespace scenes {
const std::vector<Title::Stage> Title::STAGE_LIST = {
    {
        "tutorial_01",
        TextureName::STAGESELECT_STAGE_FRAME_01,
    },
    {
        "stage_01",
        TextureName::STAGESELECT_STAGE_FRAME_02,
    },
    {
        "stage_02",
        TextureName::STAGESELECT_STAGE_FRAME_03,
    },
    //{
    //    "stage_03",
    //    TextureName::STAGESELECT_STAGE_FRAME,
    //},
};

//コンストラクタ
Title::Title(ISceneChange* scene_change) : Scene(scene_change) {}

//初期化
bool Title::Initialize() {
  current_phase_ = Phase::TITLE;
  stage_select_move_direction_ = StageSelectMoveDirection::NONE;

  const i32 stage_num = static_cast<i32>(STAGE_LIST.size());
  current_select_stage_item_id_ = util::ModInt(0, stage_num);

  game::GameDevice::GetInstance()->GetDevice().GetHeapManager().ResetLocalHeap(
      directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY);

  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();

  bgm_key_ =
      audio.Start(util::resource::resource_names::audio::BGM_TITLE, 1.0f, true);

  constexpr auto HEAP_ID =
      directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY;
  const auto screen_size =
      game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
  {
    auto image = std::make_unique<ui::Image>();
    if (!image->Init(TextureName::TITLE_LOGO, HEAP_ID)) {
      return false;
    }
    const float x =
        screen_size.x * 0.5f - image->GetSprite().GetContentSize().x * 0.5f;
    const float y = -image->GetSprite().GetContentSize().y / 2 +
                    screen_size.y / 2 - screen_size.y / 8;
    image->SetPosition(math::Vector2(x, y));
    image->SetZOrder(0.5f);
    ui::UIComponent* comp = board_.AddComponent(std::move(image));
    title_images_.emplace_back(comp);
  }
  {
    auto image = std::make_unique<ui::Image>();
    if (!image->Init(TextureName::TITLE_PUSHBUTTON_GUIDE, HEAP_ID)) {
      return false;
    }
    const float x =
        screen_size.x * 0.5f - image->GetSprite().GetContentSize().x * 0.5f;
    const float y = -image->GetSprite().GetContentSize().y / 2 +
                    screen_size.y / 2 + screen_size.y / 4;
    image->SetPosition(math::Vector2(x, y));
    image->SetZOrder(0.5f);
    ui::UIComponent* comp = board_.AddComponent(std::move(image));
    title_images_.emplace_back(comp);
  }

  {
    for (u64 i = 0; i < stage_num; i++) {
      auto image = std::make_unique<ui::Image>();
      if (!image->Init(STAGE_LIST[i].stage_image_name, HEAP_ID)) {
        return false;
      }
      //画面の中心に置いたときのx座標
      const float base_x =
          screen_size.x * 0.5f - image->GetSprite().GetContentSize().x * 0.5f;
      //画像の座標は基底の位置からスクリーン座標分ずらす
      const float x = base_x + screen_size.x * i;
      const float y =
          screen_size.y * 0.5f - image->GetSprite().GetContentSize().y * 0.5f;
      image->SetPosition(math::Vector2(x, y));
      image->SetZOrder(0.8f);
      image->SetEnable(false);
      ui::UIComponent* comp = board_.AddComponent(std::move(image));
      stage_movable_images_.emplace_back(comp, base_x,
                                         static_cast<float>(i) * screen_size.x);
    }

    {
      auto image = std::make_unique<ui::Image>();
      if (!image->Init(TextureName::STAGESELECT_ARROW_LEFT, HEAP_ID)) {
        return false;
      }
      const float x =
          screen_size.x * 0.25f - image->GetSprite().GetContentSize().x * 0.5f;
      const float y =
          screen_size.y * 0.5f - image->GetSprite().GetContentSize().y * 0.5f;
      image->SetPosition(math::Vector2(x, y));
      image->SetZOrder(0.8f);
      image->SetEnable(false);
      ui::UIComponent* comp = board_.AddComponent(std::move(image));
      stage_non_movable_images_.emplace_back(comp);
    }
    {
      auto image = std::make_unique<ui::Image>();
      if (!image->Init(TextureName::STAGESELECT_ARROW_RIGHT, HEAP_ID)) {
        return false;
      }
      const float x =
          screen_size.x * 0.75f - image->GetSprite().GetContentSize().x * 0.5f;
      const float y =
          screen_size.y * 0.5f - image->GetSprite().GetContentSize().y * 0.5f;
      image->SetPosition(math::Vector2(x, y));
      image->SetZOrder(0.8f);
      image->SetEnable(false);
      ui::UIComponent* comp = board_.AddComponent(std::move(image));
      stage_non_movable_images_.emplace_back(comp);
    }
    {
      auto image = std::make_unique<ui::Image>();
      if (!image->Init(TextureName::STAGESELECT_BACK, HEAP_ID)) {
        return false;
      }
      const float x = 0.0f;
      const float y = 0.0f;
      image->SetPosition(math::Vector2(x, y));
      image->SetZOrder(0.95f);
      board_.AddComponent(std::move(image));
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
      for (auto&& st : stage_movable_images_) {
        st.component->SetEnable(true);
      }
      for (auto&& im : stage_non_movable_images_) {
        im->SetEnable(true);
      }
    }
  };

  auto GetLerpT = [&]() {
    return 1.0f -
           (stage_move_select_timer_.CurrentTime() / STAGE_ITEM_MOVE_TIME);
  };

  auto UpdateStageItems = [&](float t) {
    for (auto&& st : stage_movable_images_) {
      const math::Vector2 prev_pos = st.component->GetPosition();
      const float x = math::util::Lerp(st.prev_x, st.next_x, t) + st.base_x;
      st.component->SetPosition(math::Vector2(x, prev_pos.y));
    }
  };

  auto SetupStageUpdate = [&]() {
    const auto screen_size =
        game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
    stage_move_select_timer_.Init(STAGE_ITEM_MOVE_TIME);
    const i32 size = static_cast<i32>(stage_movable_images_.size());
    for (i32 i = 0; i < size; i++) {
      auto& st = stage_movable_images_[i];
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
      system::GameDataStorage::GetInstance()->SetPlayStageData(
          {STAGE_LIST[current_select_stage_item_id_.Get()].stage_name});
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
    } else if (const bool input_cancel =
                   input.GetCommand(input::input_code::CANCEL);
               input_cancel) {
      current_phase_ = Phase::TITLE;
      for (auto&& im : title_images_) {
        im->SetEnable(true);
      }
      for (auto&& st : stage_movable_images_) {
        st.component->SetEnable(false);
      }
      for (auto&& im : stage_non_movable_images_) {
        im->SetEnable(false);
      }
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