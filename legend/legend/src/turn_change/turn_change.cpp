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
            resource.GetTexture().Get(
                resource_name::texture::UI_TURNCHANGE_ENEMY),
            directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY)) {
      MY_LOG(L"前のターン画像の初期化に失敗しました。");
      return false;
    }
    if (!next_turn_sprite_.Init(
            resource.GetTexture().Get(
                resource_name::texture::UI_TURNCHANGE_PLAYER),
            directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY)) {
      MY_LOG(L"次のターン画像の初期化に失敗しました。");
      return false;
    }
  } else if (next_mode == system::Mode::ENEMY_MOVING) {
    if (!before_turn_sprite_.Init(
            resource.GetTexture().Get(
                resource_name::texture::UI_TURNCHANGE_PLAYER),
            directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY)) {
      MY_LOG(L"前のターン画像の初期化に失敗しました。");
      return false;
    }
    if (!next_turn_sprite_.Init(
            resource.GetTexture().Get(
                resource_name::texture::UI_TURNCHANGE_ENEMY),
            directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY)) {
      MY_LOG(L"次のターン画像の初期化に失敗しました。");
      return false;
    }
  }
  before_turn_sprite_.SetRect(math::Rect(0, 0, 1, 1));
  next_turn_sprite_.SetRect(math::Rect(0, 0, 1, 1));
  before_turn_sprite_.SetZOrder(0.005f);
  next_turn_sprite_.SetZOrder(0.005f);
  before_turn_sprite_.SetPosition(math::Vector2(
      screen_size_.x * 0.5f - before_turn_sprite_.GetContentSize().x * 0.5f,
      screen_size_.y * 0.5f - before_turn_sprite_.GetContentSize().y * 0.5f));
  next_turn_sprite_.SetPosition(math::Vector2(
      screen_size_.x,
      screen_size_.y * 0.5f - next_turn_sprite_.GetContentSize().y * 0.5f));
  next_turn_sprite_.SetZOrder(0.03f);
  before_turn_sprite_.SetZOrder(0.03f);
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

  const float yPos =
      screen_size_.y * 0.5f - next_turn_sprite_.GetContentSize().y * 0.5f;

  const float start_time = 0.5f;
  const float staging_time = 1.0f;
  const float before_sprite_move_start_time = 0.3f;

  if (timer_ < start_time) return true;

  //次のモード画像の移動処理
  math::Vector2 startPos = math::Vector2(screen_size_.x, yPos);
  math::Vector2 endPos = math::Vector2(
      screen_size_.x * 0.5f - next_turn_sprite_.GetContentSize().x * 0.5f,
      yPos);
  if (timer_ - start_time <= staging_time - before_sprite_move_start_time) {
    next_turn_sprite_.SetPosition(
        LerpVector2(startPos, endPos,
                    (timer_ - start_time) /
                        (staging_time - before_sprite_move_start_time)));
  } else {
    next_turn_sprite_.SetPosition(math::Vector2(endPos));
  }

  //前のモード画像の移動処理
  startPos = math::Vector2(
      screen_size_.x * 0.5f - before_turn_sprite_.GetContentSize().x * 0.5f,
      yPos);
  endPos = math::Vector2(-before_turn_sprite_.GetContentSize().x, yPos);
  if (timer_ - start_time >= before_sprite_move_start_time) {
    before_turn_sprite_.SetPosition(math::Vector2(LerpVector2(
        startPos, endPos,
        math::util::Clamp(timer_ - start_time - before_sprite_move_start_time,
                          0.0f, 1.0f) /
            (staging_time - before_sprite_move_start_time))));
  }

  //まだ終わってないか
  if (timer_ < start_time + staging_time + 0.5f) return true;

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

math::Vector2 TurnChange::LerpVector2(math::Vector2 v1, math::Vector2 v2,
                                      float t) {
  float x = math::util::Lerp(v1.x, v2.x, t);
  float y = math::util::Lerp(v1.y, v2.y, t);

  return math::Vector2(x, y);
}

}  // namespace turn_change
}  // namespace legend
