#include "src/physics/bounding_box.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/directx/vertex.h"
#include "src/game/game_device.h"

namespace legend {
namespace physics {

//�R���X�g���N�^
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

//������
bool BoundingBox::Init() {
  //���S���W�Ɗe���̒������璸�_���W��ݒ�
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

  if (!InitVertexBuffer(vertices)) {
    return false;
  }

  const std::vector<u16> indices{0, 1, 1, 2, 2, 3, 3, 0, 0, 4, 4, 7, 7, 3,

                                 4, 5, 5, 6, 6, 7,

                                 5, 1, 6, 2};
  if (!InitIndexBuffer(indices)) {
    return false;
  }

  if (!InitTransformConstantBuffer()) {
    return false;
  }

  return true;
}

//�X�V
void BoundingBox::Update() {
  math::Vector3 position = GetPosition();
  math::Vector3 rotate = GetRotation().ToEular() * math::util::RAD_2_DEG;
  math::Vector3 scale = GetScale();
  transform_constant_buffer_.GetStagingRef().world =
      transform_.CreateWorldMatrix();
  transform_constant_buffer_.UpdateStaging();
}

//�����x�N�g�����擾
math::Vector3 BoundingBox::GetDirection(i32 direction_num) const {
  if (direction_num > directions_.size()) {
    MY_LOG(L"�i�[�������傫���l�ł�");
    return math::Vector3::kZeroVector;
  }

  return directions_[direction_num];
}

//�������擾
float BoundingBox::GetLength(i32 length_num) const {
  if (length_num > lengthes_.size()) {
    MY_LOG(L"�i�[�������傫���l�ł�");
    return 1;
  }

  return lengthes_[length_num];
}

//�X�P�[���{�����������擾
float BoundingBox::GetLengthByScale(i32 length_num) const {
  if (length_num > lengthes_.size()) {
    MY_LOG(L"�i�[�������傫���l�ł�");
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

//���݂̈ʒu���擾
math::Vector3 BoundingBox::GetPosition() const {
  return transform_.GetPosition();
}

//���݂̉�]�ʂ��擾
math::Quaternion BoundingBox::GetRotation() const {
  return transform_.GetRotation();
}

//���݂̃X�P�[�����擾
math::Vector3 BoundingBox::GetScale() const { return transform_.GetScale(); }

//������X���擾
math::Vector3 BoundingBox::GetAxisX() const { return axis_x; }

//������Y���擾
math::Vector3 BoundingBox::GetAxisY() const { return axis_y; }

//������Z���擾
math::Vector3 BoundingBox::GetAxisZ() const { return axis_z; }

//�g���K�[����̎擾
bool BoundingBox::GetIsTrigger() const { return is_trigger_; }

//�ڒn����̎擾
bool BoundingBox::GetOnGround() const { return is_on_ground_; }

//�e�����x�N�g���̐ݒ�
void BoundingBox::SetDirection(math::Vector3 direction_x,
                               math::Vector3 direction_y,
                               math::Vector3 direction_z) {
  directions_[0] = direction_x;
  directions_[1] = direction_y;
  directions_[2] = direction_z;
}

//�e�����̒����̐ݒ�
void BoundingBox::SetLength(float length_x, float length_y, float length_z) {
  lengthes_[0] = length_x;
  lengthes_[1] = length_y;
  lengthes_[2] = length_z;
}

//���S���W�̍X�V
void BoundingBox::SetPosition(math::Vector3 position) {
  transform_.SetPosition(position);
}

//��]�ʂ̐ݒ�
void BoundingBox::SetRotation(math::Quaternion rotate) {
  transform_.SetRotation(rotate);
}

//�X�P�[���̐ݒ�
void BoundingBox::SetScale(math::Vector3 scale) { transform_.SetScale(scale); }

//�������̍X�V
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

//�g���K�[�ݒ�
void BoundingBox::SetIsTrigger(bool trigger) { is_trigger_ = trigger; }

//�ڒn����̐ݒ�
void BoundingBox::SetOnGround(bool is_ground) { is_on_ground_ = is_ground; }

}  // namespace physics
}  // namespace legend