#include "src/draw/sprite_2d.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"

namespace legend {
namespace draw {

//コンストラクタ
Sprite2D::Sprite2D() {}

//デストラクタ
Sprite2D::~Sprite2D() {}

//初期化
bool Sprite2D::Init(const std::filesystem::path& filepath) {
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();
  if (!texture_.InitAndWrite(device, directx::shader::TextureRegisterID::Albedo,
                             filepath)) {
    return false;
  }
  if (!transform_constant_buffer_.Init(
          device, directx::shader::ConstantBufferRegisterID::Transform,
          L"Sprite_TransformConstantBuffer")) {
    return false;
  }

  this->content_size_ = math::Vector2(static_cast<float>(texture_.GetWidth()),
                                      static_cast<float>(texture_.GetHeight()));
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
  texture_.SetToHeap(device);

  transform_constant_buffer_.GetStagingRef().world =
      math::Matrix4x4::CreateScale(math::Vector3(
          scale_.x * content_size_.x, scale_.y * content_size_.y, 1.0f)) *
      math::Matrix4x4::CreateRotationZ(rotate_) *
      math::Matrix4x4::CreateTranslate(
          math::Vector3(position_.x, position_.y, z_order_));
  transform_constant_buffer_.UpdateStaging();
  transform_constant_buffer_.SetToHeap(device);
}

}  // namespace draw
}  // namespace legend
