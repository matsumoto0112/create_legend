#include "src/directx/bullet_debug_draw.h"

#include "src/directx/directx_helper.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"

namespace legend {
namespace directx {

BulletDebugDraw::BulletDebugDraw() {}

BulletDebugDraw::~BulletDebugDraw() {}

bool BulletDebugDraw::Init(ID3D12Device* device) {
  using directx_helper::Failed;
  constexpr u32 SIZE = sizeof(Line) * MAX_LINE_NUM;

  if (Failed(device->CreateCommittedResource(
          &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT),
          D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
          &CD3DX12_RESOURCE_DESC::Buffer(SIZE),
          D3D12_RESOURCE_STATES::
              D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
          nullptr, IID_PPV_ARGS(&vertex_buffer_)))) {
    return false;
  }
  vertex_buffer_->SetName(L"BulletDebugDraw_VertexBuffer");

  if (Failed(device->CreateCommittedResource(
          &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD),
          D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
          &CD3DX12_RESOURCE_DESC::Buffer(SIZE),
          D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
          IID_PPV_ARGS(&vertex_buffer_upload_)))) {
    return false;
  }

  auto& dev = game::GameDevice::GetInstance()->GetDevice();
  world_cb_.Init(
      dev,
      dev.GetLocalHandle(
          directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID),
      L"BulletDebugDraw_World_ConstantBuffer");
  index_ = 0;
  return true;
}

void BulletDebugDraw::Render(const math::Matrix4x4& view,
                             const math::Matrix4x4& projection,
                             device::CommandList& command_list) {
  if (lines_.empty()) return;
  const u32 SIZE = sizeof(Line) * index_;
  D3D12_SUBRESOURCE_DATA sub_resource = {};
  sub_resource.pData = lines_.data();
  sub_resource.RowPitch = sub_resource.SlicePitch = SIZE;
  command_list.GetCommandList()->ResourceBarrier(
      1, &CD3DX12_RESOURCE_BARRIER::Transition(
             vertex_buffer_.Get(),
             D3D12_RESOURCE_STATES::
                 D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
             D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST));
  UpdateSubresources<1>(command_list.GetCommandList(), vertex_buffer_.Get(),
                        vertex_buffer_upload_.Get(), 0, 0, 1, &sub_resource);
  command_list.GetCommandList()->ResourceBarrier(
      1, &CD3DX12_RESOURCE_BARRIER::Transition(
             vertex_buffer_.Get(),
             D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST,
             D3D12_RESOURCE_STATES::
                 D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

  const u32 vert_num = static_cast<u32>(lines_.size());
  D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view = {};
  vertex_buffer_view.BufferLocation = vertex_buffer_->GetGPUVirtualAddress();
  vertex_buffer_view.SizeInBytes = vert_num * sizeof(Line);
  vertex_buffer_view.StrideInBytes = sizeof(LineVertex);

  game::GameDevice::GetInstance()
      ->GetResource()
      .GetPipeline()
      .Get(L"bullet_debug_draw")
      ->SetCommandList(command_list);
  command_list.GetCommandList()->IASetPrimitiveTopology(
      D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_LINELIST);
  world_cb_.GetStagingRef().view = view;
  world_cb_.GetStagingRef().projection = projection;
  world_cb_.UpdateStaging();

  auto& dev = game::GameDevice::GetInstance()->GetDevice();
  world_cb_.RegisterHandle(
      dev, directx::shader::ConstantBufferRegisterID::WORLD_CONTEXT);
  dev.GetHeapManager().SetHeapTableToGraphicsCommandList(dev, command_list);

  command_list.GetCommandList()->IASetVertexBuffers(0, 1, &vertex_buffer_view);
  command_list.GetCommandList()->DrawInstanced(vert_num * 2, 1, 0, 0);
  index_ = 0;
}

void BulletDebugDraw::drawLine(const btVector3& from, const btVector3& to,
                               const btVector3& fromColor,
                               const btVector3& toColor) {
  if (index_ >= MAX_LINE_NUM) return;
  lines_[index_++] = {from, fromColor, to, toColor};
}

void BulletDebugDraw::drawLine(const btVector3& from, const btVector3& to,
                               const btVector3& color) {
  drawLine(from, to, color, color);
}

}  // namespace directx
}  // namespace legend