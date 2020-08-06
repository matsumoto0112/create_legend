#ifndef LEGEND_SYSTEM_PHYSICS_FIELD_H_
#define LEGEND_SYSTEM_PHYSICS_FIELD_H_

#include "src/physics/bounding_box.h"
#include "src/system/turn_system.h"

namespace legend {
namespace system {

/**
 * @class PhysicsField
 * @brief フィールドの管理クラス
 */
class PhysicsField {
 public:
  /**
   * @brief コンストラクタ
   */
  PhysicsField();
  /**
   * @brief デストラクタ
   */
  ~PhysicsField();
  bool Init();
  /**
   * @brief 更新
   */
  bool Update(math::Vector3 player_vel, bool player_move, float player_impulse,
              float player_power);
  /**
   * @brief プレイヤーあたり判定の登録
   * @param プレイヤーの直方体
   */
  void SetPlayer(physics::BoundingBox player_obb);
  /**
   * @brief エネミーあたり判定の登録
   * @param エネミーの直方体
   */
  void AddEnemy(physics::BoundingBox enemy_obb);
  /**
   * @brief 机のあたり判定の登録
   * @param 机の直方体
   */
  void AddDesk(physics::BoundingBox desk_obb);
  /**
   * @brief エネミーあたり判定の削除
   * @param 格納番号
   */
  void RemoveEnemy(i32 index_num);
  /**
   * @brief 机のあたり判定の削除
   * @param 格納番号
   */
  void RemoveDesk(i32 index_num);
  /**
   * @brief 移動処理
   * @param 速度
   */
  void PlayerMove(math::Vector3 vel, float player_impulse, float player_power);
  /**
   * @brief 減衰処理
   * @param 減衰率
   */
  void Deceleration(math::Vector3& vel, float deceleration_rate, float deceleration_x,
                    float deceleration_z);
  /**
   * @brief 重力落下
   * @param 重力
   */
  void UpdateGravity(float gravity);
  /**
   * @brief プレイヤーあたり判定の取得
   */
  physics::BoundingBox GetPlayerOBB() const;
  /**
   * @brief 机あたり判定の取得
   */
  physics::BoundingBox GetDeskOBB(i32 index_num) const;
  /**
   * @brief エネミーあたり判定の取得
   */
  physics::BoundingBox GetEnemyOBB(i32 index_num) const;
  math::Vector3 GetPlayerVelocity() const;
  bool GetPlayerMove() const;

 private:
  physics::BoundingBox player_obb_;
  std::vector<physics::BoundingBox> desk_obbs_;
  std::vector<physics::BoundingBox> enemy_obbs_;

  //! 重力
  float gravity_;
  //! 更新時間(deltaTime)
  float update_time_;

  //! プレイヤーが移動中か
  bool is_player_move_;
  math::Vector3 player_velocity_;
};

}  // namespace system
}  // namespace legend

#endif  //! LEGEND_SYSTEM_PHYSICS_FIELD_H_
