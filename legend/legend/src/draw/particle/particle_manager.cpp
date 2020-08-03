#include "src/draw/particle/particle_manager.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/vertex.h"
#include "src/game/game_device.h"

namespace {
constexpr legend::util::resource::id::VertexShader use_vertex_shader_id =
    legend::util::resource::id::VertexShader::SIMPLE_PARTICLE;
constexpr legend::util::resource::id::PixelShader use_pixel_shader_id =
    legend::util::resource::id::PixelShader::SIMPLE_PARTICLE;
}  // namespace

namespace legend {
namespace draw {
namespace particle {

ParticleManager::ParticleManager() {}

ParticleManager::~ParticleManager() {}

bool ParticleManager::Init() {
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  //共通使用する頂点バッファを作成する
  const std::vector<directx::Sprite> vertices{
      {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
      {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
      {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
      {{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}};
  if (!vertex_buffer_.Init(device, sizeof(directx::Sprite),
                           static_cast<u32>(vertices.size()),
                           L"Particle_VertexBuffer")) {
    return false;
  }
  if (!vertex_buffer_.WriteBufferResource(vertices)) {
    return false;
  }

  //共通使用するインデックスバッファを作成する
  const std::vector<u16> indices{0, 1, 2, 0, 2, 3};
  if (!index_buffer_.InitAndWrite(device, indices,
                                  directx::PrimitiveTopology::TriangleList,
                                  L"Particle_IndexBuffer")) {
    return false;
  }

  //頂点シェーダー、ピクセルシェーダーを読み込む
  util::resource::Resource& resource =
      game::GameDevice::GetInstance()->GetResource();
  const std::filesystem::path particle_root_path =
      util::Path::GetInstance()->shader() / "particle";
  if (!resource.GetVertexShader().Load(
          use_vertex_shader_id,
          particle_root_path / "simple_particle_vs.cso")) {
    return false;
  }
  if (!resource.GetPixelShader().Load(
          use_pixel_shader_id, particle_root_path / "simple_particle_ps.cso")) {
    return false;
  }

  //パーティクルパイプラインを構築する
  if (!pipeline_state_.Init(device)) {
    return false;
  }

  auto vertex_shader = resource.GetVertexShader().Get(use_vertex_shader_id);
  auto pixel_shader = resource.GetPixelShader().Get(use_pixel_shader_id);

  pipeline_state_.SetRootSignature(device.GetDefaultRootSignature());
  pipeline_state_.SetVertexShader(vertex_shader);
  pipeline_state_.SetPixelShader(pixel_shader);
  pipeline_state_.SetBlendDesc(
      directx::shader::alpha_blend_desc::BLEND_DESC_ADD, 0);
  device.GetRenderResourceManager().WriteRenderTargetInfoToPipeline(
      device, directx::render_target::RenderTargetID::BACK_BUFFER,
      &pipeline_state_);
  device.GetRenderResourceManager().WriteDepthStencilTargetInfoToPipeline(
      device, directx::render_target::DepthStencilTargetID::None,
      &pipeline_state_);

  if (!pipeline_state_.CreatePipelineState(device)) {
    return false;
  }

  return true;
}

void ParticleManager::SetDrawList(ParticleEmitter* emitter) {
  MY_ASSERTION(emitter, L"emitterがnullptrです。");
  emitters_.emplace_back(emitter);
}

void ParticleManager::DrawParticles() {
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  directx::render_target::RenderResourceManager& rr_manager =
      device.GetRenderResourceManager();
  rr_manager.SetRenderTargetID(
      directx::render_target::RenderTargetID::BACK_BUFFER);
  rr_manager.SetDepthStencilTargetID(
      directx::render_target::DepthStencilTargetID::None);
  rr_manager.SetRenderTargetsToCommandList(device);

  pipeline_state_.SetGraphicsCommandList(device);
  vertex_buffer_.SetGraphicsCommandList(device);
  index_buffer_.SetGraphicsCommandList(device);

  for (auto&& emitter : emitters_) {
    emitter->Render(index_buffer_);
  }
}

}  // namespace particle
}  // namespace draw
}  // namespace legend