#include "src/scenes/title.h"

#include "src/game/game_device.h"
#include "src/ui/image.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace scenes {

//コンストラクタ
Title::Title(ISceneChange* scene_change) : Scene(scene_change) {}

//初期化
bool Title::Initialize() {
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
    board_.AddComponent(std::move(image));
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
    board_.AddComponent(std::move(image));
  }
  return true;
}

//更新
bool Title::Update() {
  auto& input = game::GameDevice::GetInstance()->GetInput();

  if (input.GetCommand(input::input_code::Decide)) {
    scene_change_->ChangeScene(SceneType::MAIN_SCENE_1);
  }
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
  game::GameDevice::GetInstance()->GetSpriteRenderer().DrawItems(command_list);
}

void Title::Finalize() {
  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();
  audio.Stop(bgm_key_);

  game::GameDevice::GetInstance()->GetDevice().WaitExecute();
}

}  // namespace scenes
}  // namespace legend