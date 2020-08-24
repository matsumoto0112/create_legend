#ifndef LEGEND_SCENES_FADE_IN_OUT_H
#define LEGEND_SCENES_FADE_IN_OUT_H

/**
 * @file fade_in_out.h
 * @brief
 */

#include "src/draw/sprite_2d.h"
#include "src/util/timer.h"

namespace legend {
namespace scenes {
class FadeInOut {
 public:
  FadeInOut();
  virtual ~FadeInOut();
  bool Init(
      const std::wstring& image_name,
      directx::descriptor_heap::heap_parameter::LocalHeapID heap_id =
          directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY);
  void Update();
  void Draw();
  void StartFadeIn(float time);
  void StartFadeOut(float time);
  bool IsEnd();

 private:
  enum class FadeType {
    NONE,
    FADE_IN,
    FADE_OUT,
  };
  draw::Sprite2D fade_image_;
  float fade_max_time_;
  util::CountDownTimer fade_timer_;
  FadeType fade_type_;
};

}  // namespace scenes
}  // namespace legend
#endif  //! LEGEND_SCENES_FADE_IN_OUT_H
