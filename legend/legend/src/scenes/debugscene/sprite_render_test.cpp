#include "src/scenes/debugscene/sprite_render_test.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/directx/vertex.h"
#include "src/game/game_device.h"
#include "src/util/path.h"

namespace legend {
namespace scenes {
namespace debugscene {

SpriteRenderTest::SpriteRenderTest(ISceneChange* scene_change)
    : Scene(scene_change) {}

SpriteRenderTest::~SpriteRenderTest() {}

bool SpriteRenderTest::Initialize() {
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  const std::vector<directx::Sprite> vertices{
      {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
      {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
      {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
      {{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}};
  if (!vertex_buffer_.Init(device, sizeof(directx::Sprite),
                           static_cast<u32>(vertices.size()),
                           L"Sprite_VertexBuffer")) {
    return false;
  }
  if (!vertex_buffer_.WriteBufferResource(vertices)) {
    return false;
  }

  const std::vector<u16> indices{0, 1, 2, 0, 2, 3};
  if (!index_buffer_.InitAndWrite(device, indices,
                                  directx::PrimitiveTopology::TriangleList,
                                  L"Sprite_IndexBuffer")) {
    return false;
  }

  const std::filesystem::path texture_path =
      util::Path::GetInstance()->texture() / "tex.png";
  if (!texture_.InitAndWrite(device, 0, texture_path)) {
    return false;
  }

  auto vertex_shader = std::make_shared<directx::shader::VertexShader>();
  const std::filesystem::path vertex_shader_path =
      util::Path::GetInstance()->shader() / "draw2d" / "sprite_vs.cso";
  const std::vector<D3D12_INPUT_ELEMENT_DESC> layouts{
      {"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0,
       D3D12_APPEND_ALIGNED_ELEMENT,
       D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
       0},
      {"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0,
       D3D12_APPEND_ALIGNED_ELEMENT,
       D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
       0},
  };
  if (!vertex_shader->Init(device, vertex_shader_path, layouts)) {
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
  pipeline_state_.SetRenderTargetInfo(device.GetRenderTarget(), true);

  if (!pipeline_state_.CreatePipelineState(device)) {
    return false;
  }

  if (!transform_cb_.Init(device,
                          directx::shader::ConstantBufferRegisterID::Transform,
                          L"Transform_WorldContextConstantBuffer")) {
    return false;
  }

  const math::Vector3 scale(128.0f, 128.0f, 128.0f);
  transform_cb_.GetStagingRef().world = math::Matrix4x4::CreateScale(scale);
  transform_cb_.UpdateStaging();

  if (!world_cb_.Init(device,
                      directx::shader::ConstantBufferRegisterID::WorldContext,
                      L"Sprite_WorldContextConstantBuffer")) {
    return false;
  }

  world_cb_.GetStagingRef().view = math::Matrix4x4::kIdentity;
  world_cb_.GetStagingRef().projection =
      math::Matrix4x4::CreateOrthographic(math::Vector2(1280, 720));
  world_cb_.UpdateStaging();
  return true;
}  // namespace debugscene

bool SpriteRenderTest::Update() {
  if (ImGui::Begin("Camera")) {
    static float camera_position[3];
    ImGui::SliderFloat3("position", camera_position, -100.0f, 100.0f);
    world_cb_.GetStagingRef().view = math::Matrix4x4::CreateTranslate(
        -1 * math::Vector3(camera_position[0], camera_position[1],
                           camera_position[2]));
    world_cb_.UpdateStaging();
  }
  ImGui::End();
  return true;
}

void SpriteRenderTest::Draw() {
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();
  device.GetHeapManager().SetGraphicsCommandList(device);
  pipeline_state_.SetGraphicsCommandList(device);
  texture_.SetToHeap(device);
  world_cb_.SetToHeap(device);
  transform_cb_.SetToHeap(device);
  device.GetHeapManager().CopyHeapAndSetToGraphicsCommandList(device);

  vertex_buffer_.SetGraphicsCommandList(device);
  index_buffer_.SetGraphicsCommandList(device);
  index_buffer_.Draw(device);
}

void SpriteRenderTest::Finalize() {}

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend