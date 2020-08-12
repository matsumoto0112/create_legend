#ifndef LEGEND_DIRECTX_SHADER_COMPUTE_PIPELINE_STATE_DESC_H_
#define LEGEND_DIRECTX_SHADER_COMPUTE_PIPELINE_STATE_DESC_H_

/**
 * @file compute_pipeline_state_desc.h
 * @brief コンピュートパイプラインステートディスク定義
 */

#include "src/directx/shader/compute_shader.h"
#include "src/directx/shader/root_signature.h"

namespace legend {
namespace directx {
namespace shader {

/**
 * @struct ComputePipelineStateDesc
 * @brief コンピュートパイプラインステートディスク
 */
struct ComputePipelineStateDesc : public D3D12_COMPUTE_PIPELINE_STATE_DESC {
 public:
  /**
   * @brief コンストラクタ
   */
  ComputePipelineStateDesc() = default;
  /**
   * @brief ルートシグネチャをセットする
   */
  void SetRootSignature(RootSignature* root_signature);
  /**
   * @brief コンピュートシェーダーをセットする
   */
  void SetComputeShader(ComputeShader* shader);
};

}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_COMPUTE_PIPELINE_STATE_DESC_H_
