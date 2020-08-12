#ifndef LEGEND_SCENES_DEBUGSCENE_MULTI_RENDER_TARGET_TEST_H_
#define LEGEND_SCENES_DEBUGSCENE_MULTI_RENDER_TARGET_TEST_H_

/**
 * @file multi_render_target_test.h
 * @brief マルチレンダーターゲットテストシーン
 */

#include "src/camera/perspective_camera.h"
#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/constant_buffer_structure.h"
#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/directx/shader/root_signature.h"
#include "src/draw/model.h"
#include "src/scenes/scene.h"
#include "src/util/transform.h"

namespace legend {
namespace scenes {
namespace debugscene {
/**
 * @brief マルチレンダーターゲットテストシーン
 */
class MultiRenderTargetTest : public Scene {
  using Transform_CBStruct =
      directx::buffer::constant_buffer_structure::Transform;
  using WorldContext_CBStruct =
      directx::buffer::constant_buffer_structure::WorldContext;

 public:
  MultiRenderTargetTest(ISceneChange* scene_change);
  virtual ~MultiRenderTargetTest();
  virtual bool Initialize() override;
  virtual bool Update() override;
  virtual void Draw() override;
  virtual void Finalize() override;

 private:
  std::vector<util::Transform> transforms_;
  std::vector<directx::buffer::ConstantBuffer<Transform_CBStruct>>
      transform_cbs_;
  //! カメラ
  camera::PerspectiveCamera camera_;

  //ポストプロセス用
  directx::buffer::VertexBuffer post_process_vertex_buffer_;
  directx::buffer::IndexBuffer post_process_index_buffer_;

  directx::buffer::ConstantBuffer<
      directx::buffer::constant_buffer_structure::Transform>
      post_process_transform_cb_;
  directx::buffer::ConstantBuffer<WorldContext_CBStruct> post_process_world_cb_;
  struct MultiRenderTargetTestPPConstantBufferStructure {
    float border;
  };
  directx::buffer::ConstantBuffer<
      MultiRenderTargetTestPPConstantBufferStructure>
      post_process_local_cb_;
};

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DEBUGSCENE_MULTI_RENDER_TARGET_TEST_H_
