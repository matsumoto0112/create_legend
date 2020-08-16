#include "src/system/physics_field.h"

#include "src/physics/collision.h"

namespace legend {
namespace system {

//�R���X�g���N�^
PhysicsField::PhysicsField() : gravity_(-20.0f) {}

//�f�X�g���N�^
PhysicsField::~PhysicsField() {}

bool PhysicsField::Init() {
  // desk_obbs_.clear();
  // enemy_obbs_.clear();
  // is_enemy_move_.clear();
  // enemy_deceleration_x_.clear();
  // enemy_deceleration_z_.clear();

  return true;
}

//�X�V
bool PhysicsField::Update(Mode turn, math::Vector3 player_vel,
                          float player_impulse, float player_power,
                          std::vector<math::Vector3> enemies_vel) {
  update_time_ =
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();

  if (turn == Mode::PLAYER_MOVING) {
    PlayerMove(player_vel, player_impulse, player_power);
  } else {
    EnemyMove(enemies_vel);
  }

  //�v���C���[�Ɗe���Ƃ̏Փ˔���𒲂ׂ�
  bool is_on_ground = false;
  for (i32 i = 0; i < desk_obbs_.size(); i++) {
    if (is_on_ground) break;

    if (physics::Collision::GetInstance()->Collision_OBB_DeskOBB(
            player_obb_, desk_obbs_[i])) {
      // MY_LOG(L"�v���C���[�����S���Ɗ����Փ˂��܂���");
      is_on_ground = true;
      player_obb_.OnCollisionHit(actor::ActorType::DESK);
      desk_obbs_[i].OnCollisionHit(actor::ActorType::PLAYER);
    }
  }

  //�e�G�l�~�[�Ɗe���̏Փ˔���𒲂ׂ�
  for (i32 j = 0; j < enemy_obbs_.size(); j++) {
    is_on_ground = false;
    for (i32 i = 0; i < desk_obbs_.size(); i++) {
      if (is_on_ground) continue;

      if (physics::Collision::GetInstance()->Collision_OBB_DeskOBB(
              enemy_obbs_[j], desk_obbs_[i])) {
        // MY_LOG(L"�G�l�~�[�����S���Ɗ����Փ˂��܂���");
        is_on_ground = true;
      }
    }
  }

  //�v���C���[�Ə�Q���̏Փ˔���
  for (i32 i = 0; i < obstacle_obbs_.size(); i++) {
    if (physics::Collision::GetInstance()->Collision_OBB_OBB(
            player_obb_, obstacle_obbs_[i], true, false)) {
      Deceleration(player_velocity_, 75, player_deceleration_x_,
                   player_deceleration_z_);
      player_obb_.OnCollisionHit(actor::ActorType::OBSTACLE);
      obstacle_obbs_[i].OnCollisionHit(actor::ActorType::PLAYER);
    }
  }

  //�e�G�l�~�[�Ə�Q���̏Փ˔���
  for (i32 i = 0; i < enemy_obbs_.size(); i++) {
    for (i32 j = 0; j < obstacle_obbs_.size(); j++) {
      if (physics::Collision::GetInstance()->Collision_OBB_OBB(
              enemy_obbs_[i], obstacle_obbs_[j], true, false)) {
        Deceleration(enemy_velocities_[i], 75, enemy_deceleration_x_[i],
                     enemy_deceleration_z_[i]);
        enemy_obbs_[i].OnCollisionHit(actor::ActorType::OBSTACLE);
        obstacle_obbs_[i].OnCollisionHit(actor::ActorType::ENEMY);
      }
    }
  }

  //�v���C���[�Ɗe�G�l�~�[�̏Փ˔���𒲂ׂ�
  for (i32 i = 0; i < enemy_obbs_.size(); i++) {
    if (physics::Collision::GetInstance()->Collision_OBB_OBB(
            player_obb_, enemy_obbs_[i], true, true)) {
      if (turn == Mode::PLAYER_MOVE_READY || turn == Mode::PLAYER_MOVING) {
        Deceleration(player_velocity_, 75, player_deceleration_x_,
                     player_deceleration_z_);

        math::Vector3 vel = player_velocity_ * update_time_;
        math::Vector3 pos = enemy_obbs_[i].GetPosition() + vel * player_power;
        enemy_obbs_[i].SetPosition(pos);
      } else {
        Deceleration(enemy_velocities_[i], 75, enemy_deceleration_x_[i],
                     enemy_deceleration_z_[i]);

        math::Vector3 vel = enemy_velocities_[i] * update_time_;
        math::Vector3 pos = player_obb_.GetPosition() + vel * 2.0f;
        player_obb_.SetPosition(pos);
      }
    }
  }

  //�e�G�l�~�[���m�̏Փ˔���𒲂ׂ�
  for (i32 i = 0; i < enemy_obbs_.size() - 1; i++) {
    for (i32 j = i + 1; j < enemy_obbs_.size(); j++) {
      if (physics::Collision::GetInstance()->Collision_OBB_OBB(
              enemy_obbs_[i], enemy_obbs_[j], true, true)) {
        {
          Deceleration(enemy_velocities_[i], 75, enemy_deceleration_x_[i],
                       enemy_deceleration_z_[i]);

          math::Vector3 vel = enemy_velocities_[i] * update_time_;
          math::Vector3 pos = enemy_obbs_[j].GetPosition() + vel * 2.0f;
          enemy_obbs_[j].SetPosition(pos);
        }
        {
          Deceleration(enemy_velocities_[j], 75, enemy_deceleration_x_[j],
                       enemy_deceleration_z_[j]);

          math::Vector3 vel = enemy_velocities_[j] * update_time_;
          math::Vector3 pos = enemy_obbs_[i].GetPosition() + vel * 2.0f;
          enemy_obbs_[i].SetPosition(pos);
        }
      }
    }
  }

  //�v���C���[�������J�X�ɓ����������ǂ���
  for (i32 i = 0; i < fragment_obbs_.size(); i++) {
    if (physics::Collision::GetInstance()->Collision_OBB_OBB(
            player_obb_, fragment_obbs_[i], false, false)) {
      player_obb_.OnTriggerHit(actor::ActorType::FRAGMENT);
      fragment_obbs_[i].OnTriggerHit(actor::ActorType::PLAYER);
      is_hit_fragments_[i] = true;
    }
  }

  //�v���C���[�Ɨ������̏Փ˔���
  for (i32 i = 0; i < graffiti_obbs_.size(); i++) {
    if (player_velocity_ == math::Vector3::kZeroVector) break;

    if (physics::Collision::GetInstance()->Collision_OBB_OBB(
            player_obb_, graffiti_obbs_[i], false, false)) {
      //�����J�X�𐶐����鏈���Ɨ��������������߂̏���
      is_hit_graffities_[i] = true;
      graffiti_erase_percent_[i] = 10.0f;
    } else {
      is_hit_graffities_[i] = false;
      graffiti_erase_percent_[i] = 0;
    }
  }

  UpdateGravity(gravity_);

  if (ImGui::Begin("Player")) {
    math::Vector3 position = player_obb_.GetPosition();
    math::Vector3 velocity = player_vel;
    float impulse = player_impulse;
    ImGui::SliderFloat3("Velocity", &velocity.x, -1.0f, 1.0f);
    ImGui::SliderFloat("Impulse", &impulse, 0, 1.0f);
    ImGui::SliderFloat3("Position", &position.x, -100.0f, 100.0f);

    math::Vector3 rotation =
        math::Quaternion::ToEular(player_obb_.GetRotation()) *
        math::util::RAD_2_DEG;
    ImGui::SliderFloat3("Rotation", &rotation.x, -180.0f, 180.0f);
    player_obb_.SetRotation(
        math::Quaternion::FromEular(rotation * math::util::DEG_2_RAD));
  }
  ImGui::End();

  return true;
}

//�v���C���[�����蔻��̓o�^
void PhysicsField::SetPlayer(const physics::BoundingBox& player_obb) {
  player_obb_ = player_obb;
  is_player_move_ = false;
  player_velocity_ = math::Vector3::kZeroVector;
}

//�G�l�~�[�����蔻��̓o�^
void PhysicsField::AddEnemy(const physics::BoundingBox& enemy_obb) {
  enemy_obbs_.emplace_back(enemy_obb);
  enemy_velocities_.emplace_back(math::Vector3::kZeroVector);
  is_enemy_move_.emplace_back(false);
  enemy_deceleration_x_.emplace_back(0.0f);
  enemy_deceleration_z_.emplace_back(0.0f);
}

//�������蔻��̓o�^
void PhysicsField::AddDesk(const physics::BoundingBox& desk_obb) {
  desk_obbs_.emplace_back(desk_obb);
}

//��Q���̓����蔻��̓o�^
void PhysicsField::AddObstacle(const physics::BoundingBox& obstacle_obb) {
  obstacle_obbs_.emplace_back(obstacle_obb);
}

void PhysicsField::AddFragment(const physics::BoundingBox& fragment_obb) {
  fragment_obbs_.emplace_back(fragment_obb);
  is_hit_fragments_.emplace_back(false);
}

void PhysicsField::AddGraffiti(const physics::BoundingBox& graffiti_obb) {
  graffiti_obbs_.emplace_back(graffiti_obb);
  graffiti_erase_percent_.emplace_back(0.0f);
  is_hit_graffities_.emplace_back(false);
}

//�G�l�~�[�����蔻��̍폜
void PhysicsField::RemoveEnemy(i32 index_num) {
  if (index_num < 0 || enemy_obbs_.size() <= 0 ||
      enemy_obbs_.size() <= index_num)
    return;

  enemy_obbs_.erase(enemy_obbs_.begin() + index_num);
  enemy_velocities_.erase(enemy_velocities_.begin() + index_num);
  is_enemy_move_.erase(is_enemy_move_.begin() + index_num);
  enemy_deceleration_x_.erase(enemy_deceleration_x_.begin() + index_num);
  enemy_deceleration_z_.erase(enemy_deceleration_z_.begin() + index_num);
}

//�������蔻��̍폜
void PhysicsField::RemoveDesk(i32 index_num) {
  if (index_num < 0 || desk_obbs_.size() <= 0 || desk_obbs_.size() <= index_num)
    return;

  desk_obbs_.erase(desk_obbs_.begin() + index_num);
}

void PhysicsField::RemoveGraffiti(i32 index_num) {
  if (index_num < 0 || graffiti_obbs_.size() <= 0 ||
      graffiti_obbs_.size() <= index_num)
    return;

  graffiti_obbs_.erase(graffiti_obbs_.begin() + index_num);
  graffiti_erase_percent_.erase(graffiti_erase_percent_.begin() + index_num);
  is_hit_graffities_.erase(is_hit_graffities_.begin() + index_num);
}

void PhysicsField::RemoveFragment(i32 index_num) {
  if (index_num < 0 || fragment_obbs_.size() <= 0 ||
      fragment_obbs_.size() <= index_num)
    return;

  fragment_obbs_.erase(fragment_obbs_.begin() + index_num);
  is_hit_fragments_.erase(is_hit_fragments_.begin() + index_num);
}

//�d�͗���
void PhysicsField::UpdateGravity(float gravity) {
  math::Vector3 g;

  //�v���C���[�̏d�͗�������
  if (!player_obb_.GetOnGround()) {
    g = math::Vector3(0, gravity, 0);
    math::Vector3 player_pos = player_obb_.GetPosition() + g * update_time_;
    player_obb_.SetPosition(player_pos);
  }

  //�G�l�~�[�̏d�͗�������
  for (i32 i = 0; i < enemy_obbs_.size(); i++) {
    if (!enemy_obbs_[i].GetOnGround()) {
      g = math::Vector3(0, gravity, 0);
      math::Vector3 enemy_pos = enemy_obbs_[i].GetPosition() + g * update_time_;
      enemy_obbs_[i].SetPosition(enemy_pos);
    }
  }
}

//�ړ�
void PhysicsField::PlayerMove(math::Vector3 vel, float player_impulse,
                              float player_power) {
  float speed = player_power * player_impulse;
  if (!is_player_move_) {
    player_velocity_ = vel;

    if (player_velocity_ == math::Vector3::kZeroVector) return;

    float length = math::util::Sqrt(player_velocity_.x * player_velocity_.x +
                                    player_velocity_.z * player_velocity_.z);

    //���ۂɓ�������
    float rate = speed / length;
    player_velocity_.x *= rate;
    player_velocity_.z *= rate;

    //�����v�Z
    player_deceleration_x_ = player_velocity_.x / (speed * speed);
    player_deceleration_z_ = player_velocity_.z / (speed * speed);
    is_player_move_ = true;
  }

  math::Vector3 velocity =
      player_obb_.GetPosition() + player_velocity_ * update_time_ * speed;
  player_obb_.SetPosition(velocity);

  if (player_obb_.GetOnGround())
    Deceleration(player_velocity_, 30, player_deceleration_x_,
                 player_deceleration_z_);

  if (player_velocity_ == math::Vector3::kZeroVector) is_player_move_ = false;
}

//�e�G�l�~�[�̈ړ�����
void PhysicsField::EnemyMove(std::vector<math::Vector3> enemies_vel) {
  float speed = 5.0f;
  for (i32 i = 0; i < enemies_vel.size(); i++) {
    if (!is_enemy_move_[i]) {
      enemy_velocities_[i] = enemies_vel[i];

      if (enemy_velocities_[i] == math::Vector3::kZeroVector) continue;

      float length =
          math::util::Sqrt(enemy_velocities_[i].x * enemy_velocities_[i].x +
                           enemy_velocities_[i].z * enemy_velocities_[i].z);

      //���ۂɓ�������
      float rate = speed / length;
      enemy_velocities_[i].x *= rate;
      enemy_velocities_[i].z *= rate;

      //�����v�Z
      enemy_deceleration_x_[i] = enemy_velocities_[i].x / (speed * speed);
      enemy_deceleration_z_[i] = enemy_velocities_[i].z / (speed * speed);
      is_enemy_move_[i] = true;
    }

    math::Vector3 velocity = enemy_obbs_[i].GetPosition() +
                             enemy_velocities_[i] * update_time_ * speed;
    enemy_obbs_[i].SetPosition(velocity);

    if (enemy_obbs_[i].GetOnGround())
      Deceleration(enemy_velocities_[i], 30, enemy_deceleration_x_[i],
                   enemy_deceleration_z_[i]);
  }
}

//����
void PhysicsField::Deceleration(math::Vector3& vel, float deceleration_rate,
                                float deceleration_x, float deceleration_z) {
  float x = (deceleration_x * deceleration_rate * update_time_);
  float z = (deceleration_z * deceleration_rate * update_time_);

  if ((x <= vel.x && vel.x <= 0) || (0 <= vel.x && vel.x <= x)) {
    vel.x = 0;
  } else {
    vel.x -= x;
  }
  if ((z <= vel.z && vel.z <= 0) || (0 <= vel.z && vel.z <= z)) {
    vel.z = 0;
  } else {
    vel.z -= z;
  }
}

//�v���C���[�����蔻��̎擾
physics::BoundingBox PhysicsField::GetPlayerOBB() const { return player_obb_; }

//�������蔻��̎擾
physics::BoundingBox PhysicsField::GetDeskOBB(i32 index_num) const {
  return desk_obbs_[index_num];
}

//�G�l�~�[�����蔻��̎擾
physics::BoundingBox PhysicsField::GetEnemyOBB(i32 index_num) const {
  return enemy_obbs_[index_num];
}

//�X�V�����v���C���[�̑��x�̎擾
math::Vector3 PhysicsField::GetPlayerVelocity() const {
  return player_velocity_;
}

math::Quaternion PhysicsField::GetPlayerRotation() const {
  return player_obb_.GetRotation();
}

//�X�V�����e�G�l�~�[�̑��x�̎擾
math::Vector3 PhysicsField::GetEnemyVelocity(i32 index_num) const {
  return enemy_velocities_[index_num];
}

//�G�l�~�[�̈ړ�����̃��Z�b�g
void PhysicsField::ResetEnemyMove() {
  if (enemy_obbs_.size() == 0) return;

  for (i32 i = 0; i < is_enemy_move_.size(); i++) {
    is_enemy_move_[i] = false;
  }
}

//��������
float PhysicsField::GetErasePercent(i32 index_num) const {
  return graffiti_erase_percent_[index_num];
}

//�e�������ɓ���������
bool PhysicsField::GetIsHitGraffiti(i32 index_num) const {
  return is_hit_graffities_[index_num];
}

//�e�����J�X�ɓ���������
bool PhysicsField::GetIsHitFragment(i32 index_num) const {
  return is_hit_fragments_[index_num];
}
}  // namespace system
}  // namespace legend