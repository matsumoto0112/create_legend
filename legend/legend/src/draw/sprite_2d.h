#ifndef LEGEND_DRAW_SPRITE_2D_H_
#define LEGEND_DRAW_SPRITE_2D_H_

/**
 * @file sprite_2d.h
 * @brief 2D�X�v���C�g�`��
 */

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/texture_2d.h"
#include "src/directx/constant_buffer_structure.h"

namespace legend {
namespace draw {
/**
 * @file Sprite2D
 * @brief 2D�`��N���X
 */
class Sprite2D {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  Sprite2D();
  /**
   * @brief �f�X�g���N�^
   */
  ~Sprite2D();
  /**
   * @brief ������
   * @param �t�@�C����
   */
  bool Initialize(const std::filesystem::path& filepath);

  inline math::Vector2 GetPosition() const { return position_; }
  inline void SetPosition(const math::Vector2& position) {
    this->position_ = position;
  }
  inline math::Vector2 GetScale() const { return scale_; }
  inline void SetScale(const math::Vector2& scale) { this->scale_ = scale; }
  inline float GetZOrder() const { return z_order_; }
  inline void SetZOrder(float z_order) { this->z_order_ = z_order; }
  inline float GetRotate() const { return rotate_; }
  inline void SetRotate(float rotate) { this->rotate_ = rotate; }
  void SetToCommandList();

 private:
  math::Vector2 content_size_;
  math::Vector2 scale_;
  math::Vector2 position_;
  float z_order_;
  float rotate_;
  directx::buffer::ConstantBuffer<directx::constant_buffer_structure::Transform>
      transform_constant_buffer_;

  directx::buffer::Texture2D texture_;
};
}  // namespace draw
}  // namespace legend

#endif  //! LEGEND_DRAW_SPRITE_2D_H_
