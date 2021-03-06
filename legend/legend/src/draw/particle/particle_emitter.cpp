#include "src/draw/particle/particle_emitter.h"

#include "src/directx/directx_helper.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"

namespace legend {
namespace draw {
namespace particle {

ParticleEmitter::ParticleEmitter(const ParticleConstData& const_data)
    : particle_max_num_(const_data.particle_max_size),
      particle_structure_size_(const_data.particle_structure_size),
      dispatch_x_(const_data.dispatch_x),
      dispatch_y_(const_data.dispatch_y),
      name_(const_data.name),
      enable_update_(true),
      enable_render_(true),
      reset_particle_(false),
      emit_enable_(true),
      delete_(false) {}

//デストラクタ
ParticleEmitter::~ParticleEmitter() {}

//初期化
bool ParticleEmitter::Init(
    directx::device::CommandList& command_list, const void* data,
    const std::wstring& texture_name,
    const D3D12_GRAPHICS_PIPELINE_STATE_DESC& graphics_desc,
    const D3D12_COMPUTE_PIPELINE_STATE_DESC& compute_desc) {
  using directx::directx_helper::Failed;
  auto& device = game::GameDevice::GetInstance()->GetDevice();

  this->texture_name_ = texture_name;

  constexpr auto HEAP_ID =
      directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID;
  if (!transform_cb_.Init(device, device.GetLocalHandle(HEAP_ID),
                          name_ + L"Transform")) {
    return false;
  }
  if (!info_cb_.Init(device, device.GetLocalHandle(HEAP_ID),
                     name_ + L"_ParticleInfo")) {
    return false;
  }

  //パーティクルリソースの作成
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
  uav_desc.Buffer.NumElements = particle_max_num_;
  uav_desc.Buffer.StructureByteStride = particle_structure_size_;
  uav_desc.Buffer.Flags = D3D12_BUFFER_UAV_FLAGS::D3D12_BUFFER_UAV_FLAG_NONE;
  device.GetDevice()->CreateUnorderedAccessView(
      particle_uav_.Get(), particle_uav_.Get(), &uav_desc, handle_.cpu_handle_);

  vertex_buffer_view_.BufferLocation =
      particle_uav_.Get()->GetGPUVirtualAddress();
  vertex_buffer_view_.SizeInBytes = size;
  vertex_buffer_view_.StrideInBytes = particle_structure_size_;

  //パイプラインの作成
  if (!graphics_pipeline_state_.Init(device, graphics_desc)) {
    return false;
  }

  if (!compute_pipeline_state_.Init(device, compute_desc)) {
    return false;
  }

  return true;
}

//更新
void ParticleEmitter::Update(directx::device::CommandList& command_list) {
  delete_timer_.Update();
  delete_wait_timer_.Update();
  if (!enable_update_) {
    return;
  }
  if (delete_) {
    return;
  }

  auto& device = game::GameDevice::GetInstance()->GetDevice();

  //リソースを登録していく
  device.GetHeapManager().RegisterHandle(
      directx::shader::UnorderedAccessID::Particle,
      directx::shader::ResourceType::UAV, handle_);
  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();
  transform_cb_.RegisterHandle(
      device, directx::shader::ConstantBufferRegisterID::TRANSFORM);
  info_cb_.GetStagingRef().reset = reset_particle_;
  info_cb_.GetStagingRef().emit_enable = emit_enable_;
  info_cb_.UpdateStaging();
  info_cb_.RegisterHandle(
      device, directx::shader::ConstantBufferRegisterID::PARTICLE_INFO);
  device.GetHeapManager().SetHeapTableToComputeCommandList(device,
                                                           command_list);

  reset_particle_ = false;

  compute_pipeline_state_.SetCommandList(command_list);
  // CSの実行
  command_list.GetCommandList()->Dispatch(dispatch_x_, dispatch_y_, 1);
}

//描画
void ParticleEmitter::Render(
    directx::device::CommandList& graphics_command_list) {
  if (!enable_render_) {
    return;
  }
  auto& device = game::GameDevice::GetInstance()->GetDevice();

  transform_cb_.RegisterHandle(
      device, directx::shader::ConstantBufferRegisterID::TRANSFORM);
  game::GameDevice::GetInstance()
      ->GetResource()
      .GetTexture()
      .Get(texture_name_)
      ->RegisterHandle(device, directx::shader::TextureRegisterID::ALBEDO);

  graphics_command_list.GetCommandList()->ResourceBarrier(
      1, &CD3DX12_RESOURCE_BARRIER::Transition(
             particle_uav_.Get(),
             D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
             D3D12_RESOURCE_STATES::
                 D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));
  device.GetHeapManager().SetHeapTableToGraphicsCommandList(
      device, graphics_command_list);
  graphics_pipeline_state_.SetCommandList(graphics_command_list);
  graphics_command_list.GetCommandList()->IASetVertexBuffers(
      0, 1, &vertex_buffer_view_);
  graphics_command_list.GetCommandList()->IASetPrimitiveTopology(
      D3D12_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);

  graphics_command_list.GetCommandList()->DrawInstanced(particle_max_num_, 1, 0,
                                                        0);
  graphics_command_list.GetCommandList()->ResourceBarrier(
      1, &CD3DX12_RESOURCE_BARRIER::Transition(
             particle_uav_.Get(),
             D3D12_RESOURCE_STATES::
                 D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
             D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS));
}

void ParticleEmitter::SetTextureName(std::wstring_view view) {
  texture_name_ = view;
}

void ParticleEmitter::Delete() {
  enable_render_ = false;
  enable_update_ = false;
  emit_enable_ = false;
  delete_wait_timer_.Init(0.1f, [&]() { delete_ = true; });
}

void ParticleEmitter::Delete(float second) {
  delete_timer_.Init(second, [&]() { Delete(); });
}

}  // namespace particle
}  // namespace draw
}  // namespace legend