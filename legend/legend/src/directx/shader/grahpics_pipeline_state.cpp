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
  DescriptorHeap::Desc heap_desc = {};
  heap_desc.descriptor_num = 10;
  heap_desc.type = HeapType::CBV_SRV_UAV;
  heap_desc.flag = HeapFlag::ShaderVisible;
  if (!heap_.Init(device, heap_desc, L"ShaderResourceHeap")) {
    return false;
  }

  if (!root_signature_.Init(device, L"RootSignature")) {
    return false;
  }

  if (!color_constant_buffer.Init(device, heap_.GetCPUHandle(0),
                                  heap_.GetGPUHandle(0),
                                  L"Color ConstantBuffer")) {
    return false;
  }
  Color& color = color_constant_buffer.GetStagingRef();
  color.color = {0.0f, 1.0f, 0.0f, 1.0f};

  heap_size_ = device.GetDevice()->GetDescriptorHandleIncrementSize(
      D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

  constexpr u32 WIDTH = 32;
  constexpr u32 HEIGHT = 32;
  struct Pixel {
    u8 color[4];
  };
  std::vector<Pixel> datas(WIDTH * HEIGHT);
  for (u32 w = 0; w < WIDTH; w++) {
    for (u32 h = 0; h < HEIGHT; h++) {
      datas[h * WIDTH + w].color[0] = 0xff;
      datas[h * WIDTH + w].color[1] = 0xff;
      datas[h * WIDTH + w].color[2] = 0;
      datas[h * WIDTH + w].color[3] = 0xff;
    }
  }

  texture_.Init(device, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, WIDTH, HEIGHT,
                heap_.GetCPUHandle(1), heap_.GetGPUHandle(1),
                L"Yellow Texture");
  texture_.WriteResource(device, datas.data());

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
  float value = color_constant_buffer.GetStagingRef().color[1];
  value += 0.01f;
  if (value > 1.0f) value -= 1.0f;
  color_constant_buffer.GetStagingRef().color[1] = value;
  color_constant_buffer.UpdateStaging();

  root_signature_.SetGraphicsCommandList(device);
  device.GetCommandList()->SetPipelineState(pipeline_state_.Get());
  ID3D12DescriptorHeap* heaps[] = {heap_.GetHeap()};
  device.GetCommandList()->SetDescriptorHeaps(1, heaps);

  color_constant_buffer.SetGraphicsCommandList(device, 0);
  texture_.SetGraphicsCommandList(device, 1);
  // device.GetCommandList()->SetGraphicsRootDescriptorTable(
  //    1, heap_.GetGPUHandle(1));
}

}  // namespace shader
}  // namespace directx
}  // namespace legend
