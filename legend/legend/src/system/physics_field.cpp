#include "src/system/physics_field.h"

#include "src/physics/collision.h"

namespace legend {
namespace system {

//�R���X�g���N�^
PhysicsField::PhysicsField() : gravity_(-2.0f) {}

//�f�X�g���N�^
PhysicsField::~PhysicsField() {}

bool PhysicsField::Init() {
  desk_obbs_.clear();
  enemy_obbs_.clear();
  is_enemy_move_.clear();
  enemy_deceleration_x_.clear();
  enemy_deceleration_z_.clear();

  return true;
}

//�X�V
bool PhysicsField::Update(Turn turn, math::Vector3 player_vel, bool player_move,
                          float player_impulse, float player_power,
                          std::vector<math::Vector3> enemies_vel,
                          bool enemy_move) {
  update_time_ =
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();

  if (turn == Turn::PLAYER_TURN) {
    if (player_move)
      PlayerMove(player_vel, player_impulse, player_power);
    else
      is_player_move_ = false;
  } else {
    if (enemy_move) EnemyMove(enemies_vel);
  }

  //�v���C���[�Ɗe���Ƃ̏Փ˔���𒲂ׂ�
  bool is_on_ground = false;
  for (i32 i = 0; i < desk_obbs_.size(); i++) {
    if (is_on_ground) break;

    if (physics::Collision::GetInstance()->Collision_OBB_DeskOBB(
            player_obb_, desk_obbs_[i])) {
      // MY_LOG(L"�v���C���[�����S���Ɗ����Փ˂��܂���");
      is_on_ground = true;
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
            player_obb_, obstacle_obbs_[i])) {
      MY_LOG(L"�v���C���[�Ə�Q�����Փ˂��܂���");
      math::Vector3 player_len = math::Matrix4x4::MultiplyCoord(
          player_obb_.GetLength(), player_obb_.GetRotation().ToMatrix());
      math::Vector3 obstacle_len = math::Matrix4x4::MultiplyCoord(
          obstacle_obbs_[i].GetLength(),
          obstacle_obbs_[i].GetRotation().ToMatrix());
      float player_left = player_obb_.GetPosition().x - player_len.x;
      float player_right = player_obb_.GetPosition().x + player_len.x;
      float player_back = player_obb_.GetPosition().z - player_len.z;
      float player_front = player_obb_.GetPosition().z + player_len.z;
      float obstacle_left = obstacle_obbs_[i].GetPosition().x - obstacle_len.x;
      float obstacle_right = obstacle_obbs_[i].GetPosition().x + obstacle_len.x;
      float obstacle_back = obstacle_obbs_[i].GetPosition().z - obstacle_len.z;
      float obstacle_front = obstacle_obbs_[i].GetPosition().z + obstacle_len.z;
      math::Vector3 adjust_pos = math::Vector3::kZeroVector;
      if (player_left < obstacle_right) {
        adjust_pos.x = (player_left - obstacle_right);
      }
      if (player_right > obstacle_left) {
        adjust_pos.x = (player_right - obstacle_left);
      }
      if (player_back < obstacle_front) {
        adjust_pos.z = (player_back - obstacle_front);
      }
      if (player_front > obstacle_back) {
        adjust_pos.z = (player_front - obstacle_back);
      }

      Deceleration(player_velocity_, 15, player_deceleration_x_,
                   player_deceleration_z_);
      adjust_pos *= update_time_;
      player_obb_.SetPosition(player_obb_.GetPosition() + adjust_pos);
    }
  }

  //�e�G�l�~�[�Ə�Q���̏Փ˔���
  for (i32 i = 0; i < enemy_obbs_.size(); i++) {
    for (i32 j = 0; j < obstacle_obbs_.size(); j++) {
      if (physics::Collision::GetInstance()->Collision_OBB_OBB(
              enemy_obbs_[i], obstacle_obbs_[j])) {
        MY_LOG(L"�G�l�~�[�Ə�Q�����Փ˂��܂���");
        math::Vector3 enemy_len = math::Matrix4x4::MultiplyCoord(
            enemy_obbs_[i].GetLength(),
            enemy_obbs_[i].GetRotation().ToMatrix());
        math::Vector3 obstacle_len = math::Matrix4x4::MultiplyCoord(
            obstacle_obbs_[j].GetLength(),
            obstacle_obbs_[j].GetRotation().ToMatrix());
        float enemy_left = enemy_obbs_[i].GetPosition().x - enemy_len.x;
        float enemy_right = enemy_obbs_[i].GetPosition().x + enemy_len.x;
        float enemy_back = enemy_obbs_[i].GetPosition().z - enemy_len.z;
        float enemy_front = enemy_obbs_[i].GetPosition().z + enemy_len.z;
        float obstacle_left =
            obstacle_obbs_[j].GetPosition().x - obstacle_len.x;
        float obstacle_right =
            obstacle_obbs_[j].GetPosition().x + obstacle_len.x;
        float obstacle_back =
            obstacle_obbs_[j].GetPosition().z - obstacle_len.z;
        float obstacle_front =
            obstacle_obbs_[j].GetPosition().z + obstacle_len.z;
        math::Vector3 adjust_pos = math::Vector3::kZeroVector;
        if (enemy_left < obstacle_right) {
          adjust_pos.x = (enemy_left - obstacle_right);
        }
        if (enemy_right > obstacle_left) {
          adjust_pos.x = (enemy_right - obstacle_left);
        }
        if (enemy_back < obstacle_front) {
          adjust_pos.z = (enemy_back - obstacle_front);
        }
        if (enemy_front > obstacle_back) {
          adjust_pos.z = (enemy_front - obstacle_back);
        }

        Deceleration(enemy_velocities_[i], 15, enemy_deceleration_x_[i],
                     enemy_deceleration_z_[i]);
        adjust_pos *= update_time_;
        enemy_obbs_[i].SetPosition(enemy_obbs_[i].GetPosition() + adjust_pos);
      }
    }
  }

  //�v���C���[�Ɗe�G�l�~�[�̏Փ˔���𒲂ׂ�
  for (i32 i = 0; i < enemy_obbs_.size(); i++) {
    if (physics::Collision::GetInstance()->Collision_OBB_OBB(player_obb_,
                                                             enemy_obbs_[i])) {
      math::Vector3 player_len = math::Matrix4x4::MultiplyCoord(
          player_obb_.GetLength(), player_obb_.GetRotation().ToMatrix());
      math::Vector3 enemy_len = math::Matrix4x4::MultiplyCoord(
          enemy_obbs_[i].GetLength(), enemy_obbs_[i].GetRotation().ToMatrix());
      float player_left = player_obb_.GetPosition().x - player_len.x;
      float player_right = player_obb_.GetPosition().x + player_len.x;
      float player_back = player_obb_.GetPosition().z - player_len.z;
      float player_front = player_obb_.GetPosition().z + player_len.z;
      float enemy_left = enemy_obbs_[i].GetPosition().x - enemy_len.x;
      float enemy_right = enemy_obbs_[i].GetPosition().x + enemy_len.x;
      float enemy_back = enemy_obbs_[i].GetPosition().z - enemy_len.z;
      float enemy_front = enemy_obbs_[i].GetPosition().z + enemy_len.z;
      math::Vector3 adjust_pos = math::Vector3::kZeroVector;
      if (player_left < enemy_right) {
        adjust_pos.x = (player_left - enemy_right) / 2.0f;
      }
      if (player_right > enemy_left) {
        adjust_pos.x = (player_right - enemy_left) / 2.0f;
      }
      if (player_back < enemy_front) {
        adjust_pos.z = (player_back - enemy_front) / 2.0f;
      }
      if (player_front > enemy_back) {
        adjust_pos.z = (player_front - enemy_back) / 2.0f;
      }
      adjust_pos *= update_time_;

      if (turn == Turn::PLAYER_TURN) {
        Deceleration(player_velocity_, 5, player_deceleration_x_,
                     player_deceleration_z_);

        if (player_velocity_ != math::Vector3::kZeroVector) {
          math::Vector3 vel = player_velocity_ * update_time_;
          math::Vector3 pos = enemy_obbs_[i].GetPosition() + vel * player_power;
          enemy_obbs_[i].SetPosition(pos);
        } else {
          enemy_obbs_[i].SetPosition(enemy_obbs_[i].GetPosition() - adjust_pos);
        }
      } else {
        Deceleration(enemy_velocities_[i], 15, enemy_deceleration_x_[i],
                     enemy_deceleration_z_[i]);

        if (enemy_velocities_[i] != math::Vector3::kZeroVector) {
          math::Vector3 vel = enemy_velocities_[i] * update_time_;
          math::Vector3 pos = player_obb_.GetPosition() + vel * 2.0f;
          player_obb_.SetPosition(pos);
        } else {
          player_obb_.SetPosition(player_obb_.GetPosition() + adjust_pos);
        }
      }
    }
  }

  //�e�G�l�~�[���m�̏Փ˔���𒲂ׂ�
  for (i32 i = 0; i < enemy_obbs_.size() - 1; i++) {
    for (i32 j = i + 1; j < enemy_obbs_.size(); j++) {

      if (physics::Collision::GetInstance()->Collision_OBB_OBB(
              enemy_obbs_[i], enemy_obbs_[j])) {
        math::Vector3 enemy1_len = math::Matrix4x4::MultiplyCoord(
            enemy_obbs_[i].GetLength(),
            enemy_obbs_[i].GetRotation().ToMatrix());
        math::Vector3 enemy2_len = math::Matrix4x4::MultiplyCoord(
            enemy_obbs_[j].GetLength(),
            enemy_obbs_[j].GetRotation().ToMatrix());
        float enemy1_left = enemy_obbs_[i].GetPosition().x - enemy1_len.x;
        float enemy1_right = enemy_obbs_[i].GetPosition().x + enemy1_len.x;
        float enemy1_back = enemy_obbs_[i].GetPosition().z - enemy1_len.z;
        float enemy1_front = enemy_obbs_[i].GetPosition().z + enemy1_len.z;
        float enemy2_left = enemy_obbs_[j].GetPosition().x - enemy2_len.x;
        float enemy2_right = enemy_obbs_[j].GetPosition().x + enemy2_len.x;
        float enemy2_back = enemy_obbs_[j].GetPosition().z - enemy2_len.z;
        float enemy2_front = enemy_obbs_[j].GetPosition().z + enemy2_len.z;
        math::Vector3 adjust_pos = math::Vector3::kZeroVector;
        if (enemy1_left < enemy2_right) {
          adjust_pos.x = (enemy1_left - enemy2_right) / 2.0f;
        }
        if (enemy1_right > enemy2_left) {
          adjust_pos.x = (enemy1_right - enemy2_left) / 2.0f;
        }
        if (enemy1_back < enemy2_front) {
          adjust_pos.z = (enemy1_back - enemy2_front) / 2.0f;
        }
        if (enemy1_front > enemy2_back) {
          adjust_pos.z = (enemy1_front - enemy2_back) / 2.0f;
        }
        adjust_pos *= update_time_;

        {
          Deceleration(enemy_velocities_[i], 15, enemy_deceleration_x_[i],
                       enemy_deceleration_z_[i]);

          if (enemy_velocities_[i] != math::Vector3::kZeroVector) {
            math::Vector3 vel = enemy_velocities_[i] * update_time_;
            math::Vector3 pos = enemy_obbs_[j].GetPosition() + vel * 2.0f;
            enemy_obbs_[j].SetPosition(pos);
          } else {
            enemy_obbs_[j].SetPosition(enemy_obbs_[j].GetPosition() -
                                       adjust_pos);
          }
        }
        {
          Deceleration(enemy_velocities_[j], 15, enemy_deceleration_x_[j],
                       enemy_deceleration_z_[j]);

          if (enemy_velocities_[j] != math::Vector3::kZeroVector) {
            math::Vector3 vel = enemy_velocities_[j] * update_time_;
            math::Vector3 pos = enemy_obbs_[i].GetPosition() + vel * 2.0f;
            enemy_obbs_[i].SetPosition(pos);
          } else {
            enemy_obbs_[i].SetPosition(enemy_obbs_[i].GetPosition() +
                                       adjust_pos);
          }
        }
      }
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

//�G�l�~�[�����蔻��̍폜
void PhysicsField::RemoveEnemy(i32 index_num) {
  if (index_num < 0 || enemy_obbs_.size() <= 0 ||
      enemy_obbs_.size() < index_num)
    return;

  enemy_obbs_.erase(enemy_obbs_.begin() + index_num);
  enemy_velocities_.erase(enemy_velocities_.begin() + index_num);
  is_enemy_move_.erase(is_enemy_move_.begin() + index_num);
  enemy_deceleration_x_.erase(enemy_deceleration_x_.begin() + index_num);
  enemy_deceleration_z_.erase(enemy_deceleration_z_.begin() + index_num);
}

//�������蔻��̍폜
void PhysicsField::RemoveDesk(i32 index_num) {
  if (index_num < 0 || desk_obbs_.size() <= 0 || desk_obbs_.size() < index_num)
    return;

  desk_obbs_.erase(desk_obbs_.begin() + index_num);
}

//�d�͗���
void PhysicsField::UpdateGravity(float gravity) {
  math::Vector3 g;

  //�v���C���[�̏d�͗�������
  if (!player_obb_.GetOnGround()) {
    g = player_velocity_ + math::Vector3(0, gravity, 0);
    math::Vector3 player_pos = player_obb_.GetPosition() + g * update_time_;
    player_obb_.SetPosition(player_pos);
  }

  //�G�l�~�[�̏d�͗�������
  for (i32 i = 0; i < enemy_obbs_.size(); i++) {
    if (!enemy_obbs_[i].GetOnGround()) {
      g = enemy_velocities_[i] + math::Vector3(0, gravity, 0);
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

    if (!is_player_move_) {
      //�����v�Z
      player_deceleration_x_ = player_velocity_.x / (speed * speed);
      player_deceleration_z_ = player_velocity_.z / (speed * speed);
      is_player_move_ = true;
    }
  }

  math::Vector3 velocity =
      player_obb_.GetPosition() + player_velocity_ * update_time_ * speed;
  player_obb_.SetPosition(velocity);

  if (player_obb_.GetOnGround())
    Deceleration(player_velocity_, 2, player_deceleration_x_,
                 player_deceleration_z_);
}

//�e�G�l�~�[�̈ړ�����
void PhysicsField::EnemyMove(std::vector<math::Vector3> enemies_vel) {
  float speed = 1.0f;
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

      if (!is_enemy_move_[i]) {
        //�����v�Z
        enemy_deceleration_x_[i] = enemy_velocities_[i].x / (speed * speed);
        enemy_deceleration_z_[i] = enemy_velocities_[i].z / (speed * speed);
        is_enemy_move_[i] = true;
      }
    }

    math::Vector3 velocity = enemy_obbs_[i].GetPosition() +
                             enemy_velocities_[i] * update_time_ * speed;
    enemy_obbs_[i].SetPosition(velocity);

    if (enemy_obbs_[i].GetOnGround())
      Deceleration(enemy_velocities_[i], 2, enemy_deceleration_x_[i],
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
  for (i32 i = 0; i < is_enemy_move_.size(); i++) {
    is_enemy_move_[i] = false;
  }
}
}  // namespace system
}  // namespace legend