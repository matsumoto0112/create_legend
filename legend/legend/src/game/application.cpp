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

Application::~Application()
{ }

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
  this->FrameBegin();
  this->Update();
  this->Draw();
  this->FrameEnd();
}

//������
bool Application::Init() {
  device_ = std::make_unique<directx::DirectX12Device>();
  if (!device_->Init(main_window_)) {
    return false;
  }

  util::debug::Log(L"������");
  return true;
}

//�I������
void Application::Finalize() { util::debug::Log(L"�I��"); }

//�X�V
void Application::Update() { util::debug::Log(L"�X�V"); }

//�`��
void Application::Draw() { util::debug::Log(L"�`��"); }

void Application::FrameBegin() { device_->Prepare(); }

void Application::FrameEnd() { device_->Present(); }

}  // namespace device
}  // namespace legend
