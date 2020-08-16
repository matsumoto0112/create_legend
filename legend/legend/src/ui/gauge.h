#ifndef LEGEND_UI_GAUGE_H
#define LEGEND_UI_GAUGE_H

/**
 * @file gauge.h
 * @brief
 */

#include "src/ui/ui_component.h"

namespace legend {
namespace ui {
class Gauge : public UIComponent {
 public:
  Gauge();
  ~Gauge();
  bool Init(
      const std::wstring& name,
      directx::descriptor_heap::heap_parameter::LocalHeapID heap_id =
          directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY);
  virtual void SetPosition(const math::Vector2& position) override;
  virtual math::Vector2 GetPosition() const override;
  virtual void SetZOrder(float z) override;
  virtual float GetZOrder() const override;
  virtual void StackOwnItems(draw::SpriteRenderer& renderer) override;
  void SetValue(float border);
  float GetValue() const;

 private:
  draw::Sprite2D sprite_;
  struct GaugeParameter {
    float value;
  };
  GaugeParameter param_;
  directx::buffer::ConstantBuffer<GaugeParameter> constant_buffer_;
};

}  // namespace ui
}  // namespace legend
#endif  //! LEGEND_UI_GAUGE_H
