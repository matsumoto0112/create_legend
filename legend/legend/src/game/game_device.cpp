#include "src/game/game_device.h"

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

  //サウンド
  audio_manager = std::make_unique<audio::AudioManager>();
  if (!audio_manager->Init()) {
    return false;
  }

  //リソース管理
  // ImGui

  window_->Show(SW_SHOW);
  return true;
}

bool GameDevice::BeginFrame() {
  fps_counter_.Update();
  if (!device_->Prepare()) {
    return false;
  }

  return true;
}

bool GameDevice::EndFrame() {
  if (!device_->Present()) {
    return false;
  }

  return true;
}

void GameDevice::Finalize() {}

// bool GameDevice::Init() {
//  // device_ = std::make_unique<directx::DirectX12Device>();
//  // if (!device_->Init(target_window)) {
//  //  return false;
//  //}
//
//  // audio_manager = std::make_unique<audio::AudioManager>();
//  // if (!audio_manager->Init()) {
//  //  return false;
//  //}
//
//  // resource_ = std::make_unique<util::resource::Resource>();
//  // if (!resource_->Init()) {
//  //  return false;
//  //}
//
//  // sprite_renderer_ = std::make_unique<draw::SpriteRenderer>();
//  // if (!sprite_renderer_->Init(
//  //        ToVector2(target_window.lock()->GetScreenSize()))) {
//  //  return false;
//  //}
//
//  // random_ = std::make_unique<util::Random>();
//
//  // const math::IntVector2 size = main_window_.lock()->GetScreenSize();
//  // if (!test_device_.Init(size.x, size.y, main_window_.lock()->GetHWND())) {
//  //  return false;
//  //}
//
//  // return true;
//
//  return true;
//}

// void GameDevice::Update() {
//  // fps_counter_.Update();
//
//  // static std::deque<float> fps = {};
//  // static float ave = 0.0f;
//  // static int cnt = 0;
//  // cnt++;
//
//  // fps.emplace_back(fps_counter_.GetFPS<float>());
//  // if (fps.size() > 20) {
//  //  fps.pop_front();
//  //}
//  // ave = std::accumulate(fps.begin(), fps.end(), 0.0f);
//  // ave /= fps.size();
//
//  // std::wstringstream wss;
//  // wss << L"Game " << ave;
//  // main_window_.lock()->SetWindowTitle(wss.str().c_str());
//  // main_window_.lock()->UpdateSettings(
//  //    window::Window::UpdateWindowSettingFlags::WindowTitle);
//
//  // test_device_.Update();
//  // test_device_.Render();
//  // input_manager_->Update();
//  // audio_manager->Update();
//}

// void GameDevice::Finalize() { test_device_.Destroy(); }

}  // namespace game
}  // namespace  legend