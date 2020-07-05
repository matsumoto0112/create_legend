#ifndef LEGEND_DRAW_SPRITE_2D_H_
#define LEGEND_DRAW_SPRITE_2D_H_

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/texture_2d.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/directx/shader/graphics_pipeline_state.h"
#include "src/directx/shader/root_signature.h"
#include "src/math/matrix_4x4.h"

namespace legend {
namespace draw {

struct Transform {
  math::Matrix4x4 world;
};
struct WorldContext {
  math::Matrix4x4 view;
  math::Matrix4x4 projection;
};

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
   */
  bool Initialize(const std::wstring file_name,
                  const math::IntVector2 window_size);
  /**
   * @brief 描画
   */
  void Draw();

 private:
  struct Sprite {
    directx::buffer::VertexBuffer vertex_buffer_;
    directx::buffer::IndexBuffer index_buffer_;
    directx::buffer::ConstantBuffer<Transform> transform_constant_buffer_;
  };

  Sprite sprite_;
  math::Vector3 position_;
  math::Vector3 scale_;
  directx::buffer::ConstantBuffer<WorldContext> world_constant_buffer_;
  directx::buffer::Texture2D texture_;
  std::shared_ptr<directx::shader::RootSignature> root_signature_;
  directx::shader::GraphicsPipelineState pipeline_state_;
};
}  // namespace draw
}  // namespace legend

#endif  //! LEGEND_DRAW_SPRITE_2D_H_
