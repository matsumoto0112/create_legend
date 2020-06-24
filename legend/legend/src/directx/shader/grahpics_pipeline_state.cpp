#include "src/directx/shader/graphics_pipeline_state.h"

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
  D3D12_DESCRIPTOR_HEAP_DESC heap_desc;
  heap_desc.NumDescriptors = 1;
  heap_desc.Type =
      D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
  heap_desc.Flags =
      D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
  heap_desc.NodeMask = 0;
  if (FAILED(device.GetDevice()->CreateDescriptorHeap(&heap_desc,
                                                      IID_PPV_ARGS(&heap_)))) {
    return false;
  }

  CD3DX12_DESCRIPTOR_RANGE ranges[1];
  ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
                 1, 0, 0);
  CD3DX12_ROOT_PARAMETER params[1];
  params[0].InitAsDescriptorTable(1, ranges);

  CD3DX12_ROOT_SIGNATURE_DESC root_signature_desc = {};
  root_signature_desc.Init(
      1, params, 0, nullptr,
      D3D12_ROOT_SIGNATURE_FLAGS::
          D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

  ComPtr<ID3DBlob> signature;
  ComPtr<ID3DBlob> error;
  if (FAILED(D3D12SerializeRootSignature(
          &root_signature_desc,
          D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1, &signature,
          &error))) {
    MY_LOG(L"D3D12SerializeRootSignature failed");
    return false;
  }

  if (FAILED(device.GetDevice()->CreateRootSignature(
          0, signature->GetBufferPointer(), signature->GetBufferSize(),
          IID_PPV_ARGS(&root_signature_)))) {
    MY_LOG(L"CreateRootSignature failed");
    return false;
  }

  if (FAILED(device.GetDevice()->CreateCommittedResource(
          &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD),
          D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
          &CD3DX12_RESOURCE_DESC::Buffer(sizeof(color_)),
          D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
          IID_PPV_ARGS(&constant_buffer_)))) {
    return false;
  }

  color_.color[0] = 0.0f;
  color_.color[1] = 1.0f;
  color_.color[2] = 0.0f;
  void* begin;
  if (FAILED(constant_buffer_->Map(0, nullptr, &begin))) {
    return false;
  }
  memcpy(begin, &color_, sizeof(color_));
  constant_buffer_->Unmap(0, nullptr);

  D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
  cbvDesc.BufferLocation = constant_buffer_->GetGPUVirtualAddress();
  cbvDesc.SizeInBytes = 256;
  device.GetDevice()->CreateConstantBufferView(
      &cbvDesc, heap_->GetCPUDescriptorHandleForHeapStart());

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

//コマンドリストにセットする
void GraphicsPipelineState::SetGraphicsCommandList(DirectX12Device& device) {
  float value = color_.color[1] + 0.01f;
  if (value > 1.0f) value -= 1.0f;
  color_.color[1] = value;
  void* begin;
  if (FAILED(constant_buffer_->Map(0, nullptr, &begin))) {
    return;
  }
  memcpy(begin, &color_, sizeof(color_));

  device.GetCommandList()->SetGraphicsRootSignature(root_signature_.Get());
  device.GetCommandList()->SetPipelineState(pipeline_state_.Get());
  ID3D12DescriptorHeap* heaps[] = {heap_.Get()};
  device.GetCommandList()->SetDescriptorHeaps(1, heaps);
  device.GetCommandList()->SetGraphicsRootDescriptorTable(
      0, heap_->GetGPUDescriptorHandleForHeapStart());
}

}  // namespace shader
}  // namespace directx
}  // namespace legend
