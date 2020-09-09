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
  boss_generate_turn_ = turn_system_->GetBossGenerateTurn();
  is_view_ = false;

  auto& resource = game::GameDevice::GetInstance()->GetResource();

  //ボス出現までのUIの画像を初期化
  if (!boss_generate_ui_bg_.Init(
          resource.GetTexture().Get(resource_name::texture::UI_POWERGAUGE_BG),
          directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY)) {
    MY_LOG(L"ボス出現ゲージの背景画像の初期化に失敗しました。");
    return false;
  }
  if (!player_icon_.Init(
          resource.GetTexture().Get(
              resource_name::texture::UI_POWERGAUGE_BLURRYPOINT),
          directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY)) {
    MY_LOG(L"ボス出現ゲージのプレイヤーアイコン画像の初期化に失敗しました。");
    return false;
  }
  if (!boss_icon_.Init(
          resource.GetTexture().Get(
              resource_name::texture::UI_POWERGAUGE_BLURRYPOINT),
          directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY)) {
    MY_LOG(L"ボス出現ゲージのボスアイコン画像の初期化に失敗しました。");
    return false;
  }
  //描画の優先順位を設定
  boss_generate_ui_bg_.SetZOrder(0.006f);
  player_icon_.SetZOrder(0.005f);
  boss_icon_.SetZOrder(0.005f);

  //座標を指定
  boss_generate_ui_center_position_ =
      math::Vector2(screen_size_.x / 4 * 3, screen_size_.y * 0.125f);
  boss_generate_ui_bg_.SetPosition(
      math::Vector2(boss_generate_ui_center_position_.x -
                        boss_generate_ui_bg_.GetContentSize().x * 0.5f,
                    boss_generate_ui_center_position_.y -
                        boss_generate_ui_bg_.GetContentSize().y * 0.5f));
  player_icon_.SetPosition(
      math::Vector2(boss_generate_ui_center_position_.x -
                        boss_generate_ui_bg_.GetContentSize().x * 0.5f -
                        player_icon_.GetContentSize().x * 0.5f,
                    boss_generate_ui_center_position_.y +
                        boss_generate_ui_bg_.GetContentSize().y * 0.5f -
                        player_icon_.GetContentSize().y));
  boss_icon_.SetPosition(
      math::Vector2(boss_generate_ui_center_position_.x +
                        boss_generate_ui_bg_.GetContentSize().x * 0.5f -
                        boss_icon_.GetContentSize().x / 2,
                    boss_generate_ui_center_position_.y +
                        boss_generate_ui_bg_.GetContentSize().y * 0.5f -
                        boss_icon_.GetContentSize().y));

  //移動前の座標を保存
  before_player_icon_position_ = player_icon_.GetPosition();

  return true;
}
bool TurnChange::ChangeStart(system::Mode next_mode) {
  next_mode_ = next_mode;
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  //ターン切り替え演出の各画像の初期化
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

  //移動前の座標を保存
  before_player_icon_position_ = player_icon_.GetPosition();

  //ターン切り替え時のSE再生
  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();
  if (next_mode_ == system::Mode::PLAYER_MOVE_READY) {
    audio.Start(util::resource::resource_names::audio::ENEMY_TURN_END, 1.0f);
  } else {
    audio.Start(util::resource::resource_names::audio::PLAYER_TURN_END, 1.0f);
  }

  return true;
}
bool TurnChange::TurnChangeUpdate() {
  if (!is_view_) return true;

  float delta_time =
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();

  timer_ += delta_time;

  const float yPos =
      screen_size_.y * 0.5f - next_turn_sprite_.GetContentSize().y * 0.5f;

  //ターン切り替え演出の前のターン画像が移動開始するまでの時間
  const float before_sprite_move_start_time = staging_time_ / 10 * 3;

  if (timer_ < start_time_) return true;

  //次のモード画像の移動処理
  math::Vector2 startPos = math::Vector2(screen_size_.x, yPos);
  math::Vector2 endPos = math::Vector2(
      screen_size_.x * 0.5f - next_turn_sprite_.GetContentSize().x * 0.5f,
      yPos);
  if (timer_ - start_time_ <= staging_time_ - before_sprite_move_start_time) {
    next_turn_sprite_.SetPosition(
        LerpVector2(startPos, endPos,
                    (timer_ - start_time_) /
                        (staging_time_ - before_sprite_move_start_time)));
  } else {
    next_turn_sprite_.SetPosition(math::Vector2(endPos));
  }

  //前のモード画像の移動処理
  startPos = math::Vector2(
      screen_size_.x * 0.5f - before_turn_sprite_.GetContentSize().x * 0.5f,
      yPos);
  endPos = math::Vector2(-before_turn_sprite_.GetContentSize().x, yPos);
  if (timer_ - start_time_ >= before_sprite_move_start_time) {
    before_turn_sprite_.SetPosition(math::Vector2(LerpVector2(
        startPos, endPos,
        math::util::Clamp(timer_ - start_time_ - before_sprite_move_start_time,
                          0.0f, 1.0f) /
            (staging_time_ - before_sprite_move_start_time))));
  }

  //まだ終わってないか
  if (timer_ < start_time_ + staging_time_ + before_time_) return true;

  is_view_ = false;
  turn_system_->SetTurnMode(next_mode_);
  return true;
}

bool TurnChange::BossGenerateUIUpdate() {
  math::Vector2 startPos =
      math::Vector2(boss_generate_ui_center_position_.x -
                        boss_generate_ui_bg_.GetContentSize().x * 0.5f -
                        player_icon_.GetContentSize().x * 0.5f,
                    boss_generate_ui_center_position_.y +
                        boss_generate_ui_bg_.GetContentSize().y * 0.5f -
                        player_icon_.GetContentSize().y);
  math::Vector2 endPos = math::Vector2(
      math::Vector2(boss_generate_ui_center_position_.x +
                        boss_generate_ui_bg_.GetContentSize().x * 0.5f -
                        player_icon_.GetContentSize().x * 0.5f,
                    boss_generate_ui_center_position_.y +
                        boss_generate_ui_bg_.GetContentSize().y * 0.5f -
                        player_icon_.GetContentSize().y));

  math::Vector2 nextPos = math::Vector2(
      startPos + (endPos - startPos) * (((float)turn_system_->GetCurrentTurn() /
                                         (float)boss_generate_turn_)));

  player_icon_.SetPosition(
      LerpVector2(before_player_icon_position_, nextPos,
                  timer_ / (start_time_ + staging_time_ + before_time_)));

  return true;
}
bool TurnChange::Update() {
  bool is_all_true;
  is_all_true = TurnChangeUpdate();
  is_all_true = BossGenerateUIUpdate();
  return is_all_true;
}
void TurnChange::Draw() {
  draw::SpriteRenderer& sprite_renderer =
      game::GameDevice::GetInstance()->GetSpriteRenderer();

  sprite_renderer.AddDrawItems(&boss_generate_ui_bg_);
  sprite_renderer.AddDrawItems(&player_icon_);
  sprite_renderer.AddDrawItems(&boss_icon_);

  if (!is_view_) return;
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
