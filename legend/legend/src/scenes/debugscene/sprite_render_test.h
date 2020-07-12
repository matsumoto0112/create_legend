#ifndef LEGEND_SCENES_DEBUGSCENE_SPRITE_RENDER_TEST_H_
#define LEGEND_SCENES_DEBUGSCENE_SPRITE_RENDER_TEST_H_

/**
 * @file sprite_render_test.h
 * @brief スプライト描画テストシーン
 */

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/texture_2d.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/directx/constant_buffer_structure.h"
#include "src/directx/shader/graphics_pipeline_state.h"
#include "src/scenes/scene.h"

namespace legend {
namespace scenes {
namespace debugscene {
class SpriteRenderTest : public Scene {
 public:
  SpriteRenderTest(ISceneChange* scene_change);
  virtual ~SpriteRenderTest();
  virtual bool Initialize() override;
  virtual bool Update() override;
  virtual void Draw() override;
  virtual void Finalize() override;

 private:
  directx::buffer::VertexBuffer vertex_buffer_;
  directx::buffer::IndexBuffer index_buffer_;
  directx::buffer::Texture2D texture_;
  directx::buffer::ConstantBuffer<directx::constant_buffer_structure::Transform>
      transform_cb_;
  directx::buffer::ConstantBuffer<
      directx::constant_buffer_structure::WorldContext>
      world_cb_;
  directx::shader::GraphicsPipelineState pipeline_state_;
};

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DEBUGSCENE_SPRITE_RENDER_TEST_H_
