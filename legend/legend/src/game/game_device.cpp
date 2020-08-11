#include "src/game/game_device.h"

#include "src/directx/device/device_option.h"
#include "src/window/window_procedure.h"

namespace {
constexpr legend::u32 WINDOW_WIDTH = 1280;
constexpr legend::u32 WINDOW_HEIGHT = 720;

}  // namespace

namespace legend {
namespace game {
GameDevice::GameDevice() : fps_counter_{} {}

GameDevice::~GameDevice() {}

bool GameDevice::Init(window::IWindowProcedureEventCallback* callback) {
  //作るオブジェクトは
  window_ = std::make_unique<window::Window>();
  //ウィンドウ
  window_->SetScreenSize(math::IntVector2(WINDOW_WIDTH, WINDOW_HEIGHT));
  window_->SetWindowTitle(L"Game");
  window_->SetWindowProc(window::procedure::WindowProcdures);
  window_->SetWindowProcCallBack(callback);
  window_->Create();
  // DirectX12デバイス

  device_ = std::make_unique<directx::device::DirectXDevice>();
  if (!device_->Init(WINDOW_WIDTH, WINDOW_HEIGHT, window_->GetHWND())) {
    return false;
  }

  //入力
  input_manager_ = std::make_unique<input::InputManager>(window_->GetHWND());
  random_ = std::make_unique<util::Random>();

  //サウンド
  audio_manager = std::make_unique<audio::AudioManager>();
  if (!audio_manager->Init()) {
    return false;
  }

  //スプライト描画
  const math::Vector2 screen_size =
      math::Vector2(static_cast<float>(window_->GetScreenSize().x),
                    static_cast<float>(window_->GetScreenSize().y));
  if (!sprite_renderer_.Init(screen_size)) {
    return false;
  }

  //リソース管理
  if (!resource_.Init()) {
    return false;
  }

  if (!particle_manager_.Init(GetDevice(),
                              directx::device::defines::FRAME_COUNT)) {
    return false;
  }

  // ImGui
  if (!imgui_manager_.Init(window_->GetHWND(), device_->GetDevice(),
                           DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
                           directx::device::defines::FRAME_COUNT, true)) {
    return false;
  }

  return true;
}

bool GameDevice::BeginFrame() {
  fps_counter_.Update();

  imgui_manager_.BeginFrame();
  input_manager_->Update();
  audio_manager->Update();

  if (!device_->Prepare()) {
    return false;
  }
  particle_manager_.BeginFrame(GetDevice());

  return true;
}

bool GameDevice::EndFrame() {
  imgui_manager_.EndFrame(
      device_->GetCurrentFrameResource()->GetCommandList().GetCommandList());
  if (!device_->Present()) {
    return false;
  }

  return true;
}

void GameDevice::Finalize() { device_->Destroy(); }

}  // namespace game
}  // namespace  legend