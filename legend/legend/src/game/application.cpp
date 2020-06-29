#include "src/game/application.h"

#include <Windows.h>

#include <iostream>
#include <locale>
#include <string>
#include <typeinfo>

#include "src/game/game_device.h"
#include "src/util/debug.h"
#include "src/window/window_procedure.h"

namespace legend {
namespace device {

//コンストラクタ
Application::Application() : main_window_(nullptr) {}

Application::~Application() {}

//ウィンドウの登録
void Application::RegisterWindow(std::shared_ptr<window::Window> window) {
  main_window_ = window;
}

//アプリケーション実行
void Application::Run() {
  setlocale(LC_ALL, "");
  util::debug::Assertion(main_window_ != nullptr,
                         L"ウィンドウが登録されていません。");

  main_window_->SetWindowProc(window::procedure::WindowProcdures);
  main_window_->SetWindowProcCallBack(this);
  main_window_->Create();

  if (!this->Init()) {
    Finalize();
    return;
  }

  if (!game::GameDevice::GetInstance()->GetDevice().InitAfter()) {
    Finalize();
    return;
  }
  main_window_->Show(SW_SHOW);

  tagMSG msg = {};
  while (msg.message != WM_QUIT) {
    if (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessageW(&msg);
    }
  }
  Finalize();
}

//破壊時
void Application::Destroy() {}

//描画
void Application::Paint() {
  auto SendCloseMessage = [&]() {
    SendMessage(main_window_->GetHWND(), WM_CLOSE, 0, 0);
  };

  if (!this->FrameBegin()) {
    SendCloseMessage();
    return;
  }

  if (!this->Update()) {
    SendCloseMessage();
    return;
  }

  if (!this->Draw()) {
    SendCloseMessage();
    return;
  }

  if (!this->FrameEnd()) {
    SendCloseMessage();
    return;
  }
}

//初期化
bool Application::Init() {
  if (!game::GameDevice::GetInstance()->Init(main_window_)) {
    return false;
  }
  if (!imgui_manager_.Init(
          main_window_->GetHWND(),
          game::GameDevice::GetInstance()->GetDevice().GetDevice(),
          DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, 3)) {
    return false;
  }

  return true;
}

//終了処理
void Application::Finalize() {}

//更新
bool Application::Update() {
  game::GameDevice::GetInstance()->Update();
  return true;
}

//描画
bool Application::Draw() { return true; }

bool Application::FrameBegin() {
  if (!game::GameDevice::GetInstance()->GetDevice().Prepare()) return false;
  imgui_manager_.BeginFrame();
  return true;
}

bool Application::FrameEnd() {
  imgui_manager_.EndFrame(
      game::GameDevice::GetInstance()->GetDevice().GetCommandList());
  if (!game::GameDevice::GetInstance()->GetDevice().Present()) return false;
  return true;
}

}  // namespace device
}  // namespace legend
