#include "src/ui/image.h"

#include "src/game/game_device.h"

namespace legend {
namespace ui {

Image::Image() {}

Image::~Image() {}

bool Image::Init(
    const std::wstring& name,
    directx::descriptor_heap::heap_parameter::LocalHeapID heap_id) {
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  return sprite_.Init(resource.GetTexture().Get(name), heap_id);
}

void Image::StackOwnItems(draw::SpriteRenderer& renderer) {
  renderer.AddDrawItems(&sprite_);
}

void Image::SetPosition(const math::Vector2& position) {
  sprite_.SetPosition(position);
}

math::Vector2 Image::GetPosition() const { return sprite_.GetPosition(); }

void Image::SetZOrder(float z) { sprite_.SetZOrder(z); }

float Image::GetZOrder() const { return sprite_.GetZOrder(); }

}  // namespace ui
}  // namespace legend