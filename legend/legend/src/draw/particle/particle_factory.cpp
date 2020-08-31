#include "src/draw/particle/particle_factory.h"

#include "assets/shaders/gpu_particle/fire_particle.h"
#include "assets/shaders/gpu_particle/player_move_particle.h"
#include "src/directx/directx_helper.h"
#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/compute_pipeline_state_desc.h"
#include "src/directx/shader/graphics_pipeline_state_desc.h"
#include "src/game/game_device.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace draw {
namespace particle {
namespace particle_factory {

std::shared_ptr<draw::particle::ParticleEmitter> CreatePlayerMoveParticle() {
  using directx::directx_helper::Failed;
  using namespace shaders::gpu_particle::player_move_particle;
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  constexpr u32 PARTICLE_MAX_NUM =
      THREAD_X * THREAD_Y * DISPATCH_X * DISPATCH_Y;
  const ParticleEmitter::ParticleConstData const_data{
      PARTICLE_MAX_NUM, sizeof(Particle), DISPATCH_X, DISPATCH_Y,
      L"PlayerMoveParticle"};

  std::vector<shaders::gpu_particle::player_move_particle::Particle> particles(
      PARTICLE_MAX_NUM);
  // for (u32 i = 0; i < PARTICLE_MAX_NUM; i++) {
  //  //particles[i].lifetime =
  //  //    game::GameDevice::GetInstance()->GetRandom().Range(10.0f, 30.0f);
  //}

  auto SetVertexShader = [&](directx::shader::GraphicsPipelineStateDesc& desc) {
    constexpr auto name =
        util::resource::resource_names::vertex_shader::PLAYER_MOVE_PARTICLE;
    auto shader = resource.GetVertexShader().Get(name).get();
    desc.SetVertexShader(shader);
  };
  auto SetPixelShader = [&](directx::shader::GraphicsPipelineStateDesc& desc) {
    constexpr auto name =
        util::resource::resource_names::pixel_shader::PLAYER_MOVE_PARTICLE;
    auto shader = resource.GetPixelShader().Get(name).get();
    desc.SetPixelShader(shader);
  };
  auto SetGeometryShader = [&](directx::shader::GraphicsPipelineStateDesc&
                                   desc) {
    constexpr auto name =
        util::resource::resource_names::geometry_shader::PLAYER_MOVE_PARTICLE;
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
        util::resource::resource_names::compute_shader::PLAYER_MOVE_PARTICLE;
    auto shader = resource.GetComputeShader().Get(name).get();
    desc.SetComputeShader(shader);
  };

  directx::shader::ComputePipelineStateDesc compute_pso_desc = {};
  SetComputeShader(compute_pso_desc);
  compute_pso_desc.SetRootSignature(device.GetDefaultRootSignature());

  auto& particle_com_list =
      game::GameDevice::GetInstance()->GetParticleCommandList();

  auto res = particle_com_list.CreateParticle<draw::particle::ParticleEmitter>(
      device.GetCurrentFrameResource()->GetCommandList(), const_data,
      particles.data(),
      util::resource::resource_names::texture::PARTICLE_SMOKE_3,
      graphics_pso_desc, compute_pso_desc);

  return res;
}

std::shared_ptr<draw::particle::ParticleEmitter> CreateEnemyMoveParticle() {
  return CreatePlayerMoveParticle();
}

std::shared_ptr<draw::particle::ParticleEmitter> CreateFireParticle() {
  using directx::directx_helper::Failed;
  using namespace shaders::gpu_particle::fire_particle;
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  constexpr u32 PARTICLE_MAX_NUM =
      THREAD_X * THREAD_Y * DISPATCH_X * DISPATCH_Y;
  const ParticleEmitter::ParticleConstData const_data{
      PARTICLE_MAX_NUM, sizeof(Particle), DISPATCH_X, DISPATCH_Y,
      L"FireParticle"};

  std::vector<shaders::gpu_particle::fire_particle::Particle> particles(
      PARTICLE_MAX_NUM);

  auto SetVertexShader = [&](directx::shader::GraphicsPipelineStateDesc& desc) {
    constexpr auto name =
        util::resource::resource_names::vertex_shader::FIRE_PARTICLE;
    auto shader = resource.GetVertexShader().Get(name).get();
    desc.SetVertexShader(shader);
  };
  auto SetPixelShader = [&](directx::shader::GraphicsPipelineStateDesc& desc) {
    constexpr auto name =
        util::resource::resource_names::pixel_shader::FIRE_PARTICLE;
    auto shader = resource.GetPixelShader().Get(name).get();
    desc.SetPixelShader(shader);
  };
  auto SetGeometryShader =
      [&](directx::shader::GraphicsPipelineStateDesc& desc) {
        constexpr auto name =
            util::resource::resource_names::geometry_shader::FIRE_PARTICLE;
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
        util::resource::resource_names::compute_shader::FIRE_PARTICLE;
    auto shader = resource.GetComputeShader().Get(name).get();
    desc.SetComputeShader(shader);
  };

  directx::shader::ComputePipelineStateDesc compute_pso_desc = {};
  SetComputeShader(compute_pso_desc);
  compute_pso_desc.SetRootSignature(device.GetDefaultRootSignature());

  auto& particle_com_list =
      game::GameDevice::GetInstance()->GetParticleCommandList();

  auto res = particle_com_list.CreateParticle<draw::particle::ParticleEmitter>(
      device.GetCurrentFrameResource()->GetCommandList(), const_data,
      particles.data(),
      util::resource::resource_names::texture::PARTICLE_SPARKING_3,
      graphics_pso_desc, compute_pso_desc);
  res->Delete(0.5f);

  return res;
}

}  // namespace particle_factory
}  // namespace particle
}  // namespace draw
}  // namespace legend