#include "src/game/game_device.h"

namespace {
/**
 * @brief IntVector2‚©‚çVector2‚É•ÏŠ·‚·‚é
 */
legend::math::Vector2 ToVector2(const legend::math::IntVector2& int_vec2) {
  return legend::math::Vector2(static_cast<float>(int_vec2.x),
                               static_cast<float>(int_vec2.y));
}
}  // namespace

namespace legend {
namespace game {
GameDevice::GameDevice()
    : fps_counter_{}, device_(nullptr), input_manager_(nullptr) {}
GameDevice::~GameDevice() {}
bool GameDevice::Init(std::shared_ptr<window::Window> target_window) {
  this->main_window_ = target_window;
  device_ = std::make_unique<directx::DirectX12Device>();
  if (!device_->Init(target_window)) {
    return false;
  }

  input_manager_ =
      std::make_unique<input::InputManager>(target_window->GetHWND());

  audio_manager = std::make_unique<audio::AudioManager>();
  if (!audio_manager->Init()) {
    return false;
  }

  sprite_renderer_ = std::make_unique<draw::SpriteRenderer>();
  if (!sprite_renderer_->Init(ToVector2(target_window->GetScreenSize()))) {
    return false;
  }

  random_ = std::make_unique<util::Random>();
  return true;
}

void GameDevice::Update() {
  fps_counter_.Update();
  input_manager_->Update();
  audio_manager->Update();
}

}  // namespace game
}  // namespace  legend