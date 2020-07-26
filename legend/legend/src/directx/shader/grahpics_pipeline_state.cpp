#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/graphics_pipeline_state.h"

namespace legend {
namespace directx {
namespace shader {

//�R���X�g���N�^
GraphicsPipelineState::GraphicsPipelineState()
    : pipeline_state_desc_{},
      vertex_shader_(nullptr),
      pixel_shader_(nullptr),
      pipeline_state_(nullptr) {}

//�f�X�g���N�^
GraphicsPipelineState::~GraphicsPipelineState() {}

//������
bool GraphicsPipelineState::Init(IDirectXAccessor& device) {
  pipeline_state_desc_.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
  pipeline_state_desc_.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
  pipeline_state_desc_.PrimitiveTopologyType =
      D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

  return true;
}

//���[�g�V�O�l�`�����Z�b�g����
void GraphicsPipelineState::SetRootSignature(
    std::shared_ptr<RootSignature> root_signature) {
  this->root_signature_ = root_signature;
  if (this->root_signature_) {
    pipeline_state_desc_.pRootSignature =
        this->root_signature_->GetRootSignature();
  }
}

//���_�V�F�[�_�[���Z�b�g����
void GraphicsPipelineState::SetVertexShader(
    std::shared_ptr<VertexShader> vertex_shader) {
  this->vertex_shader_ = vertex_shader;

  if (this->vertex_shader_) {
    pipeline_state_desc_.VS = this->vertex_shader_->GetShaderBytecode();
    pipeline_state_desc_.InputLayout = this->vertex_shader_->GetInputLayout();
  }
}

//�s�N�Z���V�F�[�_�[���Z�b�g����
void GraphicsPipelineState::SetPixelShader(
    std::shared_ptr<PixelShader> pixel_shader) {
  this->pixel_shader_ = pixel_shader;

  if (this->pixel_shader_) {
    pipeline_state_desc_.PS = this->pixel_shader_->GetShaderBytecode();
  }
}

//�A���t�@�u�����h�f�X�N���Z�b�g����
void GraphicsPipelineState::SetBlendDesc(
    const D3D12_RENDER_TARGET_BLEND_DESC& blend_desc, u32 rtv_index) {
  pipeline_state_desc_.BlendState.RenderTarget[rtv_index] = blend_desc;
}

//�v���~�e�B�u�`��̐ݒ�
void GraphicsPipelineState::SetPrimitiveTopology(
    D3D12_PRIMITIVE_TOPOLOGY_TYPE topology_type) {
  pipeline_state_desc_.PrimitiveTopologyType = topology_type;
}

void GraphicsPipelineState::SetRTVFormat(DXGI_FORMAT format, u32 rtv_index) {
  pipeline_state_desc_.RTVFormats[rtv_index] = format;
}

void GraphicsPipelineState::SetRenderTargetNum(u32 num) {
  pipeline_state_desc_.NumRenderTargets = num;
}

void GraphicsPipelineState::SetDSVFormat(DXGI_FORMAT format) {
  pipeline_state_desc_.DSVFormat = format;
}

void GraphicsPipelineState::SetDepthStencilState(
    const D3D12_DEPTH_STENCIL_DESC& desc) {
  pipeline_state_desc_.DepthStencilState = desc;
}

//�p�C�v���C���X�e�[�g���Z�b�g����
bool GraphicsPipelineState::CreatePipelineState(IDirectXAccessor& device) {
  pipeline_state_desc_.SampleMask = UINT_MAX;
  pipeline_state_desc_.SampleDesc.Count = 1;

  if (FAILED(device.GetDevice()->CreateGraphicsPipelineState(
          &pipeline_state_desc_, IID_PPV_ARGS(&pipeline_state_)))) {
    return false;
  }

  return true;
}

//�R�}���h���X�g�ɃZ�b�g����
void GraphicsPipelineState::SetGraphicsCommandList(IDirectXAccessor& device) {
  device.GetCommandList()->SetPipelineState(pipeline_state_.Get());
}

}  // namespace shader
}  // namespace directx
}  // namespace legend
