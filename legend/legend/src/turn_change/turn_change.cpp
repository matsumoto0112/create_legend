#include "turn_change.h"

#include "src/audio/audio_manager.h"
#include "src/game/game_device.h"
#include "src/system/turn_system.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace turn_change {
namespace resource_name = util::resource::resource_names;
TurnChange::TurnChange() { is_view_ = false; }
TurnChange::~TurnChange() {}
bool TurnChange::Init(system::TurnSystem* turn_system) {
  screen_size_ = math::Vector2(
      static_cast<float>(
          game::GameDevice::GetInstance()->GetWindow().GetScreenSize().x),
      static_cast<float>(
          game::GameDevice::GetInstance()->GetWindow().GetScreenSize().y));
  turn_system_ = turn_system;
  is_view_ = false;
  return true;
}
bool TurnChange::ChangeStart(system::Mode next_mode) {
  next_mode_ = next_mode;
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  //各画像の初期化
  if (next_mode == system::Mode::PLAYER_MOVE_READY) {
    if (!before_turn_sprite_.Init(
            resource.GetTexture().Get(resource_name::texture::TITLE_LOGO),
            directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID)) {
      MY_LOG(L"前のターン画像の初期化に失敗しました。");
      return false;
    }
    if (!next_turn_sprite_.Init(
            resource.GetTexture().Get(resource_name::texture::TITLE_LOGO),
            directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID)) {
      MY_LOG(L"次のターン画像の初期化に失敗しました。");
      return false;
    }
  } else if (next_mode == system::Mode::ENEMY_MOVING) {
    if (!before_turn_sprite_.Init(
            resource.GetTexture().Get(resource_name::texture::TITLE_LOGO),
            directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID)) {
      MY_LOG(L"前のターン画像の初期化に失敗しました。");
      return false;
    }
    if (!next_turn_sprite_.Init(
            resource.GetTexture().Get(resource_name::texture::TITLE_LOGO),
            directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID)) {
      MY_LOG(L"次のターン画像の初期化に失敗しました。");
      return false;
    }
  }
  before_turn_sprite_.SetRect(math::Rect(0, 0, 1, 1));
  next_turn_sprite_.SetRect(math::Rect(0, 0, 1, 1));
  before_turn_sprite_.SetPosition(
      math::Vector2(screen_size_.x * 0.4f, screen_size_.y / 2));
  next_turn_sprite_.SetPosition(
      math::Vector2(screen_size_.x, screen_size_.y / 2));

  timer_ = 0.0f;
  is_view_ = true;

  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();
  if (next_mode_ == system::Mode::PLAYER_MOVE_READY) {
    audio.Start(util::resource::resource_names::audio::ENEMY_TURN_END, 1.0f);
  } else {
    audio.Start(util::resource::resource_names::audio::PLAYER_TURN_END, 1.0f);
  }

  return true;
}
bool TurnChange::Update() {
  if (!is_view_) return true;

  float delta_time =
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();

  timer_ += delta_time;

  //次のモード画像の移動処理
  if (timer_ <= 0.6f) {
    next_turn_sprite_.SetPosition(
        math::Vector2(screen_size_.x - screen_size_.x * 0.6f * (timer_ / 0.6f),
                      screen_size_.y / 2));
  } else {
    next_turn_sprite_.SetPosition(
        math::Vector2(screen_size_.x * 0.4f, screen_size_.y / 2));
  }

  //前のモード画像の移動処理
  if (timer_ >= 0.4f) {
    before_turn_sprite_.SetPosition(
        math::Vector2(screen_size_.x * 0.4f -
                          screen_size_.x * 0.6f * ((timer_ - 0.4f) / 0.6f),
                      screen_size_.y / 2));
  }

  //まだ終わってないか
  if (timer_ < 1.0f) return true;

  is_view_ = false;
  turn_system_->SetTurnMode(next_mode_);
  return true;
}
void TurnChange::Draw() {
  if (!is_view_) return;
  draw::SpriteRenderer& sprite_renderer =
      game::GameDevice::GetInstance()->GetSpriteRenderer();
  sprite_renderer.AddDrawItems(&before_turn_sprite_);
  sprite_renderer.AddDrawItems(&next_turn_sprite_);
}

}  // namespace turn_change
}  // namespace legend
