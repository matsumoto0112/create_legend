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

//�j��
void Application::Destroy() {}

//�`��
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

//������
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

//�I������
void Application::Finalize() {}

//�X�V
bool Application::Update() {
  game::GameDevice::GetInstance()->Update();
  return true;
}

//�`��
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
