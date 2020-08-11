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
      D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE;
  if (Failed(device.GetDevice()->CreateCommandQueue(
          &compute_queue_desc, IID_PPV_ARGS(&compute_command_queue_)))) {
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

  for (auto&& res : frame_resources_) {
    res.Init(device, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE);
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

  D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
  heap_desc.NumDescriptors = 1;
  heap_desc.Type =
      D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
  heap_desc.Flags =
      D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
  if (Failed(device.GetDevice()->CreateDescriptorHeap(&heap_desc,
                                                      IID_PPV_ARGS(&heaps_)))) {
    return false;
  }

  {
    auto AlignForUAVCounter = [](u32 size) {
      const u32 alignment = D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT;
      return (size + (alignment - 1)) & ~(alignment - 1);
    };

    std::vector<Particle> particles(PARTICLE_NUM);
    auto& random = game::GameDevice::GetInstance()->GetRandom();
    for (u32 i = 0; i < PARTICLE_NUM; i++) {
      const float x = random.Range(-5.0f, 5.0f);
      const float y = 0.0f;
      const float z = 0.0f;
      particles[i].position = math::Vector3(x, y, z);
    }
    const u32 size = sizeof(Particle) * PARTICLE_NUM;
    const u32 aligned_size =
        AlignForUAVCounter(sizeof(Particle) * PARTICLE_NUM);
    if (Failed(device.GetDevice()->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(
                D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
                D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0),
            D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(
                size, D3D12_RESOURCE_FLAGS::
                          D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS),
            D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST, nullptr,
            IID_PPV_ARGS(&particle_uav_)))) {
      return false;
    }

    if (Failed(device.GetDevice()->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD),
            D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(size),
            D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
            IID_PPV_ARGS(&particle_uav_upload_)))) {
      return false;
    }
    handle_ = directx::descriptor_heap::DescriptorHandle{
        heaps_->GetCPUDescriptorHandleForHeapStart(),
        heaps_->GetGPUDescriptorHandleForHeapStart()};

    D3D12_SUBRESOURCE_DATA sub_resource = {};
    sub_resource.pData = particles.data();
    sub_resource.RowPitch = sub_resource.SlicePitch = size;
    UpdateSubresources(command_list.GetCommandList(), particle_uav_.Get(),
                       particle_uav_upload_.Get(), 0, 0, 1, &sub_resource);
    command_list.GetCommandList()->ResourceBarrier(
        1, &CD3DX12_RESOURCE_BARRIER::Transition(
               particle_uav_.Get(),
               D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST,
               D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS));

    D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc = {};
    uav_desc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
    uav_desc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;
    uav_desc.Buffer.FirstElement = 0;
    uav_desc.Buffer.NumElements = PARTICLE_NUM;
    uav_desc.Buffer.StructureByteStride = sizeof(Particle);
    uav_desc.Buffer.Flags = D3D12_BUFFER_UAV_FLAGS::D3D12_BUFFER_UAV_FLAG_NONE;
    device.GetDevice()->CreateUnorderedAccessView(
        particle_uav_.Get(), particle_uav_.Get(), &uav_desc,
        handle_.cpu_handle_);

    vertex_buffer_view_.BufferLocation =
        particle_uav_.Get()->GetGPUVirtualAddress();
    vertex_buffer_view_.SizeInBytes = size;
    vertex_buffer_view_.StrideInBytes = sizeof(Particle);
  }

  {
    D3D12_INDIRECT_ARGUMENT_DESC argumentDescs[1] = {};
    argumentDescs[0].Type =
        D3D12_INDIRECT_ARGUMENT_TYPE::D3D12_INDIRECT_ARGUMENT_TYPE_DRAW;

    D3D12_COMMAND_SIGNATURE_DESC comDesc{};
    comDesc.pArgumentDescs = argumentDescs;
    comDesc.NumArgumentDescs = _countof(argumentDescs);
    comDesc.ByteStride = sizeof(IndirectCommand);

    if (Failed(device.GetDevice()->CreateCommandSignature(
            &comDesc, nullptr, IID_PPV_ARGS(&command_signature_)))) {
      return false;
    }
  }
  {
    command_.draw_argument.VertexCountPerInstance = PARTICLE_NUM;
    command_.draw_argument.InstanceCount = 1;
    if (Failed(device.GetDevice()->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD),
            D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(sizeof(IndirectCommand)),
            D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
            IID_PPV_ARGS(&command_resource_)))) {
      return false;
    }
    void* data;
    if (Failed(command_resource_->Map(0, nullptr, &data))) {
      return false;
    }
    memcpy(data, &command_, sizeof(command_));
    command_resource_->Unmap(0, nullptr);
  }

  if (!command_list.Close()) {
    return false;
  }
  device.ExecuteCommandList({command_list});
  device.WaitExecute();

  if (Failed(device.GetDevice()->CreateFence(
          fence_value_, D3D12_FENCE_FLAGS::D3D12_FENCE_FLAG_NONE,
          IID_PPV_ARGS(&fence_)))) {
    return false;
  }
  fence_value_++;
  return true;
}

void ParticleSystem::Execute() {
  using directx::directx_helper::Failed;
  transform_cb_.GetStagingRef().world = math::Matrix4x4::kIdentity;
  transform_cb_.UpdateStaging();
  world_cb_.GetStagingRef().view = math::Matrix4x4::CreateView(
      math::Vector3(0.0f, 0.0f, -5.0f), math::Vector3(0.0f, 0.0f, 0.0f),
      math::Vector3::kUpVector);
  world_cb_.GetStagingRef().projection = math::Matrix4x4::CreateProjection(
      45.0f * math::util::DEG_2_RAD, 1280.0f / 720.0f, 0.1f, 200.0f);
  world_cb_.UpdateStaging();

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& graphics_command_list =
      device.GetCurrentFrameResource()->GetCommandList();

  current_resource_ = &frame_resources_[device.GetRenderResourceManager()
                                            .GetCurrentBackBufferIndex()];
  const UINT64 last_completed_fence = fence_->GetCompletedValue();

  if (current_resource_->fence_value_ > last_completed_fence) {
    HANDLE event_handle = CreateEvent(nullptr, false, false, nullptr);
    if (!event_handle) {
      return;
    }
    if (Failed(fence_->SetEventOnCompletion(current_resource_->fence_value_,
                                            event_handle))) {
      return;
    }
    WaitForSingleObject(event_handle, INFINITE);
    CloseHandle(event_handle);
  }
  current_resource_->Ready();

  ID3D12DescriptorHeap* heaps[] = {heaps_.Get()};

  current_resource_->GetCommandList().GetCommandList()->SetDescriptorHeaps(
      1, heaps);
  current_resource_->GetCommandList().GetCommandList()->SetComputeRootSignature(
      device.GetDefaultRootSignature()->GetRootSignature());
  current_resource_->GetCommandList()
      .GetCommandList()
      ->SetComputeRootDescriptorTable(
          2, heaps_->GetGPUDescriptorHandleForHeapStart());
  current_resource_->GetCommandList().GetCommandList()->SetPipelineState(
      compute_pipeline_state_.Get());
  current_resource_->GetCommandList().GetCommandList()->Dispatch(DISPATCH_X,
                                                                 DISPATCH_Y, 1);

  current_resource_->GetCommandList().Close();
  ID3D12CommandList* command_lists[] = {
      current_resource_->GetCommandList().GetCommandList()};
  compute_command_queue_->ExecuteCommandLists(1, command_lists);

  const UINT64 fence_to_wait_for = fence_value_;
  if (Failed(compute_command_queue_->Signal(fence_.Get(), fence_to_wait_for))) {
    return;
  }
  fence_value_++;

  HANDLE fence_event = CreateEvent(nullptr, false, false, nullptr);
  if (Failed(fence_->SetEventOnCompletion(fence_to_wait_for, fence_event))) {
    return;
  }
  WaitForSingleObject(fence_event, INFINITE);
  CloseHandle(fence_event);

  world_cb_.SetToHeap(device);
  transform_cb_.SetToHeap(device);
  device.GetHeapManager().UpdateGlobalHeap(device, graphics_command_list);
  graphics_command_list.GetCommandList()->SetPipelineState(
      graphics_pipeline_state_.Get());
  graphics_command_list.GetCommandList()->IASetVertexBuffers(
      0, 1, &vertex_buffer_view_);
  graphics_command_list.GetCommandList()->IASetPrimitiveTopology(
      D3D12_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
  graphics_command_list.GetCommandList()->ExecuteIndirect(
      command_signature_.Get(), 1, command_resource_.Get(), 0, nullptr, 0);
}
}  // namespace particle
}  // namespace draw
}  // namespace legend