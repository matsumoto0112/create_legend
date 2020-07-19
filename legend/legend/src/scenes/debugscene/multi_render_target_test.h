#ifndef LEGEND_SCENES_DEBUGSCENE_MULTI_RENDER_TARGET_TEST_H_
#define LEGEND_SCENES_DEBUGSCENE_MULTI_RENDER_TARGET_TEST_H_

/**
 * @file multi_render_target_test.h
 * @brief �}���`�����_�[�^�[�Q�b�g�e�X�g�V�[��
 */

#include "src/camera/perspective_camera.h"
#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/multi_render_target_texture.h"
#include "src/directx/shader/graphics_pipeline_state.h"
#include "src/directx/shader/root_signature.h"
#include "src/draw/model.h"
#include "src/math/matrix_4x4.h"
#include "src/scenes/scene.h"

namespace legend {
namespace scenes {
namespace debugscene {
/**
 * @brief �}���`�����_�[�^�[�Q�b�g�e�X�g�V�[��
 */
class MultiRenderTargetTest : public Scene {
 public:
  MultiRenderTargetTest(ISceneChange* scene_change);
  virtual ~MultiRenderTargetTest();
  virtual bool Initialize() override;
  virtual bool Update() override;
  virtual void Draw() override;
  virtual void Finalize() override;

 private:
  //�ʏ�`��p
  draw::Model model_;
  directx::shader::GraphicsPipelineState pipeline_state_;
  directx::shader::GraphicsPipelineState post_process_pipeline_;

  //�|�X�g�v���Z�X�p
  directx::buffer::VertexBuffer post_process_vertex_buffer_;
  directx::buffer::IndexBuffer post_process_index_buffer_;

  directx::buffer::MultiRenderTargetTexture render_target_texture_;
  directx::buffer::ConstantBuffer<directx::constant_buffer_structure::Transform>
      post_process_transform_cb_;
  directx::buffer::ConstantBuffer<
      directx::constant_buffer_structure::WorldContext>
      post_process_world_cb_;

  //! �J����
  camera::PerspectiveCamera camera_;
};

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DEBUGSCENE_MULTI_RENDER_TARGET_TEST_H_
