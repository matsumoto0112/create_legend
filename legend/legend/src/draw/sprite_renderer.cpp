#include "src/draw/sprite_renderer.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/directx/vertex.h"
#include "src/game/game_device.h"

namespace legend {
namespace draw {

//コンストラクタ
SpriteRenderer::SpriteRenderer() {}

//デストラクタ
SpriteRenderer::~SpriteRenderer() {}

//初期化
bool SpriteRenderer::Init(const math::Vector2& window_size) {
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

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
  if (!vertex_buffer_.WriteBufferResource(vertices)) {
    return false;
  }

  const std::vector<u16> indices{0, 1, 2, 0, 2, 3};
  if (!index_buffer_.InitAndWrite(device, indices,
                                  directx::PrimitiveTopology::TriangleList,
                                  L"SpriteRenderer_IndexBuffer")) {
    return false;
  }

  auto vertex_shader = std::make_shared<directx::shader::VertexShader>();
  const std::filesystem::path vertex_shader_path =
      util::Path::GetInstance()->shader() / "draw2d" / "sprite_vs.cso";
  if (!vertex_shader->Init(
          device, vertex_shader_path,
          directx::input_element::GetElementDescs<directx::Sprite>())) {
    return false;
  }

  const std::filesystem::path pixel_shader_path =
      util::Path::GetInstance()->shader() / "draw2d" / "sprite_ps.cso";
  auto pixel_shader = std::make_shared<directx::shader::PixelShader>();
  if (!pixel_shader->Init(device, pixel_shader_path)) {
    return false;
  }

  if (!pipeline_state_.Init(device)) {
    return false;
  }
  pipeline_state_.SetRootSignature(device.GetDefaultRootSignature());
  pipeline_state_.SetVertexShader(vertex_shader);
  pipeline_state_.SetPixelShader(pixel_shader);
  pipeline_state_.SetBlendDesc(
      directx::shader::alpha_blend_desc::BLEND_DESC_ALIGNMENT, 0);
  device.GetRenderResourceManager().WriteRenderTargetInfoToPipelineDesc(
      device, 0, pipeline_state_);

  if (!pipeline_state_.CreatePipelineState(device)) {
    return false;
  }

  if (!world_cb_.Init(
          device, directx::shader::ConstantBufferRegisterID::WorldContext,
          device.GetLocalHeapHandle(0), L"Sprite_WorldContextConstantBuffer")) {
    return false;
  }

  world_cb_.GetStagingRef().view = math::Matrix4x4::kIdentity;
  world_cb_.GetStagingRef().projection =
      math::Matrix4x4::CreateOrthographic(window_size);
  world_cb_.UpdateStaging();
  return true;
}

void SpriteRenderer::AddDrawItems(Sprite2D* sprite) {
  draw_items_.push_back(sprite);
}

void SpriteRenderer::DrawItems() {
  if (draw_items_.empty()) {
    return;
  }
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  pipeline_state_.SetGraphicsCommandList(device);
  world_cb_.SetToHeap(device);

  vertex_buffer_.SetGraphicsCommandList(device);
  index_buffer_.SetGraphicsCommandList(device);
  for (auto&& sp : draw_items_) {
    if (!sp) continue;
    sp->SetToCommandList();
    device.GetHeapManager().CopyHeapAndSetToGraphicsCommandList(device);
    index_buffer_.Draw(device);
  }
  draw_items_.clear();
}

}  // namespace draw
}  // namespace legend
