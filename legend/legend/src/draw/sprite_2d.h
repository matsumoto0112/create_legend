#ifndef LEGEND_DRAW_SPRITE_2D_H_
#define LEGEND_DRAW_SPRITE_2D_H_

/**
 * @file sprite_2d.h
 * @brief 2D�X�v���C�g�`��
 */

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/texture_2d.h"
#include "src/directx/constant_buffer_structure.h"
#include "src/math/rect.h"

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
   * @param texture �g�p����e�N�X�`��
   */
  bool Init(std::shared_ptr<directx::buffer::Texture2D> texture,
            directx::descriptor_heap::heap_parameter::LocalHeapID cbv_heap_id);

 public:
  /**
   * @brief �e�N�X�`���̑傫�����擾����
   */
  inline math::Vector2 GetContentSize() const { return content_size_; }
  /**
   * @brief ���W�̐ݒ�
   */
  inline void SetPosition(const math::Vector2& position) {
    this->position_ = position;
  }
  /**
   * @brief ���W�̎擾
   */
  inline math::Vector2 GetPosition() const { return position_; }
  /**
   * @brief �X�P�[�����O�̐ݒ�
   */
  inline void SetScale(const math::Vector2& scale) { this->scale_ = scale; }
  /**
   * @brief �X�P�[�����O�̎擾
   */
  inline math::Vector2 GetScale() const { return scale_; }
  /**
   * @brief Z�I�[�_�[��ݒ肷��
   * @param z_order �ݒ肷��l(0~1)
   */
  inline void SetZOrder(float z_order) { this->z_order_ = z_order; }
  /**
   * @brief Z�I�[�_�[���擾����
   */
  inline float GetZOrder() const { return z_order_; }
  /**
   * @brief ��]�p�x��ݒ肷��
   * @param rotate �ݒ肷��p�x(rad)
   */
  inline void SetRotate(float rotate) { this->rotate_ = rotate; }
  /**
   * @brief ��]�p�x���擾����
=  */
  inline float GetRotate() const { return rotate_; }

  inline void SetRect(const math::Rect& rect) { this->rect_ = rect; }
  inline const math::Rect& GetRect() const { return rect_; }
  /**
   * @brief �R�}���h���X�g�ɐς�
   */
  void SetToCommandList();

 protected:
  //! �R���e���c�̃T�C�Y
  math::Vector2 content_size_;
  //! �X�P�[�����O
  math::Vector2 scale_;
  //! ���W
  math::Vector2 position_;
  //! Z�I�[�_�[
  float z_order_;
  //! ��]�p�x
  float rotate_;
  //! �g�����X�t�H�[���s��o�b�t�@
  directx::buffer::ConstantBuffer<directx::constant_buffer_structure::Transform>
      transform_constant_buffer_;
  //! �e�N�X�`��
  std::shared_ptr<directx::buffer::Texture2D> texture_;
  math::Rect rect_;
  directx::buffer::ConstantBuffer<directx::constant_buffer_structure::UVRect>
      uv_rect_constant_buffer_;
};
}  // namespace draw
}  // namespace legend

#endif  //! LEGEND_DRAW_SPRITE_2D_H_
