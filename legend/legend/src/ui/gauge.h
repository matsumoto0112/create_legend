#ifndef LEGEND_UI_GAUGE_H
#define LEGEND_UI_GAUGE_H

/**
 * @file gauge.h
 * @brief UIゲージクラス定義
 */

#include "src/ui/ui_component.h"

namespace legend {
namespace ui {

/**
 * @brief UIゲージクラス
 */
class Gauge : public UIComponent {
 public:
  /**
   * @brief コンストラクタ
   */
  Gauge();
  /**
   * @brief デストラクタ
   */
  virtual ~Gauge();
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
  /**
   * @brief 座標を設定する
   */
  virtual void SetPosition(const math::Vector2& position) override;
  /**
   * @brief 座標を取得する
   */
  virtual math::Vector2 GetPosition() const override;
  /**
   * @brief Z-Orderを設定する
   */
  virtual void SetZOrder(float z) override;
  /**
   * @brief Z-Orderを取得する
   */
  virtual float GetZOrder() const override;
  /**
   * @brief ゲージの値を0~1で設定する
   */
  void SetValue(float border);
  /**
   * @brief ゲージの値を取得する
   */
  float GetValue() const;

 protected:
  //! 描画するスプライト
  draw::Sprite2D sprite_;

  /**
   * @brief ゲージの描画用パラメータ
   */
  struct GaugeParameter {
    float value;
  };
  GaugeParameter param_;
  //! コンスタントバッファ
  directx::buffer::ConstantBuffer<GaugeParameter> constant_buffer_;
};

}  // namespace ui
}  // namespace legend

#endif  //! LEGEND_UI_GAUGE_H
