#include "src/scenes/game_over.h"

#include "src/game/game_device.h"
#include "src/system/game_data.h"
#include "src/ui/image.h"
#include "src/ui/number.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace scenes {

//コンストラクタ
GameOver::GameOver(ISceneChange* scene_change) : Scene(scene_change) {}

GameOver::~GameOver() {}

//初期化
bool GameOver::Initialize() {
  namespace TextureName = util::resource::resource_names::texture;

  const auto window_size =
      game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
  const system::GameDataStorage::GameData data =
      system::GameDataStorage::GetInstance()->Get();
  const auto heap_id =
      directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY;

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
  }

  // const i32 clear_turn = data.play_turn;
  // std::stringstream ss;
  // ss << std::setw(2) << clear_turn;
  // for (auto&& c : ss.str()) {
  //  auto num = std::make_unique<ui::Number>();
  //  num->Init(TextureName::UI_NUMBER_1, heap_id);
  //}

  fade_.Init(util::resource::resource_names::texture::FADE_IMAGE);
  fade_.StartFadeIn(1.0f);
  is_scene_end_ = false;
  return true;
}

//更新
bool GameOver::Update() {
  auto& input = game::GameDevice::GetInstance()->GetInput();

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
  }

  fade_.Update();
  return true;
}

//描画
void GameOver::Draw() {
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

void GameOver::Finalize() {
  game::GameDevice::GetInstance()->GetDevice().WaitExecute();
}

}  // namespace scenes
}  // namespace legend