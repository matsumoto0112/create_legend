#include "src/draw/sprite_2d.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"

namespace {

using UVRect = legend::directx::buffer::constant_buffer_structure::UVRect;
using Rect = legend::math::Rect;
using ConstantBufferID =
    legend::directx::shader::ConstantBufferRegisterID::Enum;

/**
 * @brief RectからUVRectに変換する
 */
UVRect Convert(const Rect& rect) {
  return UVRect{rect.GetLeft(), rect.GetTop(), rect.GetWidth(),
                rect.GetHeight()};
}

/**
 * @brief UVRectからRectに変換する
 */
Rect Convert(const UVRect& rect) {
  return Rect{rect.left, rect.top, rect.width, rect.height};
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
  auto& device = game::GameDevice::GetInstance()->GetDevice();

  this->texture_ = texture;

  if (!transform_constant_buffer_.Init(device, ConstantBufferID::TRANSFORM,
                                       device.GetLocalHandle(cbv_heap_id),
                                       L"Sprite_TransformConstantBuffer")) {
    return false;
  }
  if (!uv_rect_constant_buffer_.Init(device, ConstantBufferID::UV_RECT,
                                     device.GetLocalHandle(cbv_heap_id),
                                     L"Sprite_UVRectConstantBuffer")) {
    return false;
  }

  uv_rect_constant_buffer_.GetStagingRef() =
      Convert(math::Rect(0.0f, 0.0f, 1.0f, 1.0f));
  this->content_size_ =
      math::Vector2(static_cast<float>(texture_->GetWidth()),
                    static_cast<float>(texture_->GetHeight()));
  this->scale_ = math::Vector2::kUnitVector;
  this->position_ = math::Vector2::kZeroVector;
  transform_constant_buffer_.GetStagingRef().world = math::Matrix4x4::kIdentity;

  this->scale_ = math::Vector2::kUnitVector;
  this->z_order_ = 0.0f;
  this->rotate_ = 0.0f;
  return true;
}

// UV矩形を設定する
void Sprite2D::SetRect(const math::Rect& rect) {
  uv_rect_constant_buffer_.GetStagingRef() = Convert(rect);
}

// UV矩形を取得する
math::Rect Sprite2D::GetRect() const {
  return Convert(uv_rect_constant_buffer_.GetStaging());
}

//コマンドリストに積む
void Sprite2D::SetToGraphicsCommandList(
    directx::device::CommandList& command_list) {
  auto& device = game::GameDevice::GetInstance()->GetDevice();

  texture_->SetToHeap(device);

  // 2Dカメラに映ることを考慮したワールド行列を作成する
  transform_constant_buffer_.GetStagingRef().world =
      math::Matrix4x4::CreateScale(math::Vector3(
          scale_.x * content_size_.x, scale_.y * content_size_.y, 1.0f)) *
      math::Matrix4x4::CreateRotationZ(rotate_) *
      math::Matrix4x4::CreateTranslate(
          math::Vector3(position_.x, position_.y, z_order_));
  transform_constant_buffer_.UpdateStaging();
  transform_constant_buffer_.SetToHeap(device);

  uv_rect_constant_buffer_.UpdateStaging();
  uv_rect_constant_buffer_.SetToHeap(device);
}

}  // namespace draw
}  // namespace legend
