#include "src/draw/sprite_renderer.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/graphics_pipeline_state_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/directx/vertex.h"
#include "src/game/game_device.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace draw {

//コンストラクタ
SpriteRenderer::SpriteRenderer() {}

//デストラクタ
SpriteRenderer::~SpriteRenderer() {}

//初期化
bool SpriteRenderer::Init(const math::Vector2& window_size) {
  auto& device = game::GameDevice::GetInstance()->GetDevice();

  const std::vector<directx::Sprite> vertices{
      {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
      {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
      {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
      {{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}};
  if (!vertex_buffer_.Init(device, sizeof(directx::Sprite),
                           static_cast<u32>(vertices.size()),
                           L"SpriteRenderer_VertexBuffer")) {
    return false;
  }
  if (!vertex_buffer_.WriteBufferResource(vertices.data())) {
    return false;
  }

  const std::vector<u16> indices{0, 1, 2, 0, 2, 3};
  const u32 index_num = static_cast<u32>(indices.size());
  if (!index_buffer_.Init(device, sizeof(u16), index_num,
                          directx::PrimitiveTopology::TRIANGLE_LIST,
                          L"SpriteRenderer_IndexBuffer")) {
    return false;
  }
  if (!index_buffer_.WriteBufferResource(indices.data())) {
    return false;
  }

  auto& resource = game::GameDevice::GetInstance()->GetResource();
  directx::shader::GraphicsPipelineStateDesc pso_desc = {};
  pso_desc.SetVertexShader(
      resource.GetVertexShader()
          .Get(util::resource::resource_names::vertex_shader::SPRITE)
          .get());
  pso_desc.SetPixelShader(
      resource.GetPixelShader()
          .Get(util::resource::resource_names::pixel_shader::SPRITE)
          .get());

  pso_desc.BlendState.RenderTarget[0] =
      directx::shader::alpha_blend_desc::BLEND_DESC_ALIGNMENT;
  pso_desc.BlendState.AlphaToCoverageEnable = true;
  pso_desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC();
  pso_desc.NumRenderTargets = 1;
  pso_desc.PrimitiveTopologyType =
      D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
  pso_desc.pRootSignature =
      device.GetDefaultRootSignature()->GetRootSignature();
  pso_desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
  pso_desc.RTVFormats[0] = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
  pso_desc.SampleDesc.Count = 1;
  pso_desc.SampleMask = UINT_MAX;

  if (!pipeline_state_.Init(device, pso_desc)) {
    return false;
  }

  if (!world_cb_.Init(
          device, directx::shader::ConstantBufferRegisterID::WORLD_CONTEXT,
          device.GetLocalHandle(
              directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID),
          L"SpriteRenderer_WorldContextConstantBuffer")) {
    return false;
  }

  world_cb_.GetStagingRef().view = math::Matrix4x4::kIdentity;
  world_cb_.GetStagingRef().projection =
      math::Matrix4x4::CreateOrthographic(window_size);
  world_cb_.UpdateStaging();
  return true;
}

//描画リストに追加する
void SpriteRenderer::AddDrawItems(Sprite2D* sprite) {
  draw_items_.push_back(sprite);
}

//描画処理
void SpriteRenderer::DrawItems(directx::device::CommandList& command_list) {
  if (draw_items_.empty()) {
    return;
  }
  auto& device = game::GameDevice::GetInstance()->GetDevice();

  pipeline_state_.SetCommandList(command_list);
  world_cb_.SetToHeap(device);

  vertex_buffer_.SetGraphicsCommandList(command_list);
  index_buffer_.SetGraphicsCommandList(command_list);

  //各スプライトの描画情報をセットしてから描画指令を送る
  for (auto&& sp : draw_items_) {
    if (!sp) continue;
    sp->SetToGraphicsCommandList(command_list);
    device.GetHeapManager().SetHeapTableToGraphicsCommandList(device,
                                                              command_list);
    index_buffer_.Draw(command_list);
  }
  draw_items_.clear();
}

}  // namespace draw
}  // namespace legend
