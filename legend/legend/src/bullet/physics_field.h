#ifndef LEGEND_BULLET_PHYSICS_FIELD_H_
#define LEGEND_BULLET_PHYSICS_FIELD_H_

/**
 * @file physics_field.h
 * @brief 物理演算管理クラス
 */
#include <btBulletDynamicsCommon.h>

#include "src/bullet/collider.h"
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
   * @brief ライン描画
   */
  void DrawLine(const btVector3& from, const btVector3& to,
                const btVector3& color);
  /**
   * @brief 物理演算を行うオブジェクトの追加
   */
  void AddRigidBody(btRigidBody* rigid_body);
  /**
   * @brief コライダーを追加する
   */
  void AddCollision(std::shared_ptr<Collider> collider);
  /**
   * @brief コライダーを削除する
   */
  void RemoveCollision(std::shared_ptr<Collider> collider);
  btCollisionWorld::AllHitsRayResultCallback RayCast(
      const math::Vector3& start, const math::Vector3& end) const;

 private:
  std::shared_ptr<legend::directx::BulletDebugDraw> debug_drawer_;
  std::vector<std::shared_ptr<Collider>> colliders_;

  std::shared_ptr<btCollisionConfiguration> config_;
  std::shared_ptr<btCollisionDispatcher> dispatcher_;
  std::shared_ptr<btDbvtBroadphase> broadphase_;
  std::shared_ptr<btSequentialImpulseConstraintSolver> solver_;
  std::shared_ptr<btDynamicsWorld> world_;
};

}  // namespace bullet
}  // namespace legend

#endif  //! LEGEND_BULLET_PHYSICS_FIELD_H_
