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
      length_(math::Vector3::kUnitVector),
      is_trigger_(false),
      is_on_ground_(true) {
  directions_[0] = math::Vector3::kRightVector;
  directions_[1] = math::Vector3::kUpVector;
  directions_[2] = math::Vector3::kForwardVector;
}

BoundingBox::BoundingBox(math::Vector3 position, math::Quaternion rotation,
                         math::Vector3 scale)
    : Collider(util::Transform(position, rotation, scale), L"BoundingBox"),
      directions_(3),
      length_(math::Vector3::kUnitVector),
      is_trigger_(false),
      is_on_ground_(true) {
  directions_[0] = math::Vector3::kRightVector;
  directions_[1] = math::Vector3::kUpVector;
  directions_[2] = math::Vector3::kForwardVector;
}

BoundingBox::~BoundingBox() {}

//������
bool BoundingBox::Init() {
  //�`��p��Box������������
  if (!draw_box_.Init()) {
    return false;
  }
  return true;
}

//�X�V
void BoundingBox::Update() { draw_box_.SetTransform(transform_); }

//�����x�N�g�����擾
math::Vector3 BoundingBox::GetDirection(i32 direction_num) const {
  if (direction_num > directions_.size()) {
    MY_LOG(L"�i�[�������傫���l�ł�");
    return math::Vector3::kZeroVector;
  }

  return directions_[direction_num];
}

//�������擾
math::Vector3 BoundingBox::GetLength() const { return length_; }

//�X�P�[���{�����������擾
math::Vector3 BoundingBox::GetLengthByScale() const {
  return math::Vector3::MultiplyEach(length_, GetScale());
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
math::Vector3 BoundingBox::GetAxisX() const { return axis_x_; }

//������Y���擾
math::Vector3 BoundingBox::GetAxisY() const { return axis_y_; }

//������Z���擾
math::Vector3 BoundingBox::GetAxisZ() const { return axis_z_; }

//�g���K�[����̎擾
bool BoundingBox::GetIsTrigger() const { return is_trigger_; }

//�ڒn����̎擾
bool BoundingBox::GetOnGround() const { return is_on_ground_; }

//�e�����x�N�g���̐ݒ�
void BoundingBox::SetDirection(const math::Vector3& direction_x,
                               const math::Vector3& direction_y,
                               const math::Vector3& direction_z) {
  directions_[0] = direction_x;
  directions_[1] = direction_y;
  directions_[2] = direction_z;
}

void BoundingBox::SetLength(const math::Vector3& length) { length_ = length; }

//�e�����̒����̐ݒ�
void BoundingBox::SetLength(const float& length_x, const float& length_y,
                            const float& length_z) {
  length_.x = length_x;
  length_.y = length_y;
  length_.z = length_z;
}

//���S���W�̍X�V
void BoundingBox::SetPosition(const math::Vector3& position) {
  transform_.SetPosition(position);
}

//��]�ʂ̐ݒ�
void BoundingBox::SetRotation(const math::Quaternion& rotate) {
  transform_.SetRotation(rotate);
}

//�X�P�[���̐ݒ�
void BoundingBox::SetScale(const math::Vector3& scale) {
  transform_.SetScale(scale);
}

//�������̍X�V
void BoundingBox::SetAxis() {
  math::Matrix4x4 rotate_matrix = GetRotation().ToMatrix();

  axis_x_ = math::Matrix4x4::MultiplyCoord(directions_[0], rotate_matrix) *
            GetScale().x;
  axis_y_ = math::Matrix4x4::MultiplyCoord(directions_[1], rotate_matrix) *
            GetScale().y;
  axis_z_ = math::Matrix4x4::MultiplyCoord(directions_[2], rotate_matrix) *
            GetScale().z;
}

//�g���K�[�ݒ�
void BoundingBox::SetIsTrigger(bool trigger) { is_trigger_ = trigger; }

//�ڒn����̐ݒ�
void BoundingBox::SetOnGround(bool is_ground) { is_on_ground_ = is_ground; }

void BoundingBox::DebugDraw(directx::device::CommandList& command_list) {
  //�X�P�[�����O�͕ӂ̒��� * �X�P�[�����O�䗦
  const math::Vector3 scale =
      math::Vector3::MultiplyEach(transform_.GetScale(), length_);
  draw_box_.SetTransform(util::Transform(transform_.GetPosition(),
                                         transform_.GetRotation(), scale));

  draw_box_.Render(command_list);
}

}  // namespace physics
}  // namespace legend