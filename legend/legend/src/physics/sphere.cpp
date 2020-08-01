#include "src/physics/sphere.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/directx/vertex.h"

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

bool Sphere::Initialize(directx::DirectX12Device& device) {
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

  //頂点バッファ作成
  if (!vertex_buffer_.Init(device, sizeof(directx::PhysicsVertex),
                           static_cast<u32>(vertices.size()),
                           L"Sphere_VertexBuffer")) {
    return false;
  }
  if (!vertex_buffer_.WriteBufferResource(vertices)) {
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

  //インデックスバッファ作成
  if (!index_buffer_.InitAndWrite(device, indices,
                                  directx::PrimitiveTopology::PointList,
                                  L"Sphere_IndexBuffer")) {
    return false;
  }

  if (!transform_constant_buffer_.Init(
          device, directx::shader::ConstantBufferRegisterID::Transform,
          device.GetLocalHeapHandle(
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

  if (!world_constant_buffer_.Init(
          device, directx::shader::ConstantBufferRegisterID::WorldContext,
          device.GetLocalHeapHandle(
              directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID),
          L"WorldContext ConstantBuffer")) {
    return false;
  }
  world_constant_buffer_.GetStagingRef().view = math::Matrix4x4::CreateView(
      math::Vector3(0, 1, -1), math::Vector3(0, 0, 0),
      math::Vector3::kUpVector);
  const float aspect = 1280.0f / 720.0f;
  world_constant_buffer_.GetStagingRef().projection =
      math::Matrix4x4::CreateProjection(45.0f, aspect, 0.1f, 100.0);
  world_constant_buffer_.UpdateStaging();

  std::filesystem::path path = util::Path::GetInstance()->shader();
  std::filesystem::path vertex_shader_path = path / L"physics" / L"obb_vs.cso";
  std::filesystem::path pixel_shader_path = path / L"physics" / L"obb_ps.cso";
  std::vector<D3D12_INPUT_ELEMENT_DESC> elements{
      {"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0,
       D3D12_APPEND_ALIGNED_ELEMENT,
       D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
       0},
      {"NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0,
       D3D12_APPEND_ALIGNED_ELEMENT,
       D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
       0}};

  //頂点シェーダー
  std::shared_ptr<directx::shader::VertexShader> vertex_shader =
      std::make_shared<directx::shader::VertexShader>();
  if (!vertex_shader->Init(game::GameDevice::GetInstance()->GetDevice(),
                           vertex_shader_path, elements)) {
    return false;
  }

  //ピクセルシェーダー
  std::shared_ptr<directx::shader::PixelShader> pixel_shader =
      std::make_shared<directx::shader::PixelShader>();
  if (!pixel_shader->Init(game::GameDevice::GetInstance()->GetDevice(),
                          pixel_shader_path)) {
    return false;
  }

  if (!pipeline_state_.Init(game::GameDevice::GetInstance()->GetDevice())) {
    return false;
  }

  //パイプライン作成開始
  pipeline_state_.SetRootSignature(device.GetDefaultRootSignature());
  pipeline_state_.SetVertexShader(vertex_shader);
  pipeline_state_.SetPixelShader(pixel_shader);
  device.GetRenderResourceManager().WriteRenderTargetInfoToPipeline(
      device, directx::render_target::RenderTargetID::BACK_BUFFER,
      &pipeline_state_);
  pipeline_state_.SetBlendDesc(
      directx::shader::alpha_blend_desc::BLEND_DESC_ALIGNMENT, 0);
  pipeline_state_.SetPrimitiveTopology(
      D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT);

  if (!pipeline_state_.CreatePipelineState(
          game::GameDevice::GetInstance()->GetDevice())) {
    return false;
  }

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

void Sphere::Draw(directx::DirectX12Device& device) {
  pipeline_state_.SetGraphicsCommandList(device);
  world_constant_buffer_.SetToHeap(device);
  transform_constant_buffer_.SetToHeap(device);
  device.GetHeapManager().CopyHeapAndSetToGraphicsCommandList(device);
  vertex_buffer_.SetGraphicsCommandList(device);
  index_buffer_.SetGraphicsCommandList(device);
  index_buffer_.Draw(device);
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