#include "src/ui/image.h"

#include "src/game/game_device.h"

namespace legend {
namespace ui {

//コンストラクタ
Image::Image() {}

//デストラクタ
Image::~Image() {}

//初期化
bool Image::Init(
    const std::wstring& name,
    directx::descriptor_heap::heap_parameter::LocalHeapID heap_id) {
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  return sprite_.Init(resource.GetTexture().Get(name), heap_id);
}

//描画リストに追加
void Image::StackOwnItems(draw::SpriteRenderer& renderer) {
  renderer.AddDrawItems(&sprite_);
}

//座標を設定する
void Image::SetPosition(const math::Vector2& position) {
  sprite_.SetPosition(position);
}

//座標を取得する
math::Vector2 Image::GetPosition() const { return sprite_.GetPosition(); }

// Z-Orderを設定する
void Image::SetZOrder(float z) { sprite_.SetZOrder(z); }

// Z-Orderを取得する
float Image::GetZOrder() const { return sprite_.GetZOrder(); }

}  // namespace ui
}  // namespace legend