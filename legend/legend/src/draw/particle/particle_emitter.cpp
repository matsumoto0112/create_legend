#include "src/draw/particle/particle_emitter.h"

#include "src/directx/directx_helper.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"

namespace legend {
namespace draw {
namespace particle {
ParticleEmitter::ParticleEmitter(u32 particle_max_size,
                                 u32 particle_structure_size, u32 dispatch_x,
                                 u32 dispatch_y, const std::wstring& name)
    : particle_max_num_(particle_max_size),
      particle_structure_size_(particle_structure_size),
      dispatch_x_(dispatch_x),
      dispatch_y_(dispatch_y),
      name_(name) {}

ParticleEmitter::~ParticleEmitter() {}

bool ParticleEmitter::Init(directx::device::CommandList& copy_command_list,
                           const void* data,
                           D3D12_GRAPHICS_PIPELINE_STATE_DESC graphics_desc,
                           D3D12_COMPUTE_PIPELINE_STATE_DESC compute_desc) {
  using directx::directx_helper::Failed;
  auto& device = game::GameDevice::GetInstance()->GetDevice();

  const u32 transform_id = directx::shader::ConstantBufferRegisterID::TRANSFORM;
  const directx::descriptor_heap::DescriptorHandle transform_handle =
      device.GetLocalHandle(
          directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID);
  if (!transform_cb_.Init(device, transform_id, transform_handle,
                          name_ + L"Transform")) {
    return false;
  }

  const u32 size = particle_max_num_ * particle_structure_size_;
  if (Failed(device.GetDevice()->CreateCommittedResource(
          &CD3DX12_HEAP_PROPERTIES(
              D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
              D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0),
          D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
          &CD3DX12_RESOURCE_DESC::Buffer(
              size,
              D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS),
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
  handle_ = device.GetLocalHandle(
      directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID);

  D3D12_SUBRESOURCE_DATA sub_resource = {};
  sub_resource.pData = data;
  sub_resource.RowPitch = sub_resource.SlicePitch = size;
  UpdateSubresources(copy_command_list.GetCommandList(), particle_uav_.Get(),
                     particle_uav_upload_.Get(), 0, 0, 1, &sub_resource);
  copy_command_list.GetCommandList()->ResourceBarrier(
      1, &CD3DX12_RESOURCE_BARRIER::Transition(
             particle_uav_.Get(),
             D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST,
             D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS));

  D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc = {};
  uav_desc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
  uav_desc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;
  uav_desc.Buffer.FirstElement = 0;
  uav_desc.Buffer.NumElements = particle_max_num_;
  uav_desc.Buffer.StructureByteStride = particle_structure_size_;
  uav_desc.Buffer.Flags = D3D12_BUFFER_UAV_FLAGS::D3D12_BUFFER_UAV_FLAG_NONE;
  device.GetDevice()->CreateUnorderedAccessView(
      particle_uav_.Get(), particle_uav_.Get(), &uav_desc, handle_.cpu_handle_);

  vertex_buffer_view_.BufferLocation =
      particle_uav_.Get()->GetGPUVirtualAddress();
  vertex_buffer_view_.SizeInBytes = size;
  vertex_buffer_view_.StrideInBytes = particle_structure_size_;

  if (!graphics_pipeline_state_.Init(device, graphics_desc)) {
    return false;
  }

  if (!compute_pipeline_state_.Init(device, compute_desc)) {
    return false;
  }

  return true;
}

void ParticleEmitter::Update(
    directx::device::CommandList& compute_command_list) {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& graphics_command_list =
      device.GetCurrentFrameResource()->GetCommandList();

  device.GetHeapManager().RegisterHandle(
      directx::shader::UnorderedAccessID::Particle,
      directx::shader::ResourceType::UAV, handle_);
  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();
  transform_cb_.SetToHeap(device);
  device.GetHeapManager().SetHeapTableToComputeCommandList(
      device, compute_command_list);
  compute_pipeline_state_.SetCommandList(compute_command_list);
  compute_command_list.GetCommandList()->Dispatch(dispatch_x_, dispatch_y_, 1);
}

void ParticleEmitter::Render(
    directx::device::CommandList& graphics_command_list) {
  auto& device = game::GameDevice::GetInstance()->GetDevice();

  game::GameDevice::GetInstance()
      ->GetResource()
      .GetTexture()
      .Get(util::resource::id::Texture::TEX)
      ->SetToHeap(device);
  device.GetHeapManager().SetHeapTableToGraphicsCommandList(
      device, graphics_command_list);
  graphics_pipeline_state_.SetCommandList(graphics_command_list);
  graphics_command_list.GetCommandList()->IASetVertexBuffers(
      0, 1, &vertex_buffer_view_);
  graphics_command_list.GetCommandList()->DrawInstanced(particle_max_num_, 1, 0,
                                                        0);
}

}  // namespace particle
}  // namespace draw
}  // namespace legend