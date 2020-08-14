#ifndef LEGEND_DIRECTX_SHADER_GRAPHICS_PIPELINE_STATE_DESC_H_
#define LEGEND_DIRECTX_SHADER_GRAPHICS_PIPELINE_STATE_DESC_H_

/**
 * @file graphics_pipeline_state_desc.h
 * @brief グラフィックスパイプラインステートディスク定義
 */

#include "src/directx/render_target/depth_stencil.h"
#include "src/directx/render_target/multi_render_target_texture.h"
#include "src/directx/shader/geometry_shader.h"
#include "src/directx/shader/pixel_shader.h"
#include "src/directx/shader/root_signature.h"
#include "src/directx/shader/vertex_shader.h"

namespace legend {
namespace directx {
namespace shader {

/**
 * @struct GraphicsPipelineStateDesc
 * @brief グラフィックスパイプラインステートディスク
 */
struct GraphicsPipelineStateDesc : public D3D12_GRAPHICS_PIPELINE_STATE_DESC {
  /**
   * @brief コンストラクタ
   */
  GraphicsPipelineStateDesc() = default;
  /**
   * @brief ルートシグネチャをセットする
   */
  void SetRootSignature(RootSignature* root_signature);
  /**
   * @brief 頂点シェーダーの情報をセットする
   */
  void SetVertexShader(VertexShader* shader);
  /**
   * @brief ピクセルシェーダーの情報をセットする
   */
  void SetPixelShader(PixelShader* shader);
  /**
   * @brief ジオメトリシェーダーの情報をセットする
   */
  void SetGeometryShader(GeometryShader* shader);
  /**
   * @brief レンダーターゲットの情報をセットする
   */
  void SetRenderTargets(
      const render_target::MultiRenderTargetTexture& render_target);
  /**
   * @brief デプス・ステンシルの情報をセットする
   */
  void SetDepthStencilTarget(
      const render_target::DepthStencil& depth_stencil_target);
};
}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_GRAPHICS_PIPELINE_STATE_DESC_H_
