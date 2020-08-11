#include "src/physics/sphere.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/directx/vertex.h"
#include "src/game/game_device.h"

namespace legend {
namespace physics {

//コンストラクタ
Sphere::Sphere()
    : position_(math::Vector3::kZeroVector),
      rotation_(math::Quaternion::kIdentity),
      scale_(0.1f),
      radius_(2) {}

Sphere::Sphere(math::Vector3 position, math::Quaternion rotation)
    : position_(position), rotation_(rotation), scale_(0.1f), radius_(2) {}

//デストラクタ
Sphere::~Sphere() {}

bool Sphere::Initialize() {
  std::vector<directx::PhysicsVertex> vertices(u_max * (v_max + 1));
  for (i32 v = 0; v < v_max; v++) {
    for (i32 u = 0; u < u_max; u++) {
      float theta = math::util::DEG_2_RAD * (180.0f * v / v_max);
      float phi = math::util::DEG_2_RAD * (360.0f * u / u_max);
      float x = math::util::Sin(theta) * math::util::Cos(phi);
      float y = math::util::Cos(theta);
      float z = math::util::Sin(theta) * math::util::Sin(phi);
      vertices[u_max * v + u].position = math::Vector3(x, y, z);
    }
  }

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  //頂点バッファ作成
  if (!vertex_buffer_.Init(device, sizeof(directx::PhysicsVertex),
                           static_cast<u32>(vertices.size()),
                           L"Sphere_VertexBuffer")) {
    return false;
  }
  if (!vertex_buffer_.WriteBufferResource(vertices.data())) {
    return false;
  }

  std::vector<u16> indices(2 * v_max * (u_max + 1));
  i32 i = 0;
  for (i32 v = 0; v < v_max; v++) {
    for (i32 u = 0; u < u_max; u++) {
      if (u == u_max) {
        indices[i] = v * u_max;
        i++;
        indices[i] = (v + 1) * u_max;
        i++;
      } else {
        indices[i] = (v * u_max) + u;
        i++;
        indices[i] = indices[i - 1] + u_max;
        i++;
      }
    }
  }

  const u32 index_num = static_cast<u32>(indices.size());
  //インデックスバッファ作成
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
  math::Vector3 scale = math::Vector3(0.1f, 0.1f, 0.1f) * radius_;
  transform_constant_buffer_.GetStagingRef().world =
      math::Matrix4x4::CreateScale(scale) *
      math::Matrix4x4::CreateRotation(rotate) *
      math::Matrix4x4::CreateTranslate(position);
  transform_constant_buffer_.UpdateStaging();

  return true;
}

void Sphere::Update() {
  math::Vector3 position = GetPosition();
  math::Vector3 rotate = GetRotation().ToEular() * math::util::RAD_2_DEG;
  math::Vector3 scale = math::Vector3(GetRadius(), GetRadius(), GetRadius());
  transform_constant_buffer_.GetStagingRef().world =
      math::Matrix4x4::CreateScale(scale) *
      math::Matrix4x4::CreateRotation(rotate) *
      math::Matrix4x4::CreateTranslate(position);
  transform_constant_buffer_.UpdateStaging();
}

void Sphere::Draw() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();

  transform_constant_buffer_.SetToHeap(device);
  device.GetHeapManager().SetCommandList(command_list);
  vertex_buffer_.SetGraphicsCommandList(command_list);
  index_buffer_.SetGraphicsCommandList(command_list);
  index_buffer_.Draw(command_list);
}

//座標の取得
math::Vector3 Sphere::GetPosition() const { return position_; }

//回転量の取得
math::Quaternion Sphere::GetRotation() const { return rotation_; }

//スケールの取得
float Sphere::GetScale() const { return scale_; }

//半径の取得
float Sphere::GetRadius() const { return radius_ * scale_; }

//半径の2乗を取得
float Sphere::GetSquareRadius() const { return GetRadius() * GetRadius(); }

//座標の設定
void Sphere::SetPosition(math::Vector3 position) { position_ = position; }

//回転量の設定
void Sphere::SetRotation(math::Quaternion rotate) { rotation_ = rotate; }

//スケールの設定
void Sphere::SetScale(float scale) { scale_ = scale; }

//半径の設定
void Sphere::SetRadius(float radius) { radius_ = radius; }
}  // namespace physics
}  // namespace legend