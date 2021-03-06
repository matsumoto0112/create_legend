#include "src/scenes/mainscene/main_scene_1.h"

#include "src/game/game_device.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace scenes {
namespace mainscene {

//コンストラクタ
MainScene1::MainScene1(ISceneChange* scene_change) : Scene(scene_change) {}

//デストラクタ
MainScene1::~MainScene1() {}

//初期化
bool MainScene1::Initialize() {
  const auto play_stage_data =
      system::GameDataStorage::GetInstance()->GetPlayStageData();

  if (!turn_system_.Init(play_stage_data.stage_name)) {
    return false;
  }

  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();
  bgm_ =
      audio.Start(util::resource::resource_names::audio::BGM_MAIN, 0.25f, true);

  return true;
}

//更新
bool MainScene1::Update() {
  if (!turn_system_.Update()) {
    return false;
  }

  if (turn_system_.IsGameEnd()) {
    const auto data = turn_system_.GetResult();
    system::GameDataStorage::GetInstance()->SetResultData(data);
    scene_change_->ChangeScene(SceneType::GAMEOVER);
  }
  return true;
}

//描画
void MainScene1::Draw() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
  auto& render_resource_manager = device.GetRenderResourceManager();

  ////まずは通常のモデル描画
  // render_resource_manager.SetRenderTargets(
  //    command_list, directx::render_target::RenderTargetID::BACK_BUFFER, true,
  //    directx::render_target::DepthStencilTargetID::DEPTH_ONLY, true);

  turn_system_.Draw();

  if (util::debug::ENABLE_DEBUG) {
    //次にコリジョンなどのデバッグ描画
    render_resource_manager.SetRenderTargets(
        command_list, directx::render_target::RenderTargetID::BACK_BUFFER,
        false, directx::render_target::DepthStencilTargetID::NONE, false);
    turn_system_.DebugDraw();
  }
}

//終了
void MainScene1::Finalize() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();
  audio.Stop(bgm_);

  device.WaitExecute();
}

//ターン別の更新処理
bool MainScene1::UpdateTurn() { return true; }
}  // namespace mainscene
}  // namespace scenes
}  // namespace legend