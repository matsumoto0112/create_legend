#include "src/draw/particle/smoke_particle.h"

#include "src/directx/directx_helper.h"
#include "src/directx/shader/alpha_blend_desc.h"
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

  std::vector<Particle> particles(PARTICLE_NUM);
  if (!ParticleEmitter::Init(copy_command_list, particles.data())) {
    return false;
  }

  auto& device = game::GameDevice::GetInstance()->GetDevice();

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

  {
    D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc = {};
    pso_desc.pRootSignature =
        device.GetDefaultRootSignature()->GetRootSignature();
    pso_desc.InputLayout = vs.GetInputLayout();
    pso_desc.VS = vs.GetShaderBytecode();
    pso_desc.PS = ps.GetShaderBytecode();
    pso_desc.GS = gs.GetShaderBytecode();
    pso_desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    pso_desc.BlendState.RenderTarget[0] =
        directx::shader::alpha_blend_desc::BLEND_DESC_ADD;
    pso_desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    pso_desc.DepthStencilState.DepthEnable = false;
    pso_desc.SampleDesc.Count = 1;
    pso_desc.SampleMask = UINT_MAX;
    pso_desc.NumRenderTargets = 1;
    pso_desc.RTVFormats[0] = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
    pso_desc.PrimitiveTopologyType =
        D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

    if (Failed(device.GetDevice()->CreateGraphicsPipelineState(
            &pso_desc, IID_PPV_ARGS(&graphics_pipeline_state_)))) {
      return false;
    }
  }

  {
    D3D12_COMPUTE_PIPELINE_STATE_DESC pso_desc = {};
    pso_desc.pRootSignature =
        device.GetDefaultRootSignature()->GetRootSignature();
    pso_desc.CS = cs.GetShaderBytecode();
    if (Failed(device.GetDevice()->CreateComputePipelineState(
            &pso_desc, IID_PPV_ARGS(&compute_pipeline_state_)))) {
      return false;
    }
  }

  return true;
}

bool SmokeParticle::Update(directx::device::CommandList& compute_command_list) {
  if (!ParticleEmitter::Update(compute_command_list)) {
    return false;
  }

  return true;
}

bool SmokeParticle::Render(
    directx::device::CommandList& graphics_command_list) {
    graphics_command_list.GetCommandList()->IASetPrimitiveTopology(
      D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

  if (!ParticleEmitter::Render(graphics_command_list)) {
    return false;
  }

  return true;
}

}  // namespace particle
}  // namespace draw
}  // namespace legend
