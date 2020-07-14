#ifndef LEGEND_DRAW_SPRITE_RENDERER_H_
#define LEGEND_DRAW_SPRITE_RENDERER_H_

/**
 * @file sprite_renderer.h
 * @brief スプライト描画機能定義クラス
 */

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/directx/constant_buffer_structure.h"
#include "src/directx/shader/graphics_pipeline_state.h"
#include "src/draw/sprite_2d.h"

namespace legend {
namespace draw {

/**
 * @brief スプライト描画器
 */
class SpriteRenderer {
 public:
  SpriteRenderer();
  ~SpriteRenderer();
  bool Init(const math::Vector2& window_size);
  void AddDrawItems(Sprite2D* sprite);
  void DrawItems();

 private:
  directx::buffer::VertexBuffer vertex_buffer_;
  directx::buffer::IndexBuffer index_buffer_;
  directx::buffer::ConstantBuffer<
      directx::constant_buffer_structure::WorldContext>
      world_cb_;
  directx::shader::GraphicsPipelineState pipeline_state_;
  std::vector<Sprite2D*> draw_items_;
};

}  // namespace draw
}  // namespace legend

#endif  //! LEGEND_DRAW_SPRITE_RENDERER_H_
