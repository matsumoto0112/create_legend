#ifndef LEGEND_ENEMY_ENEMY_H_
#define LEGEND_ENEMY_ENEMY_H_

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/shader/graphics_pipeline_state.h"
#include "src/draw/model.h"
#include "src/physics/collision.h"
#include "src/util/transform.h"

namespace legend {
namespace enemy {

/**
 * @class Enemy
 * @brief エネミーのクラス
 */
class Enemy : public actor::Actor<physics::BoundingBox> {
 public:
  /**
   * @brief 初期化パラメータ
   */
  struct InitializeParameter {
    util::Transform transform;
    math::Vector3 bouding_box_length;
  };

 public:
  /**
   * @brief コンストラクタ
   */
  Enemy();
  /**
   * @brief デストラクタ
   */
  ~Enemy();
  /**
   * @brief 初期化
   */
  virtual bool Init(const InitializeParameter& parameter);
  /**
   * @brief 更新
   */
  bool Update();
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
   * @brief 回転量の設定
   */
  void SetRotation();
  /**
   * @brief 移動に必要なパラメータの初期化
   */
  void ResetParameter();
  ///**
  // * @brief 減速
  // * @param 減速率(1より大きい値で)
  // */
  // void Deceleration(float deceleration_rate);
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
   * @brief 移動に加える力の取得
   */
  float GetPower() const;
  /**
   * @brief 移動終了判定の取得
   */
  bool GetMoveEnd() const;
  /**
   * @brief 移動終了判定のリセット
   */
  void ResetMoveEnd();

 private:
  //! 速度
  math::Vector3 velocity_;
  ////! 減速率
  // float deceleration_x_;
  // float deceleration_z_;
  //! 移動中か
  bool is_move_;

  //! 移動に加える力
  const float power_ = 1;
  ////! 実際に加える力の加減
  // float impulse_;

  //! 更新時間
  float update_time_;

  //! 移動終了判定
  bool move_end_;
};

}  // namespace enemy
}  // namespace legend

#endif  //! LEGEND_ENEMY_ENEMY_H_