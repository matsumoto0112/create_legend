#ifndef LEGEND_DRAW_SPRITE_RENDERER_H_
#define LEGEND_DRAW_SPRITE_RENDERER_H_

/**
 * @file sprite_renderer.h
 * @brief スプライト描画機能定義クラス
 */

#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/directx/shader/graphics_pipeline_state.h"
#include "src/draw/sprite_2d.h"

namespace legend {
namespace draw {

/**
 * @brief スプライト描画器
 */
class SpriteRenderer {
 public:
  /**
   * @brief コンストラクタ
   */
  SpriteRenderer();
  /**
   * @brief デストラクタ
   */
  virtual ~SpriteRenderer();
  /**
   * @brief 初期化
   * @param window_size ウィンドウサイズ
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(const math::Vector2& window_size);
  /**
   * @brief 描画リストにスプライトを追加する
   */
  void AddDrawItems(Sprite2D* sprite);
  /**
   * @brief 描画リストにあるスプライトを描画する
   * @param command_list コマンドリスト
   */
  void DrawItems(directx::device::CommandList& command_list);

 private:
  using WorldContext_CBStruct =
      directx::buffer::constant_buffer_structure::WorldContext;

 private:
  //! 頂点バッファ
  directx::buffer::VertexBuffer vertex_buffer_;
  //! インデックスバッファ
  directx::buffer::IndexBuffer index_buffer_;
  //! ワールド情報コンスタントバッファ
  directx::buffer::ConstantBuffer<WorldContext_CBStruct> world_cb_;
  //! 描画パイプライン
  directx::shader::GraphicsPipelineState pipeline_state_;
  //! スプライトリスト
  std::vector<Sprite2D*> draw_items_;
};

}  // namespace draw
}  // namespace legend

#endif  //! LEGEND_DRAW_SPRITE_RENDERER_H_
