#ifndef LEGEND_SKILL_EXPLOSION_PENCIL_H_
#define LEGEND_SKILL_EXPLOSION_PENCIL_H_

#include "src/actor/actor.h"
#include "src/bullet/bounding_sphere.h"
#include "src/draw/particle/particle_emitter.h"
#include "src/util/timer.h"

/**
 * @file explosion_pencil.h
 * @brief 鉛筆による爆風クラス
 */

namespace legend {
namespace skill {
/**
 * @brief 爆風
 */
class ExplosionPencil : public actor::Actor {
  using Parent = actor::Actor;

 public:
  /**
   * @brief コンストラクタ
   */
  ExplosionPencil();
  /**
   * @brief デストラクタ
   */
  virtual ~ExplosionPencil();
  /**
   * @brief 初期化
   */
  void Init(util::Transform transform, actor::IActorMediator* mediator);
  /**
   * @brief 更新
   */
  bool Update();
  /**
   * @brief 衝突判定
   */
  void OnHit(bullet::Collider* other);
  /**
   * @brief 削除
   */
  void Destroy(actor::IActorMediator* mediator);

 private:
  //! コライダー
  std::shared_ptr<bullet::BoundingSphere> sphere_;
  //! 半径
  float radius_;
  //! 爆発威力
  float explosion_power_;
  std::shared_ptr<draw::particle::ParticleEmitter> explosion_particle_;
};
}  // namespace skill
}  // namespace legend

#endif  //! LEGEND_SKILL_EXPLOSION_PENCIL_H_
