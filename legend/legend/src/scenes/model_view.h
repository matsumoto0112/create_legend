#ifndef LEGEND_SCENES_MODEL_VIEW_H_
#define LEGEND_SCENES_MODEL_VIEW_H_

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/render_target_texture.h"
#include "src/directx/buffer/texture_2d.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/directx/shader/graphics_pipeline_state.h"
#include "src/directx/shader/root_signature.h"
#include "src/math/matrix_4x4.h"
#include "src/scenes/scene.h"

namespace legend {
namespace scenes {

struct Transform {
  math::Matrix4x4 world;
};
struct WorldContext {
  math::Matrix4x4 view;
  math::Matrix4x4 projection;
};

/**
 * @class ModelView
 * @brief モデル描画テストシーン
 */
class ModelView : public Scene {
 public:
  /**
   * @brief コンストラクタ
   */
  ModelView(ISceneChange* scene_change);
  /**
   * @brief デストラクタ
   */
  ~ModelView();
  /**
   * @brief 初期化
   */
  void Initialize() override;
  /**
   * @brief 更新
   */
  void Update() override;
  /**
   * @brief 描画
   */
  void Draw() override;

  /**
   * @brief 終了処理
   */
  void Finalize() override;

 private:
  struct Object {
    directx::buffer::VertexBuffer vertex_buffer_;
    directx::buffer::IndexBuffer index_buffer_;
    directx::buffer::ConstantBuffer<Transform> transform_cb_;
    directx::buffer::ConstantBuffer<WorldContext> world_cb_;
  };
  std::vector<Object> objects_;

  directx::buffer::Texture2D texture_;
  std::shared_ptr<directx::shader::RootSignature> root_signature_;
  directx::shader::GraphicsPipelineState pipeline_state_;
};

}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_MODEL_VIEW_H_
