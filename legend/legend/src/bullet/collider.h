#ifndef LEGEND_BULLET_COLLIDER_H_
#define LEGEND_BULLET_COLLIDER_H_
#include <btBulletDynamicsCommon.h>

#include "src/math/quaternion.h"
#include "src/util/transform.h"
/**
 * @file collider.h
 * @brief
 */

namespace legend {
namespace bullet {
class Collider : public btActionInterface {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  Collider(util::Transform* owner_transform);
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
  void ApplyCentralImpulse(btVector3 impulse);
  /**
   * @brief �ړ��ʂ̎擾
   */
  btVector3 GetVelocity();
  /**
   * @brief �ړ��ʂ̎w��
   */
  void SetVelocity(btVector3 velocity);
  /**
   * @brief ��]�ړ��ʂ̎擾
   */
  btVector3 GetAngularVelocity();
  /**
   * @brief ��]�ړ��ʂ̎w��

   */
  void SetAngularVelocity(btVector3 velocity);
  btMotionState* GetMotionState() const { return motion_state_.get(); }
  virtual void updateAction(btCollisionWorld* collisionWorld,
                            btScalar deltaTimeStep) override;
  virtual void debugDraw(btIDebugDraw* debugDrawer) override;

 protected:
  util::Transform* owner_transform_;
  std::shared_ptr<btCollisionShape> shape_;
  std::shared_ptr<btDefaultMotionState> motion_state_;
  btVector3 inertia_;
  std::shared_ptr<btRigidBody> rigid_body_;

};
}  // namespace bullet
}  // namespace legend

#endif  //! LEGEND_BULLET_COLLIDER_H_
