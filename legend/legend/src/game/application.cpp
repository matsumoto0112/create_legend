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

//�R���X�g���N�^
Application::Application() : main_window_(nullptr) {}

Application::~Application() {}

//�E�B���h�E�̓o�^
void Application::RegisterWindow(std::shared_ptr<window::Window> window) {
  main_window_ = window;
}

//�A�v���P�[�V�������s
void Application::Run() {
  setlocale(LC_ALL, "");
  util::debug::Assertion(main_window_ != nullptr,
                         L"�E�B���h�E���o�^����Ă��܂���B");

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

//�j��
void Application::Destroy() {}

//�`��
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

//������
bool Application::Init() {
  device_ = std::make_unique<directx::DirectX12Device>();
  if (!device_->Init(main_window_)) {
    return false;
  }

  MY_LOG(L"������");
  return true;
}

//�I������
void Application::Finalize() { MY_LOG(L"�I��"); }

//�X�V
bool Application::Update() {
  MY_LOG(L"�X�V");
  return true;
}

//�`��
bool Application::Draw() {
  MY_LOG(L"�`��");
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
