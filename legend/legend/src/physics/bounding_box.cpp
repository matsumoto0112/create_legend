#include "src/physics/bounding_box.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/directx/vertex.h"
#include "src/game/game_device.h"

namespace legend {
namespace physics {

//コンストラクタ
BoundingBox::BoundingBox()
    : Collider(util::Transform(), L"BoundingBox"),
      directions_(3),
      lengthes_(3),
      is_trigger_(false),
      is_on_ground_(true) {
  // SetScale(math::Vector3(0.1f, 0.1f, 0.1f));

  directions_[0] = math::Vector3::kRightVector;
  directions_[1] = math::Vector3::kUpVector;
  directions_[2] = math::Vector3::kForwardVector;

  lengthes_[0] = 1.0f;
  lengthes_[1] = 1.0f;
  lengthes_[2] = 1.0f;
}

BoundingBox::BoundingBox(math::Vector3 position, math::Quaternion rotation,
                         math::Vector3 scale)
    : Collider(util::Transform(position, rotation, scale), L"BoundingBox"),
      directions_(3),
      lengthes_(3),
      is_trigger_(false),
      is_on_ground_(true) {
  directions_[0] = math::Vector3::kRightVector;
  directions_[1] = math::Vector3::kUpVector;
  directions_[2] = math::Vector3::kForwardVector;

  lengthes_[0] = 1.0f;
  lengthes_[1] = 1.0f;
  lengthes_[2] = 1.0f;
}

BoundingBox::~BoundingBox() {}

//初期化
bool BoundingBox::Init() {
  if (!draw_box_.Init()) {
    return false;
  }
  return true;
}

//更新
void BoundingBox::Update() { draw_box_.SetTransform(transform_); }

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

void BoundingBox::DebugDraw(directx::device::CommandList& command_list) {
  math::Vector3 length(lengthes_[0], lengthes_[1], lengthes_[2]);
  math::Vector3 scale =
      math::Vector3::MultiplyEach(transform_.GetScale(), length);
  draw_box_.SetTransform(util::Transform(transform_.GetPosition(),
                                         transform_.GetRotation(), scale));

  draw_box_.Render(command_list);
}

}  // namespace physics
}  // namespace legend