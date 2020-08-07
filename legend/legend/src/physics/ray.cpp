#include "src/physics/ray.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/directx/vertex.h"
#include "src/game/game_device.h"

namespace legend {
namespace physics {

//コンストラクタ
Ray::Ray()
    : start_position_(math::Vector3::kZeroVector),
      direction_(math::Vector3::kRightVector),
      max_distance_(1) {}

//コンストラクタ
Ray::Ray(math::Vector3 direction, float max_distance)
    : start_position_(math::Vector3::kZeroVector),
      direction_(direction),
      max_distance_(max_distance) {}

//コンストラクタ
Ray::Ray(math::Vector3 start_position, math::Vector3 direction,
         float max_distance)
    : start_position_(start_position),
      direction_(direction),
      max_distance_(max_distance) {}

//デストラクタ
Ray::~Ray() {}

bool Ray::Initialize() {
  const std::vector<directx::PhysicsVertex> vertices{
      {{start_position_}},            // 0
      {{direction_ * max_distance_}}  // 1
  };

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  //頂点バッファ作成
  if (!vertex_buffer_.Init(device, sizeof(directx::PhysicsVertex),
                           static_cast<u32>(vertices.size()),
                           L"Ray_VertexBuffer")) {
    return false;
  }
  if (!vertex_buffer_.WriteBufferResource(vertices.data())) {
    return false;
  }

  const std::vector<u16> indices{0, 1};
  //インデックスバッファ作成
  const u32 index_num = static_cast<u32>(indices.size());
  if (!index_buffer_.Init(device, sizeof(u16), index_num,
                          directx::PrimitiveTopology::POINT_LIST,
                          L"Sphere_IndexBuffer")) {
    return false;
  }
  if (!index_buffer_.WriteBufferResource(indices.data())) {
    return false;
  }

  if (!transform_constant_buffer_.Init(
          device, directx::shader::ConstantBufferRegisterID::TRANSFORM,
          device.GetLocalHandle(
              directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID),
          L"Transform ConstantBuffer")) {
    return false;
  }

  math::Vector3 position = math::Vector3::kZeroVector;
  math::Vector3 rotate = math::Vector3::kZeroVector;
  math::Vector3 scale = math::Vector3::kUnitVector;
  transform_constant_buffer_.GetStagingRef().world =
      math::Matrix4x4::CreateScale(scale) *
      math::Matrix4x4::CreateRotation(rotate) *
      math::Matrix4x4::CreateTranslate(position);
  transform_constant_buffer_.UpdateStaging();

  return true;
}

void Ray::Update() {}

void Ray::Draw() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();

  transform_constant_buffer_.SetToHeap(device);
  device.GetHeapManager().SetGraphicsCommandList(command_list);
  vertex_buffer_.SetGraphicsCommandList(command_list);
  index_buffer_.SetGraphicsCommandList(command_list);
  index_buffer_.Draw(command_list);
}

//始点の取得
math::Vector3 Ray::GetStartPosition() const { return start_position_; }

//方向ベクトルの取得
math::Vector3 Ray::GetDirection() const { return direction_; }

//最大範囲の取得
float Ray::GetDistance() const { return max_distance_; }
}  // namespace physics
}  // namespace legend