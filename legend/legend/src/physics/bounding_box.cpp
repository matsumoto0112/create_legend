#include "src/physics/bounding_box.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/directx/vertex.h"

namespace legend {
namespace physics {

//コンストラクタ
BoundingBox::BoundingBox()
    : transform_(),
      directions_(3),
      lengthes_(3),
      is_trigger_(false),
      is_on_ground_(true) {
  //SetScale(math::Vector3(0.1f, 0.1f, 0.1f));

  directions_[0] = math::Vector3::kRightVector;
  directions_[1] = math::Vector3::kUpVector;
  directions_[2] = math::Vector3::kForwardVector;

  lengthes_[0] = 1.0f;
  lengthes_[1] = 1.0f;
  lengthes_[2] = 1.0f;
}

BoundingBox::BoundingBox(math::Vector3 position, math::Quaternion rotation,
                         math::Vector3 scale)
    : transform_(),
      directions_(3),
      lengthes_(3),
      is_trigger_(false),
      is_on_ground_(true) {
  SetPosition(position);
  SetRotation(rotation);
  SetScale(scale);

  directions_[0] = math::Vector3::kRightVector;
  directions_[1] = math::Vector3::kUpVector;
  directions_[2] = math::Vector3::kForwardVector;

  lengthes_[0] = 1.0f;
  lengthes_[1] = 1.0f;
  lengthes_[2] = 1.0f;
}

BoundingBox::~BoundingBox() {}

//初期化
bool BoundingBox::Initialize(directx::DirectX12Device& device) {
  //中心座標と各軸の長さから頂点座標を設定
  float left = GetPosition().x - GetLength(0);
  float right = GetPosition().x + GetLength(0);
  float down = GetPosition().y - GetLength(1);
  float up = GetPosition().y + GetLength(1);
  float front = GetPosition().z - GetLength(2);
  float back = GetPosition().z + GetLength(2);

  const std::vector<directx::PhysicsVertex> vertices{
      {{left, down, front}},   // 0
      {{left, down, back}},    // 1
      {{right, down, back}},   // 2
      {{right, down, front}},  // 3
      {{left, up, front}},     // 4
      {{left, up, back}},      // 5
      {{right, up, back}},     // 6
      {{right, up, front}}     // 7
  };

  //頂点バッファ作成
  if (!vertex_buffer_.Init(device, sizeof(directx::PhysicsVertex),
                           static_cast<u32>(vertices.size()),
                           L"BoundingBox_VertexBuffer")) {
    return false;
  }
  if (!vertex_buffer_.WriteBufferResource(vertices)) {
    return false;
  }

  const std::vector<u16> indices{0, 1, 1, 2, 2, 3, 3, 0, 0, 4, 4, 7, 7, 3,

                                 4, 5, 5, 6, 6, 7,

                                 5, 1, 6, 2};
  //インデックスバッファ作成
  if (!index_buffer_.InitAndWrite(device, indices,
                                  directx::PrimitiveTopology::LineList,
                                  L"Bounding_IndexBuffer")) {
    return false;
  }

  if (!transform_constant_buffer_.Init(
          device, directx::shader::ConstantBufferRegisterID::Transform,
          device.GetLocalHeapHandle(directx::descriptor_heap::heap_parameter::
                                        LocalHeapID::PHYSICS_TEST),
          L"Transform ConstantBuffer")) {
    return false;
  }

  math::Vector3 position = math::Vector3::kZeroVector;
  math::Vector3 rotate = math::Vector3::kZeroVector;
  math::Vector3 scale = math::Vector3::kUnitVector;
  transform_constant_buffer_.GetStagingRef().world =
      transform_.CreateWorldMatrix();
  transform_constant_buffer_.UpdateStaging();

  if (!world_constant_buffer_.Init(
          device, directx::shader::ConstantBufferRegisterID::WorldContext,
          device.GetLocalHeapHandle(directx::descriptor_heap::heap_parameter::
                                        LocalHeapID::PHYSICS_TEST),
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
      D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE);

  if (!pipeline_state_.CreatePipelineState(
          game::GameDevice::GetInstance()->GetDevice())) {
    return false;
  }

  return true;
}

//更新
void BoundingBox::Update() {
  math::Vector3 position = GetPosition();
  math::Vector3 rotate = GetRotation().ToEular() * math::util::RAD_2_DEG;
  math::Vector3 scale = GetScale();
  transform_constant_buffer_.GetStagingRef().world =
      transform_.CreateWorldMatrix();
  transform_constant_buffer_.UpdateStaging();
}

//描画
void BoundingBox::Draw(directx::DirectX12Device& device) {
  pipeline_state_.SetGraphicsCommandList(device);
  world_constant_buffer_.SetToHeap(device);
  transform_constant_buffer_.SetToHeap(device);
  device.GetHeapManager().CopyHeapAndSetToGraphicsCommandList(device);
  vertex_buffer_.SetGraphicsCommandList(device);
  index_buffer_.SetGraphicsCommandList(device);
  index_buffer_.Draw(device);
}

//方向ベクトルを取得
math::Vector3 BoundingBox::GetDirection(i32 direction_num) const {
  if (direction_num > directions_.size()) {
    MY_LOG(L"格納数よりも大きい値です");
    return math::Vector3::kZeroVector;
  }

  return directions_[direction_num];
}

//長さを取得
float BoundingBox::GetLength(i32 length_num) const {
  if (length_num > lengthes_.size()) {
    MY_LOG(L"格納数よりも大きい値です");
    return 1;
  }

  return lengthes_[length_num];
}

//スケール倍した長さを取得
float BoundingBox::GetLengthByScale(i32 length_num) const {
  if (length_num > lengthes_.size()) {
    MY_LOG(L"格納数よりも大きい値です");
    return 1;
  }

  float scale;
  if (length_num == 0)
    scale = GetScale().x;
  else if (length_num == 1)
    scale = GetScale().y;
  else
    scale = GetScale().z;

  return lengthes_[length_num] * scale;
}

//現在の位置を取得
math::Vector3 BoundingBox::GetPosition() const {
  return transform_.GetPosition();
}

//現在の回転量を取得
math::Quaternion BoundingBox::GetRotation() const {
  return transform_.GetRotation();
}

//現在のスケールを取得
math::Vector3 BoundingBox::GetScale() const { return transform_.GetScale(); }

//分離軸Xを取得
math::Vector3 BoundingBox::GetAxisX() const { return axis_x; }

//分離軸Yを取得
math::Vector3 BoundingBox::GetAxisY() const { return axis_y; }

//分離軸Zを取得
math::Vector3 BoundingBox::GetAxisZ() const { return axis_z; }

//トリガー判定の取得
bool BoundingBox::GetIsTrigger() const { return is_trigger_; }

//接地判定の取得
bool BoundingBox::GetOnGround() const { return is_on_ground_; }

//各方向ベクトルの設定
void BoundingBox::SetDirection(math::Vector3 direction_x,
                               math::Vector3 direction_y,
                               math::Vector3 direction_z) {
  directions_[0] = direction_x;
  directions_[1] = direction_y;
  directions_[2] = direction_z;
}

//各方向の長さの設定
void BoundingBox::SetLength(float length_x, float length_y, float length_z) {
  lengthes_[0] = length_x;
  lengthes_[1] = length_y;
  lengthes_[2] = length_z;
}

//中心座標の更新
void BoundingBox::SetPosition(math::Vector3 position) {
  transform_.SetPosition(position);
}

//回転量の設定
void BoundingBox::SetRotation(math::Quaternion rotate) {
  transform_.SetRotation(rotate);
}

//スケールの設定
void BoundingBox::SetScale(math::Vector3 scale) { transform_.SetScale(scale); }

//分離軸の更新
void BoundingBox::SetAxis() {
  math::Matrix4x4 rotate_matrix = math::Matrix4x4::CreateRotation(
      GetRotation().ToEular() * math::util::RAD_2_DEG);

  axis_x = math::Matrix4x4::MultiplyCoord(directions_[0], rotate_matrix) *
           GetScale().x;
  axis_y = math::Matrix4x4::MultiplyCoord(directions_[1], rotate_matrix) *
           GetScale().y;
  axis_z = math::Matrix4x4::MultiplyCoord(directions_[2], rotate_matrix) *
           GetScale().z;
}

//トリガー設定
void BoundingBox::SetIsTrigger(bool trigger) { is_trigger_ = trigger; }

//接地判定の設定
void BoundingBox::SetOnGround(bool is_ground) { is_on_ground_ = is_ground; }

}  // namespace physics
}  // namespace legend