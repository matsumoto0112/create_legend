#include "src/game/application.h"

#include "src/game/game_device.h"

namespace legend {
namespace device {

//コンストラクタ
Application::Application() {}

Application::~Application() {}

//アプリケーション実行
void Application::Run() {
  setlocale(LC_ALL, "");

  if (!this->Init()) {
    Destroy();
    return;
  }

  game::GameDevice::GetInstance()->GetWindow().Show(SW_SHOW);

  tagMSG msg = {};
  while (msg.message != WM_QUIT) {
    if (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessageW(&msg);
    }
  }
  Destroy();
}

//破壊時
void Application::Destroy() {}

bool Application::BeginFrame() {
  if (!game::GameDevice::GetInstance()->BeginFrame()) {
    return false;
  }
  return true;
}

bool Application::EndFrame() {
  if (!game::GameDevice::GetInstance()->EndFrame()) {
    return false;
  }
  return true;
}

//描画
void Application::Paint() {
  auto SendCloseMessage = [&]() {
    HWND hwnd = game::GameDevice::GetInstance()->GetWindow().GetHWND();
    SendMessageW(hwnd, WM_CLOSE, 0, 0);
  };

  if (!this->BeginFrame()) {
    SendCloseMessage();
    return;
  }

  if (!this->Update()) {
    SendCloseMessage();
    return;
  }

  if (!this->Render()) {
    SendCloseMessage();
    return;
  }

  if (!this->EndFrame()) {
    SendCloseMessage();
    return;
  }
}

//初期化
bool Application::Init() {
  if (!game::GameDevice::GetInstance()->Init(this)) {
    return false;
  }
  // if (!imgui_manager_.Init(
  //        main_window_->GetHWND(),
  //        game::GameDevice::GetInstance()->GetDevice().GetDevice(),
  //        DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, 3)) {
  //  return false;
  //}

  return true;
}

//更新
bool Application::Update() {
  // game::GameDevice::GetInstance()->Update();
  // if (game::GameDevice::GetInstance()->GetInput().GetCommand(
  //        input::input_code::End)) {
  //  return false;
  //}
  return true;
}

bool Application::Render() { return true; }

////描画
// bool Application::Draw() { return true; }

// bool Application::FrameBegin() {
//  // if (!game::GameDevice::GetInstance()->GetDevice().Prepare()) return
//  false;
//  // imgui_manager_.BeginFrame();
//  return true;
//}
//
// bool Application::FrameEnd() {
//  // imgui_manager_.EndFrame(
//  //    game::GameDevice::GetInstance()->GetDevice().GetCommandList());
//  // if (!game::GameDevice::GetInstance()->GetDevice().Present()) return
//  false; return true;
//}

}  // namespace device
}  // namespace legend
