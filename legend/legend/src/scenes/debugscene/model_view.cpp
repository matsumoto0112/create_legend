#include "src/scenes/debugscene/model_view.h"

#include "src/directx/directx_helper.h"
#include "src/directx/shader/pixel_shader.h"
#include "src/directx/shader/vertex_shader.h"
#include "src/directx/vertex.h"
#include "src/game/game_device.h"
#include "src/util/path.h"

namespace legend {
namespace scenes {
namespace debugscene {

//コンストラクタ
ModelView::ModelView(ISceneChange* scene_change) : Scene(scene_change) {}

ModelView::~ModelView() {}

//初期化
bool ModelView::Initialize() {
  if (!Scene::Initialize()) {
    return false;
  }

  auto& device = game::GameDevice::GetInstance()->GetDevice();

  {
    struct Position {
      math::Vector3 position;
    };
    const std::vector<Position> vertices{
        {math::Vector3{0.0f, 1.0f, 0.0f}},
        {math::Vector3{1.0f, 0.0f, 0.0f}},
        {math::Vector3{-1.0f, 0.0f, 0.0f}},
    };
    const u32 vertex_num = static_cast<u32>(vertices.size());
    const u32 vertex_size = sizeof(Position);
    if (!vertex_buffer_.Init(device, vertex_size, vertex_num,
                             L"VertexBuffer")) {
      return false;
    }
    if (!vertex_buffer_.WriteBufferResource(vertices.data())) {
      return false;
    }
  }

  {
    const std::vector<u32> indices = {
        0,
        1,
        2,
    };
    const u32 index_num = static_cast<u32>(indices.size());
    const u32 index_size = sizeof(u32);
    if (!index_buffer_.Init(device, index_num, index_size,
                            directx::PrimitiveTopology::TriangleList,
                            L"IndexBuffer")) {
      return true;
    }
    if (!index_buffer_.WriteBufferResource(indices.data())) {
      return false;
    }
  }

  if (!root_signature_.InitByDefault(device, L"DefaultRootSignature")) {
    return false;
  }

  {
    const std::filesystem::path shader = util::Path::GetInstance()->shader();
    directx::shader::VertexShader vs;
    if (!vs.Init(device, shader / "default" / "default_vs.cso")) {
      return false;
    }
    directx::shader::PixelShader ps;
    if (!ps.Init(device, shader / "default" / "default_ps.cso")) {
      return false;
    }

    D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc = {};
    pso_desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    pso_desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC1(D3D12_DEFAULT);
    pso_desc.DSVFormat = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
    pso_desc.Flags = D3D12_PIPELINE_STATE_FLAGS::D3D12_PIPELINE_STATE_FLAG_NONE;
    pso_desc.InputLayout = vs.GetInputLayout();
    pso_desc.NodeMask = 0;
    pso_desc.NumRenderTargets = 1;
    pso_desc.PrimitiveTopologyType =
        D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    pso_desc.pRootSignature = root_signature_.GetRootSignature();
    pso_desc.PS = ps.GetShaderBytecode();
    pso_desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    pso_desc.RTVFormats[0] = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
    pso_desc.SampleDesc.Count = 1;
    pso_desc.SampleMask = UINT_MAX;
    pso_desc.VS = vs.GetShaderBytecode();
    if (directx::directx_helper::Failed(
            device.GetDevice()->CreateGraphicsPipelineState(
                &pso_desc, IID_PPV_ARGS(&pipeline_)))) {
      return false;
    }
  }

  return true;
}

//更新
bool ModelView::Update() {
  if (!Scene::Update()) {
    return false;
  }

  return true;
}

//描画
void ModelView::Draw() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();

  directx::device::CommandList& command_list =
      device.current_resource_->command_lists_[device.MID_COMMAND_LIST_ID];
  root_signature_.SetGraphicsCommandList(command_list);
  command_list.GetCommandList()->SetPipelineState(pipeline_.Get());
  vertex_buffer_.SetGraphicsCommandList(command_list);
  index_buffer_.SetGraphicsCommandList(command_list);
  index_buffer_.Draw(command_list);
}

void ModelView::Finalize() {}

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
