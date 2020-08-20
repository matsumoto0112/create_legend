#include "src/bullet/collider.h"

#include <set>

#include "src/actor/actor.h"
#include "src/bullet/bullet_helper.h"

namespace {
// 接触情報
struct ContactPoint {
  const btCollisionObject* pHitObj;
  btVector3 HitPos;    // 接触点
  btVector3 Normal;    // 接触方向　押し出す方向
  btScalar Dist;       // 接触距離 絶対値
  btVector3 GhostPos;  // ゴースト側の接触点
};

struct MyCollisionCallback : public btCollisionWorld::ContactResultCallback {
  MyCollisionCallback(std::vector<ContactPoint>& ct_points,
                      btCollisionObject* me)
      : ContactPoints(ct_points), pMe(me) {}

  btCollisionObject* pMe;
  btCollisionObject* pCancel = nullptr;  //(仮)このゴーストとは判定しない
  std::vector<ContactPoint>& ContactPoints;  //ここに結果追加
  btScalar Penetrate = 0.0f;
  virtual btScalar addSingleResult(btManifoldPoint& cp,
                                   const btCollisionObjectWrapper* colObj0Wrap,
                                   int partId0, int index0,
                                   const btCollisionObjectWrapper* colObj1Wrap,
                                   int partId1, int index1) override {
    if (pCancel == colObj0Wrap->getCollisionObject() ||
        pCancel == colObj1Wrap->getCollisionObject())
      return 0;

    bool is_A = pMe == colObj0Wrap->getCollisionObject();
    btScalar dist = cp.getDistance();
    // dist > 0の場合　接触していない 最短距離
    if (dist > -Penetrate) return 0;

    // 接触法線
    btVector3 normal = is_A ? cp.m_normalWorldOnB : -cp.m_normalWorldOnB;

    // 接触情報
    ContactPoint ct_point;
    ct_point.Dist = -dist;
    ct_point.GhostPos =
        is_A ? cp.getPositionWorldOnA() : cp.getPositionWorldOnB();
    ct_point.HitPos =
        is_A ? cp.getPositionWorldOnB() : cp.getPositionWorldOnA();
    ct_point.Normal = normal;
    ct_point.pHitObj = is_A ? colObj1Wrap->getCollisionObject()
                            : colObj0Wrap->getCollisionObject();
    ContactPoints.push_back(ct_point);

    return 0;  //戻り値に意味なし　呼ぶ側が使っていない
  }
};
}  // namespace

namespace legend {
namespace bullet {

Collider::Collider(actor::Actor* owner) : owner_(owner) {}
Collider::~Collider() {}
bool Collider::Update() { return true; }

btRigidBody* Collider::GetRigidBody() { return rigid_body_.get(); }

void Collider::ApplyCentralImpulse(const math::Vector3& impulse) {
  if (rigid_body_ == nullptr) {
    return;
  }

  rigid_body_->activate();
  rigid_body_->applyCentralImpulse(helper::TobtVector3(impulse));
}

btVector3 Collider::GetVelocity() {
  if (rigid_body_ == nullptr) {
    return btVector3(0, 0, 0);
  }
  return rigid_body_->getLinearVelocity();
}

void Collider::SetVelocity(const math::Vector3& velocity) {
  if (rigid_body_ == nullptr) {
    return;
  }
  rigid_body_->activate();
  rigid_body_->setLinearVelocity(helper::TobtVector3(velocity));
}

btVector3 Collider::GetAngularVelocity() {
  if (rigid_body_ == nullptr) {
    return btVector3(0, 0, 0);
  }
  return rigid_body_->getAngularVelocity();
}

void Collider::SetAngularVelocity(const math::Vector3& velocity) {
  if (rigid_body_ == nullptr) {
    return;
  }
  rigid_body_->setAngularVelocity(helper::TobtVector3(velocity));
}

void Collider::updateAction(btCollisionWorld* collisionWorld,
                            btScalar deltaTimeStep) {
  std::vector<ContactPoint> ct_points;
  MyCollisionCallback cb(ct_points, rigid_body_.get());
  cb.Penetrate = 0.0f;
  collisionWorld->contactTest(rigid_body_.get(), cb);

  std::set<Collider*> hit_object_list;
  for (auto&& b : ct_points) {
    Collider* other = static_cast<Collider*>(b.pHitObj->getUserPointer());
    hit_object_list.emplace(other);
  }

  for (auto&& a : hit_object_list) {
    OnHit(a);
  }
}

void Collider::debugDraw(btIDebugDraw* debugDrawer) {}

void Collider::UpdateOwnerTransform() const {
  btTransform tr;
  motion_state_->getWorldTransform(tr);
  owner_->GetTransformRef().SetPosition(helper::ToVector3(tr.getOrigin()));
  owner_->GetTransformRef().SetRotation(helper::ToQuaternion(tr.getRotation()));
}

void Collider::OnHit(Collider* other) {
  if (callback_) {
    callback_(other);
  }
}

int Collider::GetFlags() const { return rigid_body_->getCollisionFlags(); }

void Collider::SetFlags(int flags) { rigid_body_->setCollisionFlags(flags); }

}  // namespace bullet
}  // namespace legend
