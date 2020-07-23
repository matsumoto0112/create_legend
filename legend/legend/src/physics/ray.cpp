#include "src/physics/ray.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/vertex.h"

namespace legend {
namespace physics {

//コンストラクタ
Ray::Ray()
    : start_position_(math::Vector3::kZeroVector),
      direction_(math::Vector3::kForwardVector),
      max_distance_(5) {}

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

bool Ray::Initialize(directx::DirectX12Device& device) {
  const std::vector<directx::PhysicsVertex> vertices{
      {{start_position_}},            // 0
      {{direction_ * max_distance_}}  // 1
  };

  //頂点バッファ作成
  if (!vertex_buffer_.Init(device, sizeof(directx::PhysicsVertex),
                           static_cast<u32>(vertices.size()),
                           L"Ray_VertexBuffer")) {
    return false;
  }
  if (!vertex_buffer_.WriteBufferResource(vertices)) {
    return false;
  }

  const std::vector<u16> indices{0, 1};
  //インデックスバッファ作成
  if (!index_buffer_.InitAndWrite(device, indices,
                                  directx::PrimitiveTopology::LineList,
                                  L"Ray_IndexBuffer")) {
    return false;
  }

  if (!transform_constant_buffer_.Init(device, 0,
                                       L"Transform ConstantBuffer")) {
    return false;
  }

  math::Vector3 position = math::Vector3::kZeroVector;
  math::Vector3 rotate = math::Vector3::kZeroVector;
  math::Vector3 scale = math::Vector3::kUnitVector * 0.5f;
  transform_constant_buffer_.GetStagingRef().world =
      math::Matrix4x4::CreateScale(scale) *
      math::Matrix4x4::CreateRotation(rotate) *
      math::Matrix4x4::CreateTranslate(position);
  transform_constant_buffer_.UpdateStaging();

  if (!world_constant_buffer_.Init(device, 1, L"WorldContext ConstantBuffer")) {
    return false;
  }

  world_constant_buffer_.GetStagingRef().view = math::Matrix4x4::CreateView(
      math::Vector3(0, 1, -1), math::Vector3(0, 0, 0),
      math::Vector3::kUpVector);
  const float aspect = 1280.0f / 720.0f;
  world_constant_buffer_.GetStagingRef().projection =
      math::Matrix4x4::CreateProjection(45.0f, aspect, 0.1f, 100.0);
  world_constant_buffer_.UpdateStaging();

  //ルートシグネチャ作成
  root_signature_ = std::make_shared<directx::shader::RootSignature>();
  if (!root_signature_->Init(game::GameDevice::GetInstance()->GetDevice(),
                             L"Global Root Signature")) {
    return false;
  }

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
  pipeline_state_.SetRootSignature(root_signature_);
  pipeline_state_.SetVertexShader(vertex_shader);
  pipeline_state_.SetPixelShader(pixel_shader);
  pipeline_state_.SetRenderTargetInfo(device.GetRenderTarget(), true);
  pipeline_state_.SetBlendDesc(
      directx::shader::alpha_blend_desc::BLEND_DESC_ALIGNMENT, 0);
  pipeline_state_.SetPrimitiveTopology(
      D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE);

  if (!pipeline_state_.CreatePipelineState(
          game::GameDevice::GetInstance()->GetDevice())) {
    return false;
  }
  return true;
}

void Ray::Update() {}

void Ray::Draw(directx::DirectX12Device& device) {
  root_signature_->SetGraphicsCommandList(device);
  pipeline_state_.SetGraphicsCommandList(device);
  device.GetHeapManager().SetGraphicsCommandList(device);
  world_constant_buffer_.SetToHeap(device);

  transform_constant_buffer_.SetToHeap(device);
  device.GetHeapManager().CopyHeapAndSetToGraphicsCommandList(device);
  vertex_buffer_.SetGraphicsCommandList(device);
  index_buffer_.SetGraphicsCommandList(device);
  index_buffer_.Draw(device);
}

//始点の取得
math::Vector3 Ray::GetStartPosition() const { return start_position_; }

//方向ベクトルの取得
math::Vector3 Ray::GetDirection() const { return direction_; }

//最大範囲の取得
float Ray::GetDistance() const { return max_distance_; }
}  // namespace physics
}  // namespace legend