#include "src/directx/shader/graphics_pipeline_state.h"

namespace legend {
namespace directx {
namespace shader {
GraphicsPipelineState::GraphicsPipelineState()
    : pipeline_state_desc_{},
      vertex_shader_(nullptr),
      pixel_shader_(nullptr),
      pipeline_state_(nullptr) {}

GraphicsPipelineState::~GraphicsPipelineState() {}

bool GraphicsPipelineState::Init(DirectX12Device& device) {
  pipeline_state_desc_.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
  pipeline_state_desc_.DepthStencilState.DepthEnable = FALSE;
  pipeline_state_desc_.DepthStencilState.StencilEnable = FALSE;

  pipeline_state_desc_.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

  return true;
}

void GraphicsPipelineState::SetVertexShader(
    std::shared_ptr<VertexShader> vertex_shader) {
  this->vertex_shader_ = vertex_shader;

  if (vertex_shader_) {
    pipeline_state_desc_.VS = vertex_shader_->GetShaderBytecode();
  }
}

void GraphicsPipelineState::SetPixelShader(
    std::shared_ptr<PixelShader> pixel_shader) {
  this->pixel_shader_ = pixel_shader;

  if (pixel_shader_) {
    pipeline_state_desc_.PS = pixel_shader_->GetShaderBytecode();
  }
}

bool GraphicsPipelineState::CreatePipelineState(DirectX12Device& device) {
  CD3DX12_ROOT_SIGNATURE_DESC root_signature_desc;
  root_signature_desc.Init(
      0, nullptr, 0, nullptr,
      D3D12_ROOT_SIGNATURE_FLAGS::
          D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
  ComPtr<ID3DBlob> signature;
  ComPtr<ID3DBlob> error;
  if (FAILED(D3D12SerializeRootSignature(
          &root_signature_desc,
          D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1, &signature,
          &error))) {
    util::debug::Log(L"D3D12SerializeRootSignature failed");
    return false;
  }

  if (FAILED(device.GetDevice()->CreateRootSignature(
          0, signature->GetBufferPointer(), signature->GetBufferSize(),
          IID_PPV_ARGS(&root_signature_)))) {
    util::debug::Log(L"CreateRootSignature failed");
    return false;
  }

  D3D12_INPUT_ELEMENT_DESC input_element_descs[] = {
      {"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
       D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
       0},
  };

  pipeline_state_desc_.InputLayout = {input_element_descs,
                                      _countof(input_element_descs)};
  pipeline_state_desc_.pRootSignature = root_signature_.Get();
  pipeline_state_desc_.SampleMask = UINT_MAX;
  pipeline_state_desc_.PrimitiveTopologyType =
      D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
  pipeline_state_desc_.NumRenderTargets = 1;
  pipeline_state_desc_.RTVFormats[0] = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
  pipeline_state_desc_.SampleDesc.Count = 1;

  if (FAILED(device.GetDevice()->CreateGraphicsPipelineState(
          &pipeline_state_desc_, IID_PPV_ARGS(&pipeline_state_)))) {
    return false;
  }

  return true;
}

void GraphicsPipelineState::SetGraphicsCommandList(DirectX12Device& device) {
  device.GetCommandList()->SetGraphicsRootSignature(root_signature_.Get());
  device.GetCommandList()->SetPipelineState(pipeline_state_.Get());
}

}  // namespace shader
}  // namespace directx
}  // namespace legend
