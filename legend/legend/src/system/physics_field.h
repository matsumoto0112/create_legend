#ifndef LEGEND_SYSTEM_PHYSICS_FIELD_H_
#define LEGEND_SYSTEM_PHYSICS_FIELD_H_

#include "src/physics/bounding_box.h"
#include "src/system/mode.h"

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
  bool Update(Mode turn, math::Vector3 player_vel, float player_impulse,
              float player_power, std::vector<math::Vector3> enemies_vel);
  /**
   * @brief プレイヤーあたり判定の登録
   * @param プレイヤーの直方体
   */
  void SetPlayer(const physics::BoundingBox& player_obb);
  /**
   * @brief エネミーあたり判定の登録
   * @param エネミーの直方体
   */
  void AddEnemy(const physics::BoundingBox& enemy_obb);
  /**
   * @brief 机のあたり判定の登録
   * @param 机の直方体
   */
  void AddDesk(const physics::BoundingBox& desk_obb);
  /**
   * @brief 障害物のあたり判定の登録
   * @param 障害物の直方体
   */
  void AddObstacle(const physics::BoundingBox& obstacle_obb);
  /**
   * @brief 消しカスのあたり判定の登録
   */
  void AddFragment(const physics::BoundingBox& fragment_obb);
  /**
   * @brief 落書きのあたり判定の登録
   */
  void AddGraffiti(const physics::BoundingBox& graffiti_obb);
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
   * @brief 落書きのあたり判定の削除
   * @param 格納番号
   */
  void RemoveGraffiti(i32 index_num);
  /**
   * @brief消しカスのあたり判定の削除
   * @param 格納番号
   */
  void RemoveFragment(i32 index_num);
  /**
   * @brief プレイヤーの移動処理
   * @param 速度
   */
  void PlayerMove(math::Vector3 vel, float player_impulse, float player_power);
  /**
   * @brief エネミーの移動処理
   * @param 速度
   */
  void EnemyMove(std::vector<math::Vector3> enemies_vel);
  /**
   * @brief 減衰処理
   * @param 減衰率
   */
  void Deceleration(math::Vector3& vel, float deceleration_rate,
                    float deceleration_x, float deceleration_z);
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
  /**
   * @brief 更新したプレイヤーの速度の取得
   */
  math::Vector3 GetPlayerVelocity() const;
  /**
   * @brief 更新したプレイヤーの回転の取得
   */
  math::Quaternion GetPlayerRotation() const;
  /**
   * @brief 更新した各エネミーの速度の取得
   */
  math::Vector3 GetEnemyVelocity(i32 index_num) const;
  /**
   * @brief エネミーの移動判定のリセット
   */
  void ResetEnemyMove();
  /**
   * @brief 落書きの消える割合を取得
   */
  float GetErasePercent(i32 index_num) const;
  /**
   * @brief 落書きにヒットしたかを取得
   */
  bool GetIsHitGraffiti(i32 index_num) const;
  /**
   * @brief 消しカスにヒットしたかを取得
   */
  bool GetIsHitFragment(i32 index_num) const;

 private:
  //! プレイヤーの直方体
  physics::BoundingBox player_obb_;
  //! エネミーの直方体
  std::vector<physics::BoundingBox> desk_obbs_;
  //! 机の直方体
  std::vector<physics::BoundingBox> enemy_obbs_;
  //! 障害物の直方体
  std::vector<physics::BoundingBox> obstacle_obbs_;
  //! 消しカスの直方体
  std::vector<physics::BoundingBox> fragment_obbs_;
  //! 落書きの直方体
  std::vector<physics::BoundingBox> graffiti_obbs_;

  //! 重力
  float gravity_;
  //! 更新時間(deltaTime)
  float update_time_;

  float player_deceleration_x_;
  float player_deceleration_z_;
  std::vector<float> enemy_deceleration_x_;
  std::vector<float> enemy_deceleration_z_;

  //! プレイヤーが移動中か
  bool is_player_move_;
  //! プレイヤーの速度
  math::Vector3 player_velocity_;

  //! 各エネミーの速度
  std::vector<math::Vector3> enemy_velocities_;
  //! 各エネミーの移動開始判定
  std::vector<bool> is_enemy_move_;

  //! 各落書きを消す割合
  std::vector<float> graffiti_erase_percent_;
  //! 各落書きに当たっているか
  std::vector<bool> is_hit_graffities_;
  //! 消しカスに当たっているか
  std::vector<bool> is_hit_fragments_;
};

}  // namespace system
}  // namespace legend

#endif  //! LEGEND_SYSTEM_PHYSICS_FIELD_H_
