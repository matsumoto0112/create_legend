#ifndef LEGEND_DIRECTX_SHADER_COMPUTE_PIPELINE_STATE_DESC_H_
#define LEGEND_DIRECTX_SHADER_COMPUTE_PIPELINE_STATE_DESC_H_

/**
 * @file compute_pipeline_state_desc.h
 */

#include "src/directx/shader/compute_shader.h"
#include "src/directx/shader/root_signature.h"

namespace legend {
namespace directx {
namespace shader {
struct ComputePipelineStateDesc : public D3D12_COMPUTE_PIPELINE_STATE_DESC {
 public:
  ComputePipelineStateDesc() = default;
  void SetComputeShader(RootSignature* root_signature);
  void SetRootSignature(ComputeShader* shader);
};

}  // namespace shader
}  // namespace directx
}  // namespace legend
#endif  //! LEGEND_DIRECTX_SHADER_COMPUTE_PIPELINE_STATE_DESC_H_
