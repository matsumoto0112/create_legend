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

Application::~Application()
{ }

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
  this->FrameBegin();
  this->Update();
  this->Draw();
  this->FrameEnd();
}

//初期化
bool Application::Init() {
  device_ = std::make_unique<directx::DirectX12Device>();
  if (!device_->Init(main_window_)) {
    return false;
  }

  util::debug::Log(L"初期化");
  return true;
}

//終了処理
void Application::Finalize() { util::debug::Log(L"終了"); }

//更新
void Application::Update() { util::debug::Log(L"更新"); }

//描画
void Application::Draw() { util::debug::Log(L"描画"); }

void Application::FrameBegin() { device_->Prepare(); }

void Application::FrameEnd() { device_->Present(); }

}  // namespace device
}  // namespace legend
