#include "src/bullet/trigger.h"

#include "src/actor/actor.h"
#include "src/bullet/bullet_helper.h"

namespace {
struct ContactPoint {
  const btCollisionObject* pHitObj;
  btVector3 HitPos;    // �ڐG�_
  btVector3 Normal;    // �ڐG�����@�����o������
  btScalar Dist;       // �ڐG���� ��Βl
  btVector3 GhostPos;  // �S�[�X�g���̐ڐG�_
};

// contactTest�AcontactPairTest�p�R�[���o�b�N
struct MyContactResultCallback
    : public btCollisionWorld::ContactResultCallback {
  MyContactResultCallback(std::vector<ContactPoint>& ct_points,
                          btGhostObject* me)
      : ContactPoints(ct_points), pMe(me) {}

  btGhostObject* pMe;
  btGhostObject* pCancel = nullptr;  //(��)���̃S�[�X�g�Ƃ͔��肵�Ȃ�
  std::vector<ContactPoint>& ContactPoints;  //�����Ɍ��ʒǉ�
  btScalar Penetrate = 0.0f;

  virtual btScalar addSingleResult(btManifoldPoint& cp,
                                   const btCollisionObjectWrapper* colObj0Wrap,
                                   int partId0, int index0,
                                   const btCollisionObjectWrapper* colObj1Wrap,
                                   int partId1, int index1) {
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

Trigger::Trigger(actor::Actor* owner) : owner_(owner) {}

Trigger::~Trigger() {}

bool Trigger::Update() { return true; }

void Trigger::updateAction(btCollisionWorld* collisionWorld,
                           btScalar deltaTimeStep) {
  if (!ghost_) return;
  std::vector<ContactPoint> ct_points;
  MyContactResultCallback cb(ct_points, ghost_.get());
  cb.Penetrate = 0.0f;
  // cb.pCancel = cancel_ghost;
  collisionWorld->contactTest(ghost_.get(), cb);

  for (auto&& b : ct_points) {
    Trigger* other = static_cast<Trigger*>(b.pHitObj->getUserPointer());
    if (other) {
      OnTrigger(other);
    }
  }
}

void Trigger::debugDraw(btIDebugDraw* debugDrawer) {}

void Trigger::OnTrigger(Trigger* other) {
  if (callback_) {
    callback_(other);
  }
}

}  // namespace bullet
}  // namespace legend