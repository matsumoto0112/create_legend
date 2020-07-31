#ifndef LEGEND_PLAYER_PLAYER_H_
#define LEGEND_PLAYER_PLAYER_H_

#include "src/physics/collision.h"

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
  Player(math::Vector3 position, math::Vector3 rotation, math::Vector3 scale,
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
  void SetPower();
  /**
   * @brief 重力の反映
   */
  void UseGravity();
  /**
   * @brief 移動に必要なパラメータの初期化
   */
  void ResetParameter();
  /**
   * @brief 減速
   */
  void Deceleration();
  /**
   * @brief 座標の取得
   */
  math::Vector3 GetPosition() const;
  /**
   * @brief 移動量の取得
   */
  math::Vector3 GetVelocity() const;
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

  //! 中心座標
  math::Vector3 position_;
  //! 回転
  math::Vector3 rotation_;
  //! スケール
  math::Vector3 scale_;

  //! 速度
  math::Vector3 velocity_;
  //! 前フレームの速度
  math::Vector3 before_velocity_;
  std::vector<math::Vector3> stick_velocities_;
  //! リストの最大値
  const i32 max_stick_velocity_num_ = 10;
  //! 減速率
  float deceleration_x_;
  float deceleration_z_;
  //! 移動中か
  bool is_move_;

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
  bool up_power_ = true;
  //! パワー設定終了か
  bool is_set_power_ = false;

  //! 更新時間
  const float update_time_ = 1 / 60.0f;

  //! 重力
  const float gravity_ = -9.8f;
};

}  // namespace player
}  // namespace legend

#endif  //! LEGEND_PLAYER_PLAYER_H_
