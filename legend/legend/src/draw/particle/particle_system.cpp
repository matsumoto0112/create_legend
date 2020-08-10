#include "src/draw/particle/particle_system.h"

#include "src/directx/device/command_list.h"
#include "src/directx/directx_helper.h"
#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/vertex_shader.h"
#include "src/game/game_device.h"

namespace legend {
namespace draw {
namespace particle {

ParticleSystem::ParticleSystem() {}

ParticleSystem::~ParticleSystem() {}

bool ParticleSystem::Init() {
  using directx::directx_helper::Failed;

  auto& device = game::GameDevice::GetInstance()->GetDevice();

  D3D12_COMMAND_QUEUE_DESC compute_queue_desc = {};
  compute_queue_desc.Flags =
      D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
  compute_queue_desc.Type =
      D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
  if (Failed(device.GetDevice()->CreateCommandQueue(
          &compute_queue_desc, IID_PPV_ARGS(&compute_command_queue_)))) {
    return false;
  }

  if (Failed(device.GetDevice()->CreateCommandAllocator(
          D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE,
          IID_PPV_ARGS(&compute_command_alocator_)))) {
    return false;
  }

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

  directx::device::CommandList command_list;
  if (!command_list.Init(
          device, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT)) {
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

  if (Failed(device.GetDevice()->CreateCommandList(
          0, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE,
          compute_command_alocator_.Get(), nullptr,
          IID_PPV_ARGS(&compute_command_list_)))) {
    return false;
  }

  if (!transform_cb_.Init(
          device, 0,
          device.GetLocalHandle(
              directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID),
          L"")) {
    return false;
  }
  if (!world_cb_.Init(
          device, 1,
          device.GetLocalHandle(
              directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID),
          L"")) {
    return false;
  }

  {
    std::vector<Particle> particles(PARTICLE_NUM);
    constexpr u32 size = PARTICLE_NUM * sizeof(Particle);
    const D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(size);
    if (Failed(device.GetDevice()->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT),
            D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc,
            D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
            nullptr, IID_PPV_ARGS(&particle_uav_)))) {
      return false;
    }

    if (Failed(device.GetDevice()->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD),
            D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc,
            D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
            IID_PPV_ARGS(&particle_uav_upload_)))) {
      return false;
    }
    handle_ = device.GetLocalHandle(
        directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID);

    auto AlignForUAVCounter = [](u32 size) {
      const u32 alignment = D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT;
      return (size + (alignment - 1)) & ~(alignment - 1);
    };

    // D3D12_SUBRESOURCE_DATA sub_resource = {};
    // sub_resource.pData = particles.data();
    // sub_resource.RowPitch = sub_resource.SlicePitch = size;
    // UpdateSubresources(command_list.GetCommandList(), particle_uav_.Get(),
    //                   particle_uav_upload_.Get(), 0, 0, 1, &sub_resource);
    // command_list.GetCommandList()->ResourceBarrier(
    //    1, &CD3DX12_RESOURCE_BARRIER::Transition(
    //           particle_uav_.Get(),
    //           D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST,
    //           D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS));

    D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc = {};
    uav_desc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
    uav_desc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;
    uav_desc.Buffer.FirstElement = 0;
    uav_desc.Buffer.NumElements = PARTICLE_NUM;
    uav_desc.Buffer.CounterOffsetInBytes = AlignForUAVCounter(size);
    uav_desc.Buffer.Flags = D3D12_BUFFER_UAV_FLAGS::D3D12_BUFFER_UAV_FLAG_NONE;
    uav_desc.Buffer.StructureByteStride = sizeof(Particle);
    device.GetDevice()->CreateUnorderedAccessView(
        particle_uav_.Get(), nullptr, &uav_desc, handle_.cpu_handle_);
  }

  if (!command_list.Close()) {
    return false;
  }
  device.ExecuteCommandList({command_list});
  device.WaitExecute();
  return true;
}

void ParticleSystem::Execute() {}
}  // namespace particle
}  // namespace draw
}  // namespace legend