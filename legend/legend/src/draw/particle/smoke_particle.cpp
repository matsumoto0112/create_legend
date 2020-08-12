#include "src/draw/particle/smoke_particle.h"

#include "src/directx/directx_helper.h"
#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/compute_pipeline_state_desc.h"
#include "src/directx/shader/graphics_pipeline_state_desc.h"
#include "src/directx/shader/vertex_shader.h"
#include "src/game/game_device.h"
#include "src/util/path.h"

namespace legend {
namespace draw {
namespace particle {

SmokeParticle::SmokeParticle()
    : ParticleEmitter(PARTICLE_NUM, sizeof(Particle), DISPATCH_X, DISPATCH_Y,
                      L"SmokeParticle") {}

SmokeParticle::~SmokeParticle() {}

bool SmokeParticle::Init(directx::device::CommandList& copy_command_list) {
  using directx::directx_helper::Failed;
  auto& device = game::GameDevice::GetInstance()->GetDevice();

  std::vector<Particle> particles(PARTICLE_NUM);

  const auto shader_path = util::Path::GetInstance()->shader() / "particle";
  directx::shader::VertexShader vs;
  if (!vs.Init(device, shader_path / "gpu_particle_vs.cso")) {
    return false;
  }
  directx::shader::ShaderBase ps;
  if (!ps.Init(device, shader_path / "gpu_particle_ps.cso")) {
    return false;
  }
  directx::shader::ShaderBase gs;
  if (!gs.Init(device, shader_path / "gpu_particle_gs.cso")) {
    return false;
  }
  directx::shader::ShaderBase cs;
  if (!cs.Init(device, shader_path / "gpu_particle_cs.cso")) {
    return false;
  }

  directx::shader::GraphicsPipelineStateDesc graphics_pso_desc = {};
  graphics_pso_desc.SetPixelShader(&ps);
  graphics_pso_desc.SetVertexShader(&vs);
  graphics_pso_desc.SetGeometryShader(&gs);
  graphics_pso_desc.SetRootSignature(device.GetDefaultRootSignature());
  graphics_pso_desc.SetRenderTargets(
      device.GetRenderResourceManager().GetRenderTarget(
          directx::render_target::RenderTargetID::BACK_BUFFER));
  graphics_pso_desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
  graphics_pso_desc.BlendState.RenderTarget[0] =
      directx::shader::alpha_blend_desc::BLEND_DESC_ADD;
  graphics_pso_desc.DepthStencilState =
      CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
  graphics_pso_desc.DepthStencilState.DepthEnable = false;
  graphics_pso_desc.SampleDesc.Count = 1;
  graphics_pso_desc.SampleMask = UINT_MAX;
  graphics_pso_desc.PrimitiveTopologyType =
      D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

  directx::shader::ComputePipelineStateDesc compute_pso_desc = {};
  compute_pso_desc.SetRootSignature(device.GetDefaultRootSignature());
  compute_pso_desc.SetComputeShader(&cs);

  if (!ParticleEmitter::Init(copy_command_list, particles.data(),
                             graphics_pso_desc, compute_pso_desc)) {
    return false;
  }

  return true;
}

void SmokeParticle::Update(ParticleCommandList& command_list) {
  ParticleEmitter::Update(command_list);
}

void SmokeParticle::Render(
    directx::device::CommandList& graphics_command_list) {
  auto& device = game::GameDevice::GetInstance()->GetDevice();

  graphics_command_list.GetCommandList()->IASetPrimitiveTopology(
      D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
  game::GameDevice::GetInstance()
      ->GetResource()
      .GetTexture()
      .Get(util::resource::id::Texture::TEX)
      ->SetToHeap(device);

  ParticleEmitter::Render(graphics_command_list);
}

}  // namespace particle
}  // namespace draw
}  // namespace legend
