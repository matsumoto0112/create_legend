#ifndef LEGEND_BULLET_COLLIDER_H_
#define LEGEND_BULLET_COLLIDER_H_

/**
 * @file collider.h
 * @brief
 */

#include <btBulletDynamicsCommon.h>

#include "src/math/quaternion.h"
#include "src/util/transform.h"
#include "src/bullet/shape_type.h"

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
  virtual ~Collider();
  /**
   * @brief �X�V����
   */
  virtual bool Update();
  /**
   * @brief ����������
   */
  void InitBox(const BoxInitializeParameter& parameter);
  /**
   * @brief ����������
   */
  void InitSphere(const SphereInitializeParameter& parameter);
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
  math::Vector3 GetVelocity();
  /**
   * @brief �ړ��ʂ̎w��
   */
  void SetVelocity(const math::Vector3& velocity);
  /**
   * @brief ��]�ړ��ʂ̎擾
   */
  math::Vector3 GetAngularVelocity();
  /**
   * @brief ��]�ړ��ʂ̎w��
   */
  void SetAngularVelocity(const math::Vector3& velocity);
  /**
   * @brief ���݂̃��[���h�ϊ��s��̏�Ԃ��擾����
   */
  btMotionState* GetMotionState() const { return motion_state_.get(); }
  /**
   * @brief �����n�X�V���ɌĂ΂��
   */
  virtual void updateAction(btCollisionWorld* collisionWorld,
                            btScalar deltaTimeStep) override;
  /**
   * @brief �f�o�b�O�`�掞�ɌĂ΂��i���g�p�j
   */
  virtual void debugDraw(btIDebugDraw* debugDrawer) override;
  void SetTransform(const util::Transform& transform);
  /**
   * @brief ���������L���Ă���A�N�^�[�̃g�����X�t�H�[�����X�V����
   */
  virtual void UpdateOwnerTransform() const;
  /**
   * @breif �Փˎ��R�[���o�b�N
   */
  virtual void OnHit(Collider* other);
  /**
   * @brief �Փˎ��R�[���o�b�N�֐��o�^
   */
  void SetCollisionCallBack(CollisionCallback callback) {
    this->callback_ = callback;
  }
  /**
   * @brief ���������L���Ă���A�N�^�[���擾����
   */
  actor::Actor* GetOwner() const { return owner_; }

  int GetFlags() const;
  void SetFlags(int flags);

  btCollisionShape* GetShape() const { return shape_.get(); }

  std::map<Collider*, btVector3> GetHitPositions();

 protected:
  actor::Actor* owner_;
  std::shared_ptr<btCollisionShape> shape_;
  std::shared_ptr<btDefaultMotionState> motion_state_;
  btVector3 inertia_;
  std::shared_ptr<btRigidBody> rigid_body_;
  CollisionCallback callback_;
  std::map<Collider*, btVector3> hit_positions_;
};
}  // namespace bullet
}  // namespace legend

#endif  //! LEGEND_BULLET_COLLIDER_H_
