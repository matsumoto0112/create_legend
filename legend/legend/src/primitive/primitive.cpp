#include "src/primitive/primitive.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"

namespace legend {
namespace primitive {

legend::primitive::PrimitiveBase::PrimitiveBase(const std::wstring& name)
    : name_(name) {}

PrimitiveBase::~PrimitiveBase() {}

void PrimitiveBase::Render(directx::device::CommandList& command_list) {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  resource.GetPipeline()
      .Get(util::resource::id::Pipeline::PRIMITIVE_LINE)
      ->SetGraphicsCommandList(command_list);

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();
  transform_cb_.SetToHeap(device);
  device.GetHeapManager().UpdateGlobalHeap(device, command_list);

  vertex_buffer_.SetGraphicsCommandList(command_list);
  index_buffer_.SetGraphicsCommandList(command_list);
  index_buffer_.Draw(command_list);
}

bool PrimitiveBase::InitBuffer(
    const std::vector<directx::PhysicsVertex>& vertices,
    const std::vector<u16>& indices) {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  const u32 vertex_num = static_cast<u32>(vertices.size());
  const u32 vertex_size = sizeof(directx::PhysicsVertex);
  if (!vertex_buffer_.Init(device, vertex_size, vertex_num,
                           name_ + L"_VertexBuffer")) {
    return false;
  }
  if (!vertex_buffer_.WriteBufferResource(vertices.data())) {
    return false;
  }

  const u32 index_num = static_cast<u32>(indices.size());
  const u32 index_size = sizeof(u16);
  if (!index_buffer_.Init(device, index_size, index_num,
                          directx::PrimitiveTopology::LINE_LIST,
                          name_ + L"_IndexBuffer")) {
    return false;
  }
  if (!index_buffer_.WriteBufferResource(indices.data())) {
    return false;
  }

  if (!transform_cb_.Init(
          device, directx::shader::ConstantBufferRegisterID::TRANSFORM,
          device.GetHeapManager().GetLocalHeap(
              directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID),
          L"")) {
    return false;
  }
  return true;
}

}  // namespace primitive
}  // namespace legend