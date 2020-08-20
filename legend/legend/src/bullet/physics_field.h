#ifndef LEGEND_BULLET_PHYSICS_FIELD_H_
#define LEGEND_BULLET_PHYSICS_FIELD_H_

/**
 * @file physics_field.h
 * @brief 物理演算管理クラス
 */
#include <btBulletDynamicsCommon.h>

#include "src/bullet/collider.h"
#include "src/bullet/trigger.h"
#include "src/camera/camera.h"
#include "src/directx/bullet_debug_draw.h"

namespace legend {
namespace bullet {

class PhysicsField {
 public:
  /**
   * @brief コンストラクタ
   */
  PhysicsField();
  /**
   * @brief デストラクタ
   */
  ~PhysicsField();
  /**
   * @brief 初期化処理
   */
  bool Init();
  /**
   * @brief 更新
   */
  bool Update();
  /**
   * @brief デバック用の描画
   */
  void DebugDraw(camera::Camera* rendering_camera);
  /**
   * @brief 物理演算を行うオブジェクトの追加
   */
  void AddRigidBody(btRigidBody* rigid_body);
  void AddCollision(std::shared_ptr<Collider> collider);
  //void AddTrigger(std::shared_ptr<Trigger> trigger);
  //void RemoveCollision(std::shared_ptr<Collider> collider);
  //void RemoveTrigger(std::shared_ptr<Trigger> trigger);

 private:
  std::shared_ptr<legend::directx::BulletDebugDraw> debug_drawer_;
  std::vector<std::shared_ptr<Collider>> colliders_;
  //std::vector<std::shared_ptr<Trigger>> triggers_;

  std::shared_ptr<btCollisionConfiguration> config_;
  std::shared_ptr<btCollisionDispatcher> dispatcher_;
  std::shared_ptr<btDbvtBroadphase> broadphase_;
  std::shared_ptr<btSequentialImpulseConstraintSolver> solver_;
  std::shared_ptr<btDynamicsWorld> world_;
};

}  // namespace bullet
}  // namespace legend

#endif  //! LEGEND_BULLET_PHYSICS_FIELD_H_
