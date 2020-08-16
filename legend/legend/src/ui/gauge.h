#ifndef LEGEND_UI_GAUGE_H
#define LEGEND_UI_GAUGE_H

/**
 * @file gauge.h
 * @brief UI�Q�[�W�N���X��`
 */

#include "src/ui/ui_component.h"

namespace legend {
namespace ui {

/**
 * @brief UI�Q�[�W�N���X
 */
class Gauge : public UIComponent {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  Gauge();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~Gauge();
  /**
   * @brief ������
   * @param name �摜��
   * @param heap_id �g�p����q�[�vID
   */
  bool Init(
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
   * @brief �Q�[�W�̒l��0~1�Őݒ肷��
   */
  void SetValue(float border);
  /**
   * @brief �Q�[�W�̒l���擾����
   */
  float GetValue() const;

 protected:
  //! �`�悷��X�v���C�g
  draw::Sprite2D sprite_;

  /**
   * @brief �Q�[�W�̕`��p�p�����[�^
   */
  struct GaugeParameter {
    float value;
  };
  GaugeParameter param_;
  //! �R���X�^���g�o�b�t�@
  directx::buffer::ConstantBuffer<GaugeParameter> constant_buffer_;
};

}  // namespace ui
}  // namespace legend

#endif  //! LEGEND_UI_GAUGE_H
