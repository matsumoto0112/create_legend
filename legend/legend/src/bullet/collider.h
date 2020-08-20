#ifndef LEGEND_BULLET_COLLIDER_H_
#define LEGEND_BULLET_COLLIDER_H_

/**
 * @file collider.h
 * @brief
 */

#include <btBulletDynamicsCommon.h>

#include "src/math/quaternion.h"
#include "src/util/transform.h"

namespace legend {
namespace actor {
class Actor;
}  // namespace actor

namespace bullet {
class Collider : public btActionInterface {
  using CollisionCallback = std::function<void(Collider* other)>;

 public:
  /**
   * @brief �R���X�g���N�^
   */
  Collider(actor::Actor* owner);
  /**
   * @brief �f�X�g���N�^
   */
  ~Collider();
  /**
   * @brief �X�V����
   */
  virtual bool Update();

  /**
   * @brief ���̃I�u�W�F�N�g���擾
   */
  btRigidBody* GetRigidBody();
  /**
   * @brief �I�u�W�F�N�g�̒��S����w��̗͂�������
   */
  void ApplyCentralImpulse(const math::Vector3& impulse);
  /**
   * @brief �ړ��ʂ̎擾
   */
  btVector3 GetVelocity();
  /**
   * @brief �ړ��ʂ̎w��
   */
  void SetVelocity(const math::Vector3& velocity);
  /**
   * @brief ��]�ړ��ʂ̎擾
   */
  btVector3 GetAngularVelocity();
  /**
   * @brief ��]�ړ��ʂ̎w��
   */
  void SetAngularVelocity(const math::Vector3& velocity);
  btMotionState* GetMotionState() const { return motion_state_.get(); }
  virtual void updateAction(btCollisionWorld* collisionWorld,
                            btScalar deltaTimeStep) override;
  virtual void debugDraw(btIDebugDraw* debugDrawer) override;
  virtual void UpdateOwnerTransform() const;

  virtual void OnTriggerHit(Collider* other);
  virtual void OnCollisionHit();
  void SetCollisionCallBack(CollisionCallback callback) {
    this->callback_ = callback;
  }
  actor::Actor* GetOwner() const { return owner_; }

 protected:
  actor::Actor* owner_;
  std::shared_ptr<btCollisionShape> shape_;
  std::shared_ptr<btDefaultMotionState> motion_state_;
  btVector3 inertia_;
  std::shared_ptr<btRigidBody> rigid_body_;
  CollisionCallback callback_;
};
}  // namespace bullet
}  // namespace legend

#endif  //! LEGEND_BULLET_COLLIDER_H_
