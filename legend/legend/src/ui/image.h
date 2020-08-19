#ifndef LEGEND_UI_IMAGE_H
#define LEGEND_UI_IMAGE_H

/**
 * @file image.h
 * @brief UI�C���[�W�N���X��`
 */

#include "src/draw/sprite_2d.h"
#include "src/ui/ui_component.h"

namespace legend {
namespace ui {

/**
 * @brief UI�C���[�W�N���X
 */
class Image : public UIComponent {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  Image();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~Image();
  /**
   * @brief ������
   * @param name �摜��
   * @param heap_id �g�p����q�[�vID
   */
  virtual bool Init(
      const std::wstring& name,
      directx::descriptor_heap::heap_parameter::LocalHeapID heap_id =
          directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY);
  /**
   * @brief �`�惊�X�g�ɒǉ�����
   */
  virtual void StackOwnItems(draw::SpriteRenderer& renderer) override;
  /**
   * @brief ���W��ݒ肷��
   */
  virtual void SetPosition(const math::Vector2& position) override;
  /**
   * @brief ���W���擾����
   */
  virtual math::Vector2 GetPosition() const override;
  /**
   * @brief Z-Order��ݒ肷��
   */
  virtual void SetZOrder(float z) override;
  /**
   * @brief Z-Order���擾����
   */
  virtual float GetZOrder() const override;
  /**
   * @brief �X�v���C�g���擾����
   */
  virtual draw::Sprite2D& GetSprite() { return sprite_; }

 protected:
  //! �`�悷��X�v���C�g
  draw::Sprite2D sprite_;
};

}  // namespace ui
}  // namespace legend
#endif  //! LEGEND_UI_IMAGE_H
