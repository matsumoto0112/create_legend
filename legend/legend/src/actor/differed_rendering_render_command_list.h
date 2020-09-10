#ifndef LEGEND_ACTOR_DIFFERED_RENDERING_RENDER_COMMAND_LIST_H
#define LEGEND_ACTOR_DIFFERED_RENDERING_RENDER_COMMAND_LIST_H

/**
 * @file differed_rendering_render_command_list.h
 * @brief
 * Differed-Rendering用コマンドリストクラス定義
 */

#include "src/actor/actor.h"
#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/object/back_ground.h"

namespace legend {
namespace actor {
/**
 * @brief Differed-Rendering用コマンドリスト
 */
class DifferedRenderingRenderCommandList {
 public:
  /**
   * @brief コンストラクタ
   */
  DifferedRenderingRenderCommandList();
  /**
   * @brief デストラクタ
   */
  virtual ~DifferedRenderingRenderCommandList();
  /**
   * @brief 描画リストに追加
   */
  void Push(Actor* actor);
  /**
   * @brief 描画リストのクリア
   */
  void Clear();
  /**
   * @brief 描画パスを実行する
   */
  void RenderPass();

 private:
  //! 描画リスト
  std::vector<Actor*> actor_list_;
  //! 背景オブジェクト
  object::BackGround back_ground_;
  directx::buffer::VertexBuffer vertex_buffer_;  //! Diff-Render用頂点バッファ
  directx::buffer::IndexBuffer
      index_buffer_;  //! Diff-Render用インデックスバッファ
};
}  // namespace actor
}  // namespace legend
#endif  //! LEGEND_ACTOR_DIFFERED_RENDERING_RENDER_COMMAND_LIST_H
