#include "src/physics/bounding_box.h"

#include "src/directx/vertex.h"

namespace legend {
namespace physics {

//コンストラクタ
BoundingBox::BoundingBox()
    : position_(math::Vector3::kZeroVector),
      rotation_(math::Vector3::kZeroVector),
      scale_(math::Vector3(0.1f, 0.1f, 0.1f)),
      directions_(3),
      lengthes_(3) {
  directions_[0] = math::Vector3::kRightVector;
  directions_[1] = math::Vector3::kUpVector;
  directions_[2] = math::Vector3::kForwardVector;

  lengthes_[0] = 1.0f;
  lengthes_[1] = 1.0f;
  lengthes_[2] = 1.0f;
}

//初期化
bool BoundingBox::Initialize(directx::DirectX12Device& device) {
  //中心座標と各軸の長さから頂点座標を設定
  float left = GetPosition().x + -GetLength(0);
  float right = GetPosition().x + GetLength(0);
  float down = GetPosition().y + -GetLength(1);
  float up = GetPosition().y + GetLength(1);
  float front = GetPosition().z + -GetLength(2);
  float back = GetPosition().z + GetLength(2);

  const std::vector<directx::BoundingBox> vertices{
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

  if (!transform_constant_buffer_.Init(
          device, 0,
          device.GetLocalHeapHandle(
              directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID),
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
  math::Vector3 position = GetPosition();
  math::Vector3 rotate = GetRotation();
  math::Vector3 scale = GetScale();
  ;
  transform_constant_buffer_.GetStagingRef().world =
      math::Matrix4x4::CreateScale(scale) *
      math::Matrix4x4::CreateRotation(rotate) *
      math::Matrix4x4::CreateTranslate(position);
  transform_constant_buffer_.UpdateStaging();
}

//更新
void BoundingBox::Draw(directx::DirectX12Device& device) {
  transform_constant_buffer_.SetToHeap(device);
  device.GetHeapManager().CopyHeapAndSetToGraphicsCommandList(device);
  vertex_buffer_.SetGraphicsCommandList(device);
  index_buffer_.SetGraphicsCommandList(device);
  index_buffer_.Draw(device);
}

//方向ベクトルを取得
math::Vector3 BoundingBox::GetDirection(i32 direction_num) {
  if (direction_num > directions_.size()) {
    MY_LOG(L"格納数よりも大きい値です");
    return math::Vector3::kZeroVector;
  }

  return directions_[direction_num];
}

//長さを取得
float BoundingBox::GetLength(i32 length_num) {
  if (length_num > lengthes_.size()) {
    MY_LOG(L"格納数よりも大きい値です");
    return 1;
  }

  return lengthes_[length_num];
}

//現在の位置を取得
math::Vector3 BoundingBox::GetPosition() { return position_; }

//現在の回転量を取得
math::Vector3 BoundingBox::GetRotation() { return rotation_; }

//現在のスケールを取得
math::Vector3 BoundingBox::GetScale() { return scale_; }

//分離軸Xを取得
math::Vector3 BoundingBox::GetAxisX() { return axis_x; }

//分離軸Yを取得
math::Vector3 BoundingBox::GetAxisY() { return axis_y; }

//分離軸Zを取得
math::Vector3 BoundingBox::GetAxisZ() { return axis_z; }

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

//回転量の設定
void BoundingBox::SetRotation(math::Vector3 rotate) { rotation_ = rotate; }

//スケールの設定
void BoundingBox::SetScale(math::Vector3 scale) { scale_ = scale; }

//分離軸の更新
void BoundingBox::SetAxis() {
  math::Matrix4x4 rotate_matrix = math::Matrix4x4::CreateRotation(rotation_);

  axis_x =
      math::Matrix4x4::MultiplyCoord(directions_[0], rotate_matrix) * scale_.x;
  axis_y =
      math::Matrix4x4::MultiplyCoord(directions_[1], rotate_matrix) * scale_.y;
  axis_z =
      math::Matrix4x4::MultiplyCoord(directions_[2], rotate_matrix) * scale_.z;
}

}  // namespace physics
}  // namespace legend