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
   * @brief コンストラクタ
   */
  Collider(actor::Actor* owner);
  /**
   * @brief デストラクタ
   */
  virtual ~Collider();
  /**
   * @brief 更新処理
   */
  virtual bool Update();
  /**
   * @brief 初期化処理
   */
  void InitBox(const BoxInitializeParameter& parameter);
  /**
   * @brief 初期化処理
   */
  void InitSphere(const SphereInitializeParameter& parameter);
  /**
   * @brief 剛体オブジェクトを取得
   */
  btRigidBody* GetRigidBody();
  /**
   * @brief オブジェクトの中心から指定の力を加える
   */
  void ApplyCentralImpulse(const math::Vector3& impulse);
  /**
   * @brief 移動量の取得
   */
  math::Vector3 GetVelocity();
  /**
   * @brief 移動量の指定
   */
  void SetVelocity(const math::Vector3& velocity);
  /**
   * @brief 回転移動量の取得
   */
  math::Vector3 GetAngularVelocity();
  /**
   * @brief 回転移動量の指定
   */
  void SetAngularVelocity(const math::Vector3& velocity);
  /**
   * @brief 現在のワールド変換行列の状態を取得する
   */
  btMotionState* GetMotionState() const { return motion_state_.get(); }
  /**
   * @brief 物理系更新時に呼ばれる
   */
  virtual void updateAction(btCollisionWorld* collisionWorld,
                            btScalar deltaTimeStep) override;
  /**
   * @brief デバッグ描画時に呼ばれる（未使用）
   */
  virtual void debugDraw(btIDebugDraw* debugDrawer) override;
  void SetTransform(const util::Transform& transform);
  /**
   * @brief 自分を所有しているアクターのトランスフォームを更新する
   */
  virtual void UpdateOwnerTransform() const;
  /**
   * @breif 衝突時コールバック
   */
  virtual void OnHit(Collider* other);
  /**
   * @brief 衝突時コールバック関数登録
   */
  void SetCollisionCallBack(CollisionCallback callback) {
    this->callback_ = callback;
  }
  /**
   * @brief 自分を所有しているアクターを取得する
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
