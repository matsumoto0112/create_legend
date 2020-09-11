#ifndef LEGEND_SKILL_EXPLOSION_PENCIL_H_
#define LEGEND_SKILL_EXPLOSION_PENCIL_H_

#include "src/actor/actor.h"
#include "src/bullet/bounding_sphere.h"
#include "src/draw/particle/particle_emitter.h"
#include "src/util/timer.h"

/**
 * @file explosion_pencil.h
 * @brief ���M�ɂ�锚���N���X
 */

namespace legend {
namespace skill {
/**
 * @brief ����
 */
class ExplosionPencil : public actor::Actor {
  using Parent = actor::Actor;

 public:
  /**
   * @brief �R���X�g���N�^
   */
  ExplosionPencil();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~ExplosionPencil();
  /**
   * @brief ������
   */
  void Init(util::Transform transform, actor::IActorMediator* mediator);
  /**
   * @brief �X�V
   */
  bool Update();
  /**
   * @brief �Փ˔���
   */
  void OnHit(bullet::Collider* other);
  /**
   * @brief �폜
   */
  void Destroy(actor::IActorMediator* mediator);

 private:
  //! �R���C�_�[
  std::shared_ptr<bullet::BoundingSphere> sphere_;
  //! ���a
  float radius_;
  //! �����З�
  float explosion_power_;
  std::shared_ptr<draw::particle::ParticleEmitter> explosion_particle_;
};
}  // namespace skill
}  // namespace legend

#endif  //! LEGEND_SKILL_EXPLOSION_PENCIL_H_
