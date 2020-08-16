#ifndef LEGEND_UI_IMAGE_H
#define LEGEND_UI_IMAGE_H

/**
 * @file image.h
 * @brief
 */

#include "src/draw/sprite_2d.h"
#include "src/ui/ui_component.h"

namespace legend {
namespace ui {
class Image : public UIComponent {
 public:
  Image();
  virtual ~Image();

  bool Init(
      const std::wstring& name,
      directx::descriptor_heap::heap_parameter::LocalHeapID heap_id =
          directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY);
  virtual void StackOwnItems(draw::SpriteRenderer& renderer) override;
  virtual void SetPosition(const math::Vector2& position) override;
  virtual math::Vector2 GetPosition() const override;
  virtual void SetZOrder(float z) override;
  virtual float GetZOrder() const override;

 private:
  draw::Sprite2D sprite_;
};

}  // namespace ui
}  // namespace legend
#endif  //! LEGEND_UI_IMAGE_H
