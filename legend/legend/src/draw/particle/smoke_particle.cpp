#include "src/draw/particle/smoke_particle.h"

#include "src/directx/directx_helper.h"
#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/compute_pipeline_state_desc.h"
#include "src/directx/shader/graphics_pipeline_state_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/directx/shader/vertex_shader.h"
#include "src/game/game_device.h"
#include "src/util/path.h"
#include "src/util/resource/resource_names.h"

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
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  std::vector<Particle> particles(PARTICLE_NUM);

  auto SetVertexShader = [&](directx::shader::GraphicsPipelineStateDesc& desc) {
    constexpr auto name =
        util::resource::resource_names::vertex_shader::GPU_PARTICLE;
    auto shader = resource.GetVertexShader().Get(name).get();
    desc.SetVertexShader(shader);
  };
  auto SetPixelShader = [&](directx::shader::GraphicsPipelineStateDesc& desc) {
    constexpr auto name =
        util::resource::resource_names::pixel_shader::GPU_PARTICLE;
    auto shader = resource.GetPixelShader().Get(name).get();
    desc.SetPixelShader(shader);
  };
  auto SetGeometryShader =
      [&](directx::shader::GraphicsPipelineStateDesc& desc) {
        constexpr auto name =
            util::resource::resource_names::geometry_shader::GPU_PARTICLE;
        auto shader = resource.GetGeometryShader().Get(name).get();
        desc.SetGeometryShader(shader);
      };

  directx::shader::GraphicsPipelineStateDesc graphics_pso_desc = {};
  SetVertexShader(graphics_pso_desc);
  SetPixelShader(graphics_pso_desc);
  SetGeometryShader(graphics_pso_desc);
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

  auto SetComputeShader = [&](directx::shader::ComputePipelineStateDesc& desc) {
    constexpr auto name =
        util::resource::resource_names::compute_shader::GPU_PARTICLE;
    auto shader = resource.GetComputeShader().Get(name).get();
    desc.SetComputeShader(shader);
  };

  directx::shader::ComputePipelineStateDesc compute_pso_desc = {};
  SetComputeShader(compute_pso_desc);
  compute_pso_desc.SetRootSignature(device.GetDefaultRootSignature());

  if (!ParticleEmitter::Init(copy_command_list, particles.data(),
                             graphics_pso_desc, compute_pso_desc)) {
    return false;
  }

  return true;
}

void SmokeParticle::Update(directx::device::CommandList& compute_command_list) {
  ParticleEmitter::Update(compute_command_list);
}

void SmokeParticle::Render(
    directx::device::CommandList& graphics_command_list) {
  auto& device = game::GameDevice::GetInstance()->GetDevice();

  graphics_command_list.GetCommandList()->IASetPrimitiveTopology(
      D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
  game::GameDevice::GetInstance()
      ->GetResource()
      .GetTexture()
      .Get(util::resource::resource_names::texture::TEX)
      ->RegisterHandle(device, directx::shader::TextureRegisterID::ALBEDO);

  ParticleEmitter::Render(graphics_command_list);
}

}  // namespace particle
}  // namespace draw
}  // namespace legend
