#include "src/bullet/collider.h"

#include <set>

#include "src/actor/actor.h"
#include "src/bullet/bullet_helper.h"

namespace {
// �ڐG���
struct ContactPoint {
  const btCollisionObject* pHitObj;
  btVector3 HitPos;    // �ڐG�_
  btVector3 Normal;    // �ڐG�����@�����o������
  btScalar Dist;       // �ڐG���� ��Βl
  btVector3 GhostPos;  // �S�[�X�g���̐ڐG�_
};

struct MyCollisionCallback : public btCollisionWorld::ContactResultCallback {
  MyCollisionCallback(std::vector<ContactPoint>& ct_points,
                      btCollisionObject* me)
      : ContactPoints(ct_points), pMe(me) {}

  btCollisionObject* pMe;
  btCollisionObject* pCancel = nullptr;  //(��)���̃S�[�X�g�Ƃ͔��肵�Ȃ�
  std::vector<ContactPoint>& ContactPoints;  //�����Ɍ��ʒǉ�
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
    // dist > 0�̏ꍇ�@�ڐG���Ă��Ȃ� �ŒZ����
    if (dist > -Penetrate) return 0;

    // �ڐG�@��
    btVector3 normal = is_A ? cp.m_normalWorldOnB : -cp.m_normalWorldOnB;

    // �ڐG���
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

    return 0;  //�߂�l�ɈӖ��Ȃ��@�Ăԑ����g���Ă��Ȃ�
  }
};
}  // namespace

namespace legend {
namespace bullet {

Collider::Collider(actor::Actor* owner) : owner_(owner) {}
Collider::~Collider() {
  if (rigid_body_) {
    rigid_body_->setUserPointer(nullptr);
    rigid_body_->setMotionState(nullptr);
    rigid_body_->setCollisionShape(nullptr);
  }
}
bool Collider::Update() { return true; }

void Collider::InitBox(const BoxInitializeParameter& parameter) {
  //�����̂ɐݒ�
  shape_ = std::make_shared<btBoxShape>(helper::TobtVector3(parameter.scale));

  motion_state_ = std::make_shared<btDefaultMotionState>(
      btTransform(helper::TobtQuaternion(parameter.rotation),
                  helper::TobtVector3(parameter.position)));

  //�������[�����g�̌v�Z
  inertia_ = btVector3(0, 0, 0);
  shape_->calculateLocalInertia(parameter.mass, inertia_);
  //���̃I�u�W�F�N�g����
  rigid_body_ = std::make_shared<btRigidBody>(
      btScalar(parameter.mass), motion_state_.get(), shape_.get(), inertia_);
  rigid_body_->setUserPointer(this);
  rigid_body_->setFriction(btScalar(parameter.friction));
}

void Collider::InitSphere(const SphereInitializeParameter& parameter) {
  //���̂ɐݒ�
  shape_ = std::make_shared<btSphereShape>(parameter.radius);

  motion_state_ = std::make_shared<btDefaultMotionState>(
      btTransform(helper::TobtQuaternion(parameter.rotation),
                  helper::TobtVector3(parameter.position)));

  //�������[�����g�̌v�Z
  inertia_ = btVector3(0, 0, 0);
  shape_->calculateLocalInertia(parameter.mass, inertia_);

  //���̃I�u�W�F�N�g����
  rigid_body_ = std::make_shared<btRigidBody>(
      parameter.mass, motion_state_.get(), shape_.get(), inertia_);
  rigid_body_->setUserPointer(this);

  //���C�W����ݒ�
  rigid_body_->setFriction(parameter.friction);
}

btRigidBody* Collider::GetRigidBody() { return rigid_body_.get(); }

void Collider::ApplyCentralImpulse(const math::Vector3& impulse) {
  if (rigid_body_ == nullptr) {
    return;
  }

  rigid_body_->activate();
  rigid_body_->applyCentralImpulse(helper::TobtVector3(impulse));
}

math::Vector3 Collider::GetVelocity() {
  if (rigid_body_) {
    return helper::ToVector3(rigid_body_->getLinearVelocity());
  }
  return math::Vector3::kZeroVector;
}

void Collider::SetVelocity(const math::Vector3& velocity) {
  if (rigid_body_ == nullptr) {
    return;
  }
  rigid_body_->activate();
  rigid_body_->setLinearVelocity(helper::TobtVector3(velocity));
}

math::Vector3 Collider::GetAngularVelocity() {
  if (rigid_body_) {
    return helper::ToVector3(rigid_body_->getAngularVelocity());
  }
  return math::Vector3::kZeroVector;
}

void Collider::SetAngularVelocity(const math::Vector3& velocity) {
  if (rigid_body_ == nullptr) {
    return;
  }
  rigid_body_->activate();
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

void Collider::SetTransform(const util::Transform& transform) {
  btTransform tr;
  tr.setOrigin(helper::TobtVector3(transform.GetPosition()));
  tr.setRotation(helper::TobtQuaternion(transform.GetRotation()));
  motion_state_->setWorldTransform(tr);
  rigid_body_->setMotionState(motion_state_.get());
}

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
