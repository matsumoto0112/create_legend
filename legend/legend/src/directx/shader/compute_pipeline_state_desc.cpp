#include "src/directx/shader/compute_pipeline_state_desc.h"

namespace legend {
namespace directx {
namespace shader {

//���[�g�V�O�l�`���̃Z�b�g
void ComputePipelineStateDesc::SetRootSignature(RootSignature* root_signature) {
  this->pRootSignature = root_signature->GetRootSignature();
}

//�R���s���[�g�V�F�[�_�[���Z�b�g����
void ComputePipelineStateDesc::SetComputeShader(ComputeShader* shader) {
  this->CS = shader->GetShaderBytecode();
}

}  // namespace shader
}  // namespace directx
}  // namespace legend