#include "src/directx/shader/graphics_pipeline_state.h"

namespace {
legend::u32 heap_size_;
}  // namespace

namespace legend {
namespace directx {
namespace shader {

//コンストラクタ
GraphicsPipelineState::GraphicsPipelineState()
    : pipeline_state_desc_{},
      vertex_shader_(nullptr),
      pixel_shader_(nullptr),
      pipeline_state_(nullptr) {}

//デストラクタ
GraphicsPipelineState::~GraphicsPipelineState() {}

//初期化
bool GraphicsPipelineState::Init(DirectX12Device& device) {
  pipeline_state_desc_.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
  pipeline_state_desc_.DepthStencilState.DepthEnable = FALSE;
  pipeline_state_desc_.DepthStencilState.StencilEnable = FALSE;

  pipeline_state_desc_.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

  return true;
}

//頂点シェーダー
void GraphicsPipelineState::SetVertexShader(
    std::shared_ptr<VertexShader> vertex_shader) {
  this->vertex_shader_ = vertex_shader;

  if (vertex_shader_) {
    pipeline_state_desc_.VS = vertex_shader_->GetShaderBytecode();
    pipeline_state_desc_.InputLayout = vertex_shader_->GetInputLayout();
  }
}

//ピクセルシェーダー
void GraphicsPipelineState::SetPixelShader(
    std::shared_ptr<PixelShader> pixel_shader) {
  this->pixel_shader_ = pixel_shader;

  if (pixel_shader_) {
    pipeline_state_desc_.PS = pixel_shader_->GetShaderBytecode();
  }
}

//パイプラインステート
bool GraphicsPipelineState::CreatePipelineState(DirectX12Device& device) {
  if (!root_signature_.Init(device, L"RootSignature")) {
    return false;
  }

  pipeline_state_desc_.pRootSignature = root_signature_.GetRootSignature();
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

//コマンドリストにセットする
void GraphicsPipelineState::SetGraphicsCommandList(DirectX12Device& device) {
  root_signature_.SetGraphicsCommandList(device);
  device.GetCommandList()->SetPipelineState(pipeline_state_.Get());
}

}  // namespace shader
}  // namespace directx
}  // namespace legend
