#include "src/directx/shader/compute_pipeline_state_desc.h"

namespace legend {
namespace directx {
namespace shader {

void ComputePipelineStateDesc::SetComputeShader(RootSignature* root_signature) {
  this->pRootSignature = root_signature->GetRootSignature();
}
void ComputePipelineStateDesc::SetRootSignature(ComputeShader* shader) {
  this->CS = shader->GetShaderBytecode();
}

}  // namespace shader
}  // namespace directx
}  // namespace legend