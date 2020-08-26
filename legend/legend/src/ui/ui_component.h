#ifndef LEGEND_UI_UI_COMPONENT_H
#define LEGEND_UI_UI_COMPONENT_H

/**
 * @file ui_component.h
 * @brief UIコンポーネント定義
 */

#include "src/draw/sprite_renderer.h"

namespace legend {
namespace ui {

/**
 * @class UIComponent
 * @brief UIコンポーネント
 */
class UIComponent {
 public:
  /**
   * @brief コンストラクタ
   */
  UIComponent() : enable_(true){};
  /**
   * @brief デストラクタ
   */
  virtual ~UIComponent() = default;
  /**
   * @brief 座標を設定する
   */
  virtual void SetPosition(const math::Vector2& position) = 0;
  /**
   * @brief 座標を取得する
   */
  virtual math::Vector2 GetPosition() const = 0;
  /**
   * @brief Z-Orderを設定する
   */
  virtual void SetZOrder(float z) = 0;
  /**
   * @brief Z-Orderを取得する
   */
  virtual float GetZOrder() const = 0;
  /**
   * @brief 自身の描画アイテムを描画リストに追加する
   */
  virtual void StackOwnItems(draw::SpriteRenderer& renderer) = 0;

  virtual void SetEnable(bool enable) { this->enable_ = enable; }

  virtual bool GetEnable() const { return this->enable_; }

 protected:
  //! UIが描画されるかどうか
  bool enable_;
};

}  // namespace ui
}  // namespace legend

#endif  //! LEGEND_UI_UI_COMPONENT_H
