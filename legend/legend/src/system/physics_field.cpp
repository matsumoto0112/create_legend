#include "src/system/physics_field.h"

#include "src/physics/collision.h"

namespace legend {
namespace system {

//コンストラクタ
PhysicsField::PhysicsField() : gravity_(-9.8f) {}

//デストラクタ
PhysicsField::~PhysicsField() {}

bool PhysicsField::Init() {
  desk_obbs_.clear();
  enemy_obbs_.clear();

  return true;
}

//更新
bool PhysicsField::Update(Turn turn, math::Vector3 player_vel, bool player_move,
                          float player_impulse, float player_power,
                          std::vector<math::Vector3> enemies_vel,
                          bool enemy_move, float enemy_power) {
  update_time_ =
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();

  if (turn == Turn::PLAYER_TURN) {
    if (player_move)
      PlayerMove(player_vel, player_impulse, player_power);
    else
      is_player_move_ = false;
  } else {
    if (enemy_move)
      EnemyMove(enemies_vel, enemy_power);
    else
      is_last_enemy_move_ = false;
  }

  //プレイヤーと各机との衝突判定を調べる
  for (i32 i = 0; i < desk_obbs_.size(); i++) {
    if (physics::Collision::GetInstance()->Collision_OBB_DeskOBB(
            player_obb_, desk_obbs_[i])) {
      MY_LOG(L"プレイヤー消しゴムと机が衝突しました");
      break;
    }
  }

  //各エネミーと各机の衝突判定を調べる
  for (i32 i = 0; i < desk_obbs_.size(); i++) {
    for (i32 j = 0; j < enemy_obbs_.size(); j++) {
      if (physics::Collision::GetInstance()->Collision_OBB_DeskOBB(
              enemy_obbs_[j], desk_obbs_[i])) {
        MY_LOG(L"エネミー消しゴムと机が衝突しました");
      }
    }
  }

  //プレイヤーと各エネミーの衝突判定を調べる
  for (i32 i = 0; i < enemy_obbs_.size(); i++) {
    if (physics::Collision::GetInstance()->Collision_OBB_OBB(player_obb_,
                                                             enemy_obbs_[i])) {
      MY_LOG(L"プレイヤー消しゴムとエネミー消しゴムが衝突しました");
    }
  }

  if (enemy_obbs_.size() >= 2) {
    //各敵同士の衝突判定を調べる
    for (i32 i = 0; i < enemy_obbs_.size() - 1; i++) {
      if (physics::Collision::GetInstance()->Collision_OBB_OBB(
              enemy_obbs_[i], enemy_obbs_[i + 1])) {
        MY_LOG(L"敵消しゴム同士で衝突しました");
      }
    }
  }

  UpdateGravity(gravity_);
  return true;
}

//プレイヤーあたり判定の登録
void PhysicsField::SetPlayer(physics::BoundingBox& player_obb) {
  player_obb_ = player_obb;
  is_player_move_ = false;
  player_velocity_ = math::Vector3::kZeroVector;
}

//エネミーあたり判定の登録
void PhysicsField::AddEnemy(physics::BoundingBox& enemy_obb) {
  enemy_obbs_.push_back(enemy_obb);
  enemy_velocities_.push_back(math::Vector3::kZeroVector);
}

//机あたり判定の登録
void PhysicsField::AddDesk(physics::BoundingBox& desk_obb) {
  desk_obbs_.push_back(desk_obb);
}

//エネミーあたり判定の削除
void PhysicsField::RemoveEnemy(i32 index_num) {
  if (index_num < 0 || enemy_obbs_.size() <= 0 ||
      enemy_obbs_.size() < index_num)
    return;

  enemy_obbs_.erase(enemy_obbs_.begin() + index_num);
  enemy_velocities_.erase(enemy_velocities_.begin() + index_num);
}

//机あたり判定の削除
void PhysicsField::RemoveDesk(i32 index_num) {
  if (index_num < 0 || desk_obbs_.size() <= 0 || desk_obbs_.size() < index_num)
    return;

  desk_obbs_.erase(desk_obbs_.begin() + index_num);
}

//重力落下
void PhysicsField::UpdateGravity(float gravity) {
  math::Vector3 g;

  //プレイヤーの重力落下処理
  if (!player_obb_.GetOnGround()) {
    if (player_obb_.GetPosition().y <= -10) return;

    g = player_velocity_ + math::Vector3(0, gravity, 0);
    math::Vector3 player_pos = player_obb_.GetPosition() + g * update_time_;
    player_obb_.SetPosition(player_pos);
  }

  //エネミーの重力落下処理
  for (i32 i = 0; i < enemy_obbs_.size(); i++) {
    if (!enemy_obbs_[i].GetOnGround()) {
      if (enemy_obbs_[i].GetPosition().y <= -10) continue;

      g = enemy_velocities_[i] + math::Vector3(0, gravity, 0);
      math::Vector3 enemy_pos = enemy_obbs_[i].GetPosition() + g * update_time_;
      enemy_obbs_[i].SetPosition(enemy_pos);
    }
  }
}

//移動
void PhysicsField::PlayerMove(math::Vector3 vel, float player_impulse,
                              float player_power) {
  if (!is_player_move_) {
    player_velocity_ = vel;
    is_player_move_ = true;
  }

  float length = math::util::Sqrt(player_velocity_.x * player_velocity_.x +
                                  player_velocity_.z * player_velocity_.z);

  //実際に動く距離
  float x = player_velocity_.x / length;
  float z = player_velocity_.z / length;

  math::Vector3 v = math::Vector3(x, 0, z);
  math::Vector3 velocity = player_obb_.GetPosition() +
                           v * update_time_ * player_impulse * player_power;
  player_obb_.SetPosition(velocity);

  //減速計算
  float deceleration_x_ = x / (length * length);
  float deceleration_z_ = z / (length * length);

  if (player_obb_.GetOnGround())
    Deceleration(player_velocity_, 2, deceleration_x_, deceleration_z_);
}

void PhysicsField::EnemyMove(std::vector<math::Vector3> enemies_vel,
                             float enemy_power) {
  for (i32 i = 0; i < enemies_vel.size(); i++) {
    if (!is_last_enemy_move_) {
      enemy_velocities_[i] = enemies_vel[i];
      if (i == enemy_obbs_.size()) is_last_enemy_move_ = true;
    }

    float length =
        math::util::Sqrt(enemy_velocities_[i].x * enemy_velocities_[i].x +
                         enemy_velocities_[i].z * enemy_velocities_[i].z);

    //実際に動く距離
    float x = enemy_velocities_[i].x / length;
    float z = enemy_velocities_[i].z / length;

    math::Vector3 v = math::Vector3(x, 0, z);
    math::Vector3 velocity =
        enemy_obbs_[i].GetPosition() + v * update_time_ * enemy_power;
    enemy_obbs_[i].SetPosition(velocity);

    //減速計算
    float deceleration_x_ = x / (length * length);
    float deceleration_z_ = z / (length * length);

    if (enemy_obbs_[i].GetOnGround())
      Deceleration(enemy_velocities_[i], 2, deceleration_x_, deceleration_z_);
  }
}

//減衰
void PhysicsField::Deceleration(math::Vector3& vel, float deceleration_rate,
                                float deceleration_x, float deceleration_z) {
  float x = deceleration_x * deceleration_rate * update_time_;
  float z = deceleration_z * deceleration_rate * update_time_;

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

//プレイヤーあたり判定の取得
physics::BoundingBox PhysicsField::GetPlayerOBB() const { return player_obb_; }

//机あたり判定の取得
physics::BoundingBox PhysicsField::GetDeskOBB(i32 index_num) const {
  return desk_obbs_[index_num];
}

//エネミーあたり判定の取得
physics::BoundingBox PhysicsField::GetEnemyOBB(i32 index_num) const {
  return enemy_obbs_[index_num];
}

math::Vector3 PhysicsField::GetPlayerVelocity() const {
  return player_velocity_;
}

bool PhysicsField::GetPlayerMove() const { return is_player_move_; }

math::Vector3 PhysicsField::GetEnemyVelocity(i32 index_num) const {
  return enemy_velocities_[index_num];
}
i32 PhysicsField::GetEnemyCount() const {
  return static_cast<i32>(enemy_obbs_.size());
}
}  // namespace system
}  // namespace legend