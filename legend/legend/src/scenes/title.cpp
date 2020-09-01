#include "src/scenes/title.h"

#include "src/game/game_device.h"
#include "src/ui/image.h"
#include "src/util/resource/resource_names.h"

namespace {
constexpr const wchar_t* STAGE_LIST[] = {
    legend::util::resource::resource_names::texture::RESULT_GAMEOVER,
    legend::util::resource::resource_names::texture::RESULT_STAGECLEAR,
};
}  // namespace

namespace legend {
namespace scenes {

//コンストラクタ
Title::Title(ISceneChange* scene_change) : Scene(scene_change) {}

//初期化
bool Title::Initialize() {
  current_phase_ = Phase::TITLE;
  stage_select_move_direction_ = StageSelectMoveDirection::NONE;

  game::GameDevice::GetInstance()->GetDevice().GetHeapManager().ResetLocalHeap(
      directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY);

  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();

  bgm_key_ =
      audio.Start(util::resource::resource_names::audio::BGM_TITLE, 1.0f, true);

  {
    auto image = std::make_unique<ui::Image>();
    if (!image->Init(
            util::resource::resource_names::texture::TITLE_LOGO,
            directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY)) {
      return false;
    }

    const auto screen_size =
        game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
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

    const auto screen_size =
        game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
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

      const auto screen_size =
          game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
      const float base_x =
          screen_size.x * 0.5f - image->GetSprite().GetContentSize().x * 0.5f;
      const float x = base_x + screen_size.x * i;
      const float y = image->GetSprite().GetContentSize().y * 0.5f + 200.0f;
      image->SetPosition(math::Vector2(x, y));
      image->SetZOrder(0.2f);
      image->SetEnable(false);
      ui::UIComponent* comp = board_.AddComponent(std::move(image));
      stage_images_.emplace_back(comp, base_x);
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

  auto UpdateStageSelect = [&]() {
    // if (stage_select_move_direction_ == StageSelectMoveDirection::LEFT) {
    //  stage_select_move_direction_ = StageSelectMoveDirection::NONE;
    //  return;
    //} else if (stage_select_move_direction_ ==
    //           StageSelectMoveDirection::RIGHT) {
    //  for (auto&& im : stage_images_) {
    //    const float base_x = im.base_x;
    //  }
    //  stage_select_move_direction_ = StageSelectMoveDirection::NONE;
    //  return;
    //}
    //決定キーでフェードを開始し、シーンを終了する
    if (input.GetCommand(input::input_code::Decide)) {
      fade_.StartFadeOut(1.0f);
      is_scene_end_ = true;
      audio.Start(util::resource::resource_names::audio::TITLE_DECISION, 1.0f);
      current_phase_ = Phase::END;
    } else if (input.GetHorizontal() > 0.0f) {
      stage_select_move_direction_ = StageSelectMoveDirection::RIGHT;
    } else if (input.GetHorizontal() < 0.0f) {
      stage_select_move_direction_ = StageSelectMoveDirection::LEFT;
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