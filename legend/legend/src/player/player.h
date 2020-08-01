#ifndef LEGEND_PLAYER_PLAYER_H_
#define LEGEND_PLAYER_PLAYER_H_

#include "src/physics/collision.h"
#include "src/util/transform.h"

namespace legend {
namespace player {

/**
 * @class Player
 * @brief プレイヤーのクラス
 */
class Player {
 public:
  /**
   * @brief コンストラクタ
   */
  Player();
  /**
   * @brief コンストラクタ
   * @param 座標
   * @param 回転
   * @param スケール
   * @param 力の最低値
   * @param 力の最大値
   */
  Player(math::Vector3 position, math::Quaternion rotation, math::Vector3 scale,
         float min_power, float max_power);
  /**
   * @brief デストラクタ
   */
  ~Player();
  /**
   * @brief 初期化
   */
  bool Initilaize(directx::DirectX12Device& device);
  /**
   * @brief 更新
   */
  bool Update();
  /**
   * @brief 描画
   */
  void Draw(directx::DirectX12Device& device);
  /**
   * @brief 移動
   */
  void Move();
  /**
   * @brief 座標の設定
   */
  void SetPosition(math::Vector3 position);
  /**
   * @brief 移動量の設定
   */
  void SetVelocity(math::Vector3 velocity);
  /**
   * @brief 移動量の設定
   */
  void SetVelocity();
  /**
   * @brief 加える力の設定
   */
  void SetImpulse();
  /**
   * @brief 移動に必要なパラメータの初期化
   */
  void ResetParameter();
  /**
   * @brief 減速
   * @param 減速率(1より大きい値で)
   */
  void Deceleration(float deceleration_rate);
  /**
   * @brief 座標の取得
   */
  math::Vector3 GetPosition() const;
  /**
   * @brief 移動量の取得
   */
  math::Vector3 GetVelocity() const;
  /**
   * @brief 回転の取得
   */
  math::Quaternion GetRotation() const;
  /**
   * @brief 加える力の取得
   */
  float GetImpulse() const;
  /**
   * @brief 直方体の取得
   */
  physics::BoundingBox& GetOBB();

 private:
  //衝突判定用の直方体
  physics::BoundingBox obb_;

  util::Transform transform_;

  //! 速度
  math::Vector3 velocity_;
  math::Vector3 input_velocity_;
  //! 前フレームの速度
  math::Vector3 change_amount_velocity_;
  std::vector<math::Vector3> stick_velocities_;
  //! リストの最大値
  const i32 max_stick_velocity_num_ = 10;
  //! 減速率
  float deceleration_x_;
  float deceleration_z_;
  //! 移動中か
  bool is_move_;

  //速度の更新時間
  float velocity_update_time_;
  const float change_time_ = 0.1f;

  //! 移動に加える力
  const float power_ = 10;
  //! 実際に加える力の加減
  float impulse_;
  //! 最小値
  float min_power_;
  //! 最大値
  float max_power_;
  //! 入力状態か
  bool is_input_;

  //! ゲージが上昇かどうか
  bool up_power_;
  //! パワー設定終了か
  bool is_set_power_ = false;

  //! 更新時間
  float update_time_;
};

}  // namespace player
}  // namespace legend

#endif  //! LEGEND_PLAYER_PLAYER_H_
