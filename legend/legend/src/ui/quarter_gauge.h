#ifndef LEGEND_UI_QUARTER_GAUGE_H
#define LEGEND_UI_QUARTER_GAUGE_H

/**
 * @file quarter_gauge.h
 * @brief
 */

#include "src/ui/gauge.h"

namespace legend {
namespace ui {

class QuarterGauge : public Gauge {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  QuarterGauge();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~QuarterGauge();
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
};

}  // namespace ui
}  // namespace legend
#endif  //! LEGEND_UI_QUARTER_GAUGE_H
