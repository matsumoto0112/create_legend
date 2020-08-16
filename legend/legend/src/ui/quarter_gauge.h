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
   * @brief コンストラクタ
   */
  QuarterGauge();
  /**
   * @brief デストラクタ
   */
  virtual ~QuarterGauge();
  /**
   * @brief 初期化
   * @param name 画像名
   * @param heap_id 使用するヒープID
   */
  bool Init(
      const std::wstring& name,
      directx::descriptor_heap::heap_parameter::LocalHeapID heap_id =
          directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY);
  /**
   * @brief 描画リストに追加する
   */
  virtual void StackOwnItems(draw::SpriteRenderer& renderer) override;
};

}  // namespace ui
}  // namespace legend
#endif  //! LEGEND_UI_QUARTER_GAUGE_H
