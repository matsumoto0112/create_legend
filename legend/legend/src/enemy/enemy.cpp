#include "src/enemy/enemy.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/util/path.h"
#include "src/util/resource/pixel_shader.h"
#include "src/util/resource/vertex_shader.h"

namespace legend {
namespace enemy {
//�R���X�g���N�^
Enemy::Enemy() : Parent(L"Enemy"), velocity_(math::Vector3::kZeroVector) {
  is_move_ = false;
  // deceleration_x_ = deceleration_z_ = 0;
}

//�f�X�g���N�^
Enemy::~Enemy() {}

//������
bool Enemy::Init(const InitializeParameter& parameter) {
  if (!Parent::InitBuffer()) return false;
  // if (!obb_.Initialize(device)) {
  //  return false;
  //}
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  this->transform_ = parameter.transform;
  this->collision_.SetPosition(transform_.GetPosition());
  this->collision_.SetRotation(transform_.GetRotation());
  this->collision_.SetScale(transform_.GetScale());
  this->collision_.SetLength(parameter.bouding_box_length);

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  model_ = resource.GetModel().Get(util::resource::id::Model::ERASER);

  move_end_ = false;

  return true;
}

//�X�V
bool Enemy::Update() {
  // obb_.Update();

  update_time_ =
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();

  if (is_move_ && velocity_ == math::Vector3::kZeroVector) move_end_ = true;
  is_move_ = (0.01f < velocity_.Magnitude());
  // Move();

  // transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  // transform_cb_.UpdateStaging();

  return true;
}

//�ړ�
void Enemy::Move() {
  if (!is_move_) return;

  //�ړ����������߂�
  float length =
      math::util::Sqrt(velocity_.x * velocity_.x + velocity_.z * velocity_.z);

  //���ۂɓ�������
  float x = -velocity_.x / length;
  float z = -velocity_.z / length;

  ////�����v�Z
  // deceleration_x_ = x / (length * length);
  // deceleration_z_ = z / (length * length);

  //�ړ�����
  math::Vector3 v = math::Vector3(x, 0, z);
  math::Vector3 position = GetPosition() + v * power_ * update_time_;
  SetPosition(position);

  // Deceleration(2);
}

void Enemy::SetPosition(math::Vector3 position) {
  transform_.SetPosition(position);
  collision_.SetPosition(position);
}

//���x�̐ݒ�
void Enemy::SetVelocity(math::Vector3 velocity) { velocity_ = velocity; }

void Enemy::SetRotation() {
  math::Quaternion rotation = transform_.GetRotation();
  rotation.y += velocity_.x;
  transform_.SetRotation(rotation);
}

//�ړ��ɕK�v�ȃp�����[�^��������
void Enemy::ResetParameter() {
  if (velocity_.Magnitude() != 0.0f) return;

  // deceleration_x_ = deceleration_z_ = 0;
  is_move_ = false;
}

////����
// void Enemy::Deceleration(float deceleration_rate) {
//  float x = deceleration_x_ * deceleration_rate * update_time_;
//  float z = deceleration_z_ * deceleration_rate * update_time_;
//
//  if ((x <= velocity_.x && velocity_.x <= 0) ||
//      (0 <= velocity_.x && velocity_.x <= x)) {
//    velocity_.x = 0;
//  } else {
//    velocity_.x -= x;
//  }
//  if ((z <= velocity_.z && velocity_.z <= 0) ||
//      (0 <= velocity_.z && velocity_.z <= z)) {
//    velocity_.z = 0;
//  } else {
//    velocity_.z -= z;
//  }
//}

//���W�̎擾
math::Vector3 Enemy::GetPosition() const { return transform_.GetPosition(); }

//�ړ��ʂ̎擾
math::Vector3 Enemy::GetVelocity() const { return velocity_; }

math::Quaternion Enemy::GetRotation() const { return transform_.GetRotation(); }

float Enemy::GetPower() const { return power_; }

bool Enemy::GetMoveEnd() const { return move_end_; }

void Enemy::ResetMoveEnd() { move_end_ = false; }

}  // namespace enemy
}  // namespace legend