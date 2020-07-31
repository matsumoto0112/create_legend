#ifndef LEGEND_DRAW_SPRITE_2D_H_
#define LEGEND_DRAW_SPRITE_2D_H_

/**
 * @file sprite_2d.h
 * @brief 2Dスプライト描画
 */

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/texture_2d.h"
#include "src/directx/constant_buffer_structure.h"
#include "src/math/rect.h"

namespace legend {
namespace draw {
/**
 * @file Sprite2D
 * @brief 2D描画クラス
 */
class Sprite2D {
 public:
  /**
   * @brief コンストラクタ
   */
  Sprite2D();
  /**
   * @brief デストラクタ
   */
  ~Sprite2D();
  /**
   * @brief 初期化
   * @param texture 使用するテクスチャ
   */
  bool Init(std::shared_ptr<directx::buffer::Texture2D> texture,
            directx::descriptor_heap::heap_parameter::LocalHeapID cbv_heap_id);

 public:
  /**
   * @brief テクスチャの大きさを取得する
   */
  inline math::Vector2 GetContentSize() const { return content_size_; }
  /**
   * @brief 座標の設定
   */
  inline void SetPosition(const math::Vector2& position) {
    this->position_ = position;
  }
  /**
   * @brief 座標の取得
   */
  inline math::Vector2 GetPosition() const { return position_; }
  /**
   * @brief スケーリングの設定
   */
  inline void SetScale(const math::Vector2& scale) { this->scale_ = scale; }
  /**
   * @brief スケーリングの取得
   */
  inline math::Vector2 GetScale() const { return scale_; }
  /**
   * @brief Zオーダーを設定する
   * @param z_order 設定する値(0~1)
   */
  inline void SetZOrder(float z_order) { this->z_order_ = z_order; }
  /**
   * @brief Zオーダーを取得する
   */
  inline float GetZOrder() const { return z_order_; }
  /**
   * @brief 回転角度を設定する
   * @param rotate 設定する角度(rad)
   */
  inline void SetRotate(float rotate) { this->rotate_ = rotate; }
  /**
   * @brief 回転角度を取得する
=  */
  inline float GetRotate() const { return rotate_; }

  inline void SetRect(const math::Rect& rect) { this->rect_ = rect; }
  inline const math::Rect& GetRect() const { return rect_; }
  /**
   * @brief コマンドリストに積む
   */
  void SetToCommandList();

 protected:
  //! コンテンツのサイズ
  math::Vector2 content_size_;
  //! スケーリング
  math::Vector2 scale_;
  //! 座標
  math::Vector2 position_;
  //! Zオーダー
  float z_order_;
  //! 回転角度
  float rotate_;
  //! トランスフォーム行列バッファ
  directx::buffer::ConstantBuffer<directx::constant_buffer_structure::Transform>
      transform_constant_buffer_;
  //! テクスチャ
  std::shared_ptr<directx::buffer::Texture2D> texture_;
  math::Rect rect_;
  directx::buffer::ConstantBuffer<directx::constant_buffer_structure::UVRect>
      uv_rect_constant_buffer_;
};
}  // namespace draw
}  // namespace legend

#endif  //! LEGEND_DRAW_SPRITE_2D_H_
