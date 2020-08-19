#ifndef LEGEND_UI_IMAGE_H
#define LEGEND_UI_IMAGE_H

/**
 * @file image.h
 * @brief UIイメージクラス定義
 */

#include "src/draw/sprite_2d.h"
#include "src/ui/ui_component.h"

namespace legend {
namespace ui {

/**
 * @brief UIイメージクラス
 */
class Image : public UIComponent {
 public:
  /**
   * @brief コンストラクタ
   */
  Image();
  /**
   * @brief デストラクタ
   */
  virtual ~Image();
  /**
   * @brief 初期化
   * @param name 画像名
   * @param heap_id 使用するヒープID
   */
  virtual bool Init(
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
   * @brief スプライトを取得する
   */
  virtual draw::Sprite2D& GetSprite() { return sprite_; }

 protected:
  //! 描画するスプライト
  draw::Sprite2D sprite_;
};

}  // namespace ui
}  // namespace legend
#endif  //! LEGEND_UI_IMAGE_H
