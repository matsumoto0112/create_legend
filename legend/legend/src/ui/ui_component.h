#ifndef LEGEND_UI_UI_COMPONENT_H
#define LEGEND_UI_UI_COMPONENT_H

/**
 * @file ui_component.h
 * @brief UI�R���|�[�l���g��`
 */

#include "src/draw/sprite_renderer.h"

namespace legend {
namespace ui {

/**
 * @class UIComponent
 * @brief UI�R���|�[�l���g
 */
class UIComponent {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  UIComponent() : enable_(true){};
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~UIComponent() = default;
  /**
   * @brief ���W��ݒ肷��
   */
  virtual void SetPosition(const math::Vector2& position) = 0;
  /**
   * @brief ���W���擾����
   */
  virtual math::Vector2 GetPosition() const = 0;
  /**
   * @brief Z-Order��ݒ肷��
   */
  virtual void SetZOrder(float z) = 0;
  /**
   * @brief Z-Order���擾����
   */
  virtual float GetZOrder() const = 0;
  /**
   * @brief ���g�̕`��A�C�e����`�惊�X�g�ɒǉ�����
   */
  virtual void StackOwnItems(draw::SpriteRenderer& renderer) = 0;

  virtual void SetEnable(bool enable) { this->enable_ = enable; }

  virtual bool GetEnable() const { return this->enable_; }

 protected:
  //! UI���`�悳��邩�ǂ���
  bool enable_;
};

}  // namespace ui
}  // namespace legend

#endif  //! LEGEND_UI_UI_COMPONENT_H
