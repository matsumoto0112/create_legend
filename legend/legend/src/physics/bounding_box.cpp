#include "src/physics/bounding_box.h"

#include "src/directx/vertex.h"

namespace legend {
namespace physics {

//コンストラクタ
BoundingBox::BoundingBox()
    : position_(math::Vector3::kZeroVector),
      rotation_(math::Vector3::kZeroVector),
      scale_(math::Vector3::kUnitVector),
      directions_(3),
      lengthes_(3) {
  directions_[0] = math::Vector3::kRightVector;
  directions_[1] = math::Vector3::kUpVector;
  directions_[2] = math::Vector3::kForwardVector;

  lengthes_[0] = 1.0f;
  lengthes_[1] = 1.0f;
  lengthes_[2] = 1.0f;
}

bool BoundingBox::Initialize(directx::DirectX12Device& device) {
  //適当な頂点を作っての描画
  const std::vector<directx::BoundingBox> vertices{
      {{-1.0f, -1.0f, 1.0f}},   // 0
      {{-1.0f, -1.0f, -1.0f}},  // 1
      {{1.0f, -1.0f, -1.0f}},   // 2
      {{1.0f, -1.0f, 1.0f}},    // 3
      {{-1.0f, 1.0f, 1.0f}},    // 4
      {{-1.0f, 1.0f, -1.0f}},   // 5
      {{1.0f, 1.0f, -1.0f}},    // 6
      {{1.0f, 1.0f, 1.0f}}      // 7
  };

  //頂点バッファ作成
  if (!vertex_buffer_.Init(device, sizeof(directx::BoundingBox),
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

  return true;
}

void BoundingBox::Update() {
  math::Vector3 position = this->GetPosition();
  math::Vector3 rotate = this->GetRotation();
  math::Vector3 scale = this->GetScale();
  ;
  transform_constant_buffer_.GetStagingRef().world =
      math::Matrix4x4::CreateScale(scale) *
      math::Matrix4x4::CreateRotation(rotate) *
      math::Matrix4x4::CreateTranslate(position);
  transform_constant_buffer_.UpdateStaging();
}

void BoundingBox::Draw(directx::DirectX12Device& device) {
  transform_constant_buffer_.SetToHeap(device);
  device.GetHeapManager().CopyHeapAndSetToGraphicsCommandList(device);
  vertex_buffer_.SetGraphicsCommandList(device);
  index_buffer_.SetGraphicsCommandList(device);
  index_buffer_.Draw(device);
}

//方向ベクトルを取得
math::Vector3 BoundingBox::GetDirection(i32 direction_num) {
  return directions_[direction_num];
}

//長さを取得
float BoundingBox::GetLength(i32 length_num) { return lengthes_[length_num]; }

//現在の位置を取得
math::Vector3 BoundingBox::GetPosition() { return position_; }

math::Vector3 BoundingBox::GetRotation() { return rotation_; }

math::Vector3 BoundingBox::GetScale() { return scale_; }

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
void BoundingBox::SetPosition(math::Vector3 position) { position_ = position; }

void BoundingBox::SetRotation(math::Vector3 rotate) { rotation_ = rotate; }

void BoundingBox::SetScale(math::Vector3 scale) { scale_ = scale; }

}  // namespace physics
}  // namespace legend