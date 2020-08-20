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
   * @brief コンストラクタ
   */
  Collider(util::Transform* owner_transform);
  /**
   * @brief デストラクタ
   */
  ~Collider();
  /**
   * @brief 更新処理
   */
  virtual bool Update();

  /**
   * @brief 剛体オブジェクトを取得
   */
  btRigidBody* GetRigidBody();
  /**
   * @brief オブジェクトの中心から指定の力を加える
   */
  void ApplyCentralImpulse(btVector3 impulse);
  /**
   * @brief 移動量の取得
   */
  btVector3 GetVelocity();
  /**
   * @brief 移動量の指定
   */
  void SetVelocity(btVector3 velocity);
  /**
   * @brief 回転移動量の取得
   */
  btVector3 GetAngularVelocity();
  /**
   * @brief 回転移動量の指定

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
