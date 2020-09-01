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
  virtual bool Update();
  /**
   * @brief 移動量の設定
   */
  void SetVelocity(math::Vector3 velocity);

  virtual void OnHit(bullet::Collider* other);

 private:
  std::unique_ptr < std::function<bool()>> special_action_ = {};
  float rotate_speed_ = 10.0f;
  bool is_rotate_ = false;
};

}  // namespace enemy
}  // namespace legend

#endif  //! LEGEND_ENEMY_BOSS_H_