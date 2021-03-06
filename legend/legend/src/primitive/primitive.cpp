#include "src/primitive/primitive.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace primitive {

//コンストラクタ
PrimitiveBase::PrimitiveBase(const std::wstring& name)
    : name_(name), color_(0.0f, 1.0f, 0.0f, 1.0f) {}

//デストラクタ
PrimitiveBase::~PrimitiveBase() {}

//描画
void PrimitiveBase::Render(directx::device::CommandList& command_list) {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  resource.GetPipeline()
      .Get(util::resource::resource_names::pipeline::OBB)
      ->SetCommandList(command_list);

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();
  transform_cb_.RegisterHandle(
      device, directx::shader::ConstantBufferRegisterID::TRANSFORM);

  color_cb_.GetStagingRef().color = color_;
  color_cb_.UpdateStaging();
  color_cb_.RegisterHandle(device,
                           directx::shader::ConstantBufferRegisterID::COLOR);

  device.GetHeapManager().SetHeapTableToGraphicsCommandList(device,
                                                            command_list);

  vertex_buffer_.SetGraphicsCommandList(command_list);
  index_buffer_.SetGraphicsCommandList(command_list);
  index_buffer_.Draw(command_list);
}

//バッファの初期化
bool PrimitiveBase::InitBuffer(
    const std::vector<directx::PhysicsVertex>& vertices,
    const std::vector<u16>& indices,
    directx::descriptor_heap::heap_parameter::LocalHeapID heap_id) {
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

  if (!transform_cb_.Init(device, device.GetHeapManager().GetLocalHeap(heap_id),
                          name_ + L"_TransformConstantBuffer")) {
    return false;
  }
  if (!color_cb_.Init(device, device.GetHeapManager().GetLocalHeap(heap_id),
                      name_ + L"_ColorConstantBuffer")) {
    return false;
  }

  return true;
}

}  // namespace primitive
}  // namespace legend