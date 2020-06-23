#include "src/game/application.h"

#include <Windows.h>

#include <iostream>
#include <locale>
#include <string>
#include <typeinfo>

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
    SendMessage(main_window_->GetHWND(), WM_CLOSE, 0, 0);
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
  ;
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
  device_ = std::make_unique<directx::DirectX12Device>();
  if (!device_->Init(main_window_)) {
    return false;
  }

  MY_LOG(L"初期化");
  return true;
}

//終了処理
void Application::Finalize() { MY_LOG(L"終了"); }

//更新
bool Application::Update() {
  MY_LOG(L"更新");
  return true;
}

//描画
bool Application::Draw() {
  MY_LOG(L"描画");
  return true;
}

bool Application::FrameBegin() {
  if (!device_->Prepare()) return false;
  return true;
}

bool Application::FrameEnd() {
  if (!device_->Present()) return false;
  return true;
}

}  // namespace device
}  // namespace legend
