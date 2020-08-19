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
class Collider {
 public:
  /**
   * @brief コンストラクタ
   */
  Collider();
  /**
   * @brief デストラクタ
   */
  ~Collider();
  /**
   * @brief 更新処理
   */
  virtual bool Update();
  btRigidBody* GetRigidBody();

 protected:
  std::shared_ptr<btCollisionShape> shape_;
  std::shared_ptr<btDefaultMotionState> motion_state_;
  btVector3 inertia_;
  std::shared_ptr<btRigidBody> rigid_body_;
};
}  // namespace bullet
}  // namespace legend

#endif  //! LEGEND_BULLET_COLLIDER_H_
