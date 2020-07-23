#include "src/physics/plane.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/vertex.h"

namespace legend {
namespace physics {
//コンストラクタ
Plane::Plane()
    : position_(math::Vector3::kZeroVector), normal_(math::Vector3(0, 1, 0)) {}

Plane::~Plane() {}

//初期化
bool Plane::Initialize(directx::DirectX12Device& device) {
  float front = GetPosition().z - 1;
  float back = GetPosition().z + 1;
  float left = GetPosition().x - 1;
  float right = GetPosition().x + 1;

  const std::vector<directx::PhysicsVertex> vertices{
      {{left, 0, front}},
      {{right, 0, front}},
      {{right, 0, back}},
      {{left, 0, back}},
  };

  //頂点バッファ作成
  if (!vertex_buffer_.Init(device, sizeof(directx::PhysicsVertex),
                           static_cast<u32>(vertices.size()),
                           L"Plane_VertexBuffer")) {
    return false;
  }
  if (!vertex_buffer_.WriteBufferResource(vertices)) {
    return false;
  }

  const std::vector<u16> indices{0, 1, 1, 2, 2, 3, 3, 0};
  //インデックスバッファ作成
  if (!index_buffer_.InitAndWrite(device, indices,
                                  directx::PrimitiveTopology::LineList,
                                  L"Plane_IndexBuffer")) {
    return false;
  }

  if (!transform_constant_buffer_.Init(device, 0,
                                       L"Transform ConstantBuffer")) {
    return false;
  }

  math::Vector3 position = math::Vector3::kZeroVector;
  transform_constant_buffer_.GetStagingRef().world =
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
       0} };

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

//描画
void Plane::Draw(directx::DirectX12Device& device) {
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

//位置の設定
void Plane::SetPosition(math::Vector3 position) { position_ = position; }

//法線ベクトルの設定
void Plane::SetNormal(math::Vector3 normal) { normal_ = normal; }

//位置の取得
math::Vector3 Plane::GetPosition() const { return position_; }

//法線ベクトルの取得
math::Vector3 Plane::GetNormal() const { return normal_; }
}  // namespace physics
}  // namespace legend