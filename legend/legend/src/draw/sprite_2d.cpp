#include "src/draw/sprite_2d.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"

namespace {
legend::directx::constant_buffer_structure::UVRect Convert(
    const legend::math::Rect& rect) {
  return legend::directx::constant_buffer_structure::UVRect{
      rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight()};
}
}  // namespace

namespace legend {
namespace draw {

//コンストラクタ
Sprite2D::Sprite2D() {}

//デストラクタ
Sprite2D::~Sprite2D() {}

//初期化
bool Sprite2D::Init(
    std::shared_ptr<directx::buffer::Texture2D> texture,
    directx::descriptor_heap::heap_parameter::LocalHeapID cbv_heap_id) {
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  this->texture_ = texture;

  if (!transform_constant_buffer_.Init(
          device, directx::shader::ConstantBufferRegisterID::Transform,
          device.GetLocalHeapHandle(cbv_heap_id),
          L"Sprite_TransformConstantBuffer")) {
    return false;
  }
  if (!uv_rect_constant_buffer_.Init(
          device, directx ::shader::ConstantBufferRegisterID::UV_RECT,
          device.GetLocalHeapHandle(cbv_heap_id),
          L"Sprite_UVRectConstantBuffer")) {
    return false;
  }

  this->rect_ = math::Rect(0.0f, 0.0f, 1.0f, 1.0f);
  uv_rect_constant_buffer_.GetStagingRef() = Convert(rect_);
  this->content_size_ =
      math::Vector2(static_cast<float>(texture_->GetWidth()),
                    static_cast<float>(texture_->GetHeight()));
  this->scale_ = math::Vector2::kUnitVector;
  this->position_ = math::Vector2::kZeroVector;
  transform_constant_buffer_.GetStagingRef().world = math::Matrix4x4::kIdentity;

  this->scale_ = math::Vector2::kUnitVector;
  this->rotate_ = 0.0f;
  return true;
}

//コマンドリストに積む
void Sprite2D::SetToCommandList() {
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();
  texture_->SetToHeap(device);

  transform_constant_buffer_.GetStagingRef().world =
      math::Matrix4x4::CreateScale(math::Vector3(
          scale_.x * content_size_.x, scale_.y * content_size_.y, 1.0f)) *
      math::Matrix4x4::CreateRotationZ(rotate_) *
      math::Matrix4x4::CreateTranslate(
          math::Vector3(position_.x, position_.y, z_order_));
  transform_constant_buffer_.UpdateStaging();
  transform_constant_buffer_.SetToHeap(device);

  uv_rect_constant_buffer_.GetStagingRef() = Convert(rect_);
  uv_rect_constant_buffer_.UpdateStaging();
  uv_rect_constant_buffer_.SetToHeap(device);
}

}  // namespace draw
}  // namespace legend
