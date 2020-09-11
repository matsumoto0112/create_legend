#ifndef LEGEND_ENEMY_BOSS_H_
#define LEGEND_ENEMY_BOSS_H_

//#include "src/actor/actor.h"
#include "src/bullet/bounding_box.h"
#include "src/enemy/enemy_actor.h"
#include "src/enemy/enemy_type.h"
#include "src/util/transform.h"

namespace legend {
namespace enemy {

/**
 * @class Boss
 * @brief ボスのクラス
 */
class Boss : public enemy::EnemyActor {
  using Parent = actor::Actor;

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
   * @brief 更新
   */
  virtual bool Update();
  /**
   * @brief 移動量の設定
   */
  void SetVelocity(math::Vector3 velocity);
  /**
   * @brief 衝突判定
   * @param 衝突相手
   */
  virtual void OnHit(bullet::Collider* other);
  /**
   * @brief エネミータイプ設定
   * @param type_index 敵タイプ番号
   */
  virtual void SetType(i32 type_index = 0);

  /**
   * @brief チュートリアルの行動
   */
  void Boss_Tutorial();
  /**
   * @brief その場で回転する行動
   */
  void Boss_Rotate_Stand();
  /**
   * @brief 直進行動
   */
  void Boss_Rush_Move();

 private:
  //! 回転速度
  float rotate_speed_ = 20.0f;
  //! 回転時間
  const float rotate_time_ = 1.0f;
  //! 稼働開店時間
  float rotate_timer_ = 0.0f;

  //! チュートリアル判定
  bool is_tutorial_ = false;
  //! その場での回転判定
  bool is_rotate_ = false;
  //! 直進行動判定
  bool is_rush_ = false;

  //! 回転SE判定
  bool is_play_spin_se_;
  //! 衝突SE判定
  bool is_play_strike_se_;
};

}  // namespace enemy
}  // namespace legend

#endif  //! LEGEND_ENEMY_BOSS_H_