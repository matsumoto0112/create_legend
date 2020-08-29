#ifndef LEGEND_ENEMY_BOSS_H_
#define LEGEND_ENEMY_BOSS_H_

#include "src/actor/actor.h"
#include "src/bullet/bounding_box.h"
#include "src/util/transform.h"
#include "src/enemy/enemy_type.h"

namespace legend {
namespace enemy {

/**
 * @class Boss
 * @brief ボスのクラス
 */
class Boss : public actor::Actor {
  using Parent = actor::Actor;

 public:
  /**
   * @brief 初期化パラメータ
   */
  struct InitializeParameter {
    util::Transform transform;
    math::Vector3 bouding_box_length;
    float mass = 4.0f;
    float restitution = 0.0f;
    float friction = 1.5f;
  };

 public:
  /**
   * @brief コンストラクタ
   */
  Boss();
  /**
   * @brief デストラクタ
   */
  ~Boss();
  /**
   * @brief 初期化
   */
  virtual bool Init(actor::IActorMediator* mediator,
                    const InitializeParameter& parameter);
  /**
   * @brief 削除
   */
  virtual void Remove();
  /**
   * @brief 更新
   */
  bool Update();
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

  enemy::enemy_type::MoveType GetMoveType() { return move_type_; };
  bullet::BoundingBox* GetCollider() const { return box_.get(); }

  void OnHit(bullet::Collider* other);
  void HitAction(bullet::Collider* other);

 private:
  //! 移動中か
  bool is_move_;

  //! 移動に加える力
  const float power_ = 1;

  //! 更新時間
  float update_time_;

  //! 移動終了判定
  bool move_end_;

  //! 移動タイプ
  enemy::enemy_type::MoveType move_type_;
  //! 衝突タイプ
  enemy::enemy_type::HitType hit_type_;

  std::shared_ptr<bullet::BoundingBox> box_;
};

}  // namespace enemy
}  // namespace legend

#endif  //! LEGEND_ENEMY_BOSS_H_