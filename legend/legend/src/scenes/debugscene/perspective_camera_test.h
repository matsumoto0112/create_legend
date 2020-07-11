#ifndef LEGEND_SCENES_DEBUGSCENE_PERSPECTIVE_CAMERA_TEST_H_
#define LEGEND_SCENES_DEBUGSCENE_PERSPECTIVE_CAMERA_TEST_H_

/**
 * @file perspective_camera_test.h
 * @brief パースペクティブカメラ挙動確認シーン定義
 */

#include "src/camera/perspective_camera.h"
#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/texture_2d.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/directx/shader/graphics_pipeline_state.h"
#include "src/directx/shader/root_signature.h"
#include "src/math/matrix_4x4.h"
#include "src/scenes/scene.h"

namespace legend {
namespace scenes {
namespace debugscene {
class PerspectiveCameraTest : public Scene {
  struct Transform {
    math::Matrix4x4 world;
  };

 public:
  PerspectiveCameraTest(ISceneChange* scene_change);
  virtual ~PerspectiveCameraTest();
  virtual void Initialize() override;
  virtual void Update() override;
  virtual void Draw() override;
  virtual void Finalize() override;

 private:
  directx::buffer::VertexBuffer vertex_buffer_;
  directx::buffer::IndexBuffer index_buffer_;
  directx::buffer::Texture2D texture_;
  directx::buffer::ConstantBuffer<Transform> transform_cb_;
  std::shared_ptr<directx::shader::RootSignature> root_signature_;
  directx::shader::GraphicsPipelineState pipeline_state_;

  camera::PerspectiveCamera camera_;
};

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DEBUGSCENE_PERSPECTIVE_CAMERA_TEST_H_