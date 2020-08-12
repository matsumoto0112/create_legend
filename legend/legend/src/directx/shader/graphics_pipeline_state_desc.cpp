#include "src/directx/shader/graphics_pipeline_state_desc.h"

namespace legend {
namespace directx {
namespace shader {

//���[�g�V�O�l�`��
void GraphicsPipelineStateDesc::SetRootSignature(
    RootSignature* root_signature) {
  this->pRootSignature = root_signature->GetRootSignature();
}

//���_�V�F�[�_�[
void GraphicsPipelineStateDesc::SetVertexShader(VertexShader* shader) {
  this->VS = shader->GetShaderBytecode();
  this->InputLayout = shader->GetInputLayout();
}

//�s�N�Z���V�F�[�_�[
void GraphicsPipelineStateDesc::SetPixelShader(PixelShader* shader) {
  this->PS = shader->GetShaderBytecode();
}

//�W�I���g���V�F�[�_�[
void GraphicsPipelineStateDesc::SetGeometryShader(GeometryShader* shader) {
  this->GS = shader->GetShaderBytecode();
}

//�����_�[�^�[�Q�b�g
void GraphicsPipelineStateDesc::SetRenderTargets(
    const render_target::MultiRenderTargetTexture& render_target) {
  const auto formats = render_target.GetRTVFormats();
  std::copy(formats.begin(), formats.end(), this->RTVFormats);
  this->NumRenderTargets = render_target.GetRenderTargetNum();
}

//�f�v�X�E�X�e���V��
void GraphicsPipelineStateDesc::SetDepthStencilTarget(
    const render_target::DepthStencil& depth_stencil_target) {
  this->DSVFormat = depth_stencil_target.GetFormat();
  this->DepthStencilState = depth_stencil_target.GetDepthStencilDesc();
}

}  // namespace shader
}  // namespace directx
}  // namespace legend