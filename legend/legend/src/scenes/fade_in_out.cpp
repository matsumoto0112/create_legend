#include "src/scenes/fade_in_out.h"

#include "src/game/game_device.h"

namespace legend {
namespace scenes {

FadeInOut::FadeInOut() {}

FadeInOut::~FadeInOut() {}

bool FadeInOut::Init(
    const std::wstring& image_name,
    directx::descriptor_heap::heap_parameter::LocalHeapID heap_id) {
  if (!fade_image_.Init(
          game::GameDevice::GetInstance()->GetResource().GetTexture().Get(
              image_name),
          heap_id)) {
    return false;
  }

  const math::IntVector2 screen_size =
      game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
  const math::Vector2 content_size = fade_image_.GetContentSize();
  fade_image_.SetScale(math::Vector2(screen_size.x / content_size.x,
                                     screen_size.y / content_size.y));

  fade_image_.SetZOrder(0.1f);
  fade_type_ = FadeType::NONE;
  return true;
}

void FadeInOut::Update() {
  if (fade_type_ == FadeType::NONE) {
    return;
  }

  if (fade_timer_.Update()) {
    fade_type_ = FadeType::NONE;
  }
}

void FadeInOut::Draw() {
  const float alpha = [&]() {
    switch (fade_type_) {
      case FadeType::FADE_OUT:
        return 1.0f - fade_timer_.CurrentTime() / fade_max_time_;
      case FadeType::FADE_IN:
        return fade_timer_.CurrentTime() / fade_max_time_;
    }
    return fade_image_.GetColor().a;
  }();
  util::Color4 color = fade_image_.GetColor();
  color.a = alpha;
  fade_image_.SetColor(color);

  game::GameDevice::GetInstance()->GetSpriteRenderer().AddDrawItems(
      &fade_image_);
}

void FadeInOut::StartFadeIn(float time) {
  fade_max_time_ = time;
  fade_timer_.Init(time);
  fade_type_ = FadeType::FADE_IN;
}

void FadeInOut::StartFadeOut(float time) {
  fade_max_time_ = time;
  fade_timer_.Init(time);
  fade_type_ = FadeType::FADE_OUT;
}

bool FadeInOut::IsEnd() { return fade_timer_.IsTime(); }

}  // namespace scenes
}  // namespace legend