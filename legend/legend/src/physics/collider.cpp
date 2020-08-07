#include "src/physics/collider.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"

namespace legend {
namespace physics {

Collider::Collider(const util::Transform& transform, const std::wstring& name)
    : name_(name), transform_(transform) {}

Collider::~Collider() {}

bool Collider::Init() { return true; }

void Collider::Update() {}

void Collider::DebugDraw() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  resource.GetPipeline()
      .Get(util::resource::id::Pipeline::OBJECT_WIREFRAME)
      ->SetGraphicsCommandList(command_list);

  transform_constant_buffer_.SetToHeap(device);
  device.GetHeapManager().UpdateGlobalHeap(device, command_list);
  vertex_buffer_.SetGraphicsCommandList(command_list);
  index_buffer_.SetGraphicsCommandList(command_list);
  index_buffer_.Draw(command_list);
}

bool Collider::InitVertexBuffer(
    const std::vector<directx::PhysicsVertex>& vertices) {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  //頂点バッファ作成
  if (!vertex_buffer_.Init(device, sizeof(directx::PhysicsVertex),
                           static_cast<u32>(vertices.size()),
                           name_ + L"_VertexBuffer")) {
    return false;
  }
  if (!vertex_buffer_.WriteBufferResource(vertices.data())) {
    return false;
  }
  return true;
}

bool Collider::InitIndexBuffer(const std::vector<u16>& indices) {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  const u32 index_num = static_cast<u32>(indices.size());
  //インデックスバッファ作成
  if (!index_buffer_.Init(device, sizeof(u16), index_num,
                          directx::PrimitiveTopology::LINE_LIST,
                          name_ + L"_IndexBuffer")) {
    return false;
  }
  if (!index_buffer_.WriteBufferResource(indices.data())) {
    return false;
  }

  return true;
}

bool Collider::InitTransformConstantBuffer() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();

  // TODO: ヒープIDはそのシーン内のIDを使用する
  if (!transform_constant_buffer_.Init(
          device, directx::shader::ConstantBufferRegisterID::TRANSFORM,
          device.GetLocalHandle(
              directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID),
          L"Transform ConstantBuffer")) {
    return false;
  }

  transform_constant_buffer_.GetStagingRef().world =
      transform_.CreateWorldMatrix();
  transform_constant_buffer_.UpdateStaging();
  return true;
}

}  // namespace physics
}  // namespace legend