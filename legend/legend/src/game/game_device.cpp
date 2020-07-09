#include "src/game/game_device.h"

namespace legend {
namespace game {
GameDevice::GameDevice()
    : fps_counter_{}, device_(nullptr), input_manager_(nullptr) {}
GameDevice::~GameDevice() {}
bool GameDevice::Init(std::shared_ptr<window::Window> target_window) {
  device_ = std::make_unique<directx::DirectX12Device>();
  if (!device_->Init(target_window)) {
    return false;
  }

  input_manager_ =
      std::make_unique<input::InputManager>(target_window->GetHWND());

  return true;
}

void GameDevice::Update() {
  fps_counter_.Update();
  input_manager_->Update();
}

}  // namespace game
}  // namespace  legend