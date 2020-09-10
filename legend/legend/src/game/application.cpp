#include "src/game/application.h"

#include "src/game/game_device.h"

namespace legend {
namespace device {

//�R���X�g���N�^
Application::Application() {}

Application::~Application() {}

//�A�v���P�[�V�������s
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
}

//�j��
void Application::Destroy() { game::GameDevice::GetInstance()->Finalize(); }

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

//�`��
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

  if (!this->LateUpdate()) {
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

//������
bool Application::Init() {
  if (!game::GameDevice::GetInstance()->Init(this)) {
    return false;
  }

  game::GameDevice::GetInstance()->GetAudioManager().SetMasterVolume(0.25f);
  game::GameDevice::GetInstance()->GetAudioManager().SetBGMVolume(0.5f);
  return true;
}

//�X�V
bool Application::Update() {
  if (game::GameDevice::GetInstance()->GetInput().GetCommand(
          input::input_code::End)) {
    return false;
  }
  return true;
}

bool Application::LateUpdate() {
  if (!game::GameDevice::GetInstance()->MidFrame()) {
    return false;
  }
  return true;
}

bool Application::Render() { return true; }

}  // namespace device
}  // namespace legend
