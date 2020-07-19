#include "src/physics/collision.h"

#include "src/math/quaternion.h"

namespace legend {
namespace physics {

//�����̓��m�̏Փ˔���
bool Collision::Collision_OBB_OBB(BoundingBox& obb1, BoundingBox& obb2) {
  //�������̍X�V
  obb1.SetAxis();
  obb2.SetAxis();
  //�������Z�o
  math::Vector3 distance = obb2.GetPosition() - obb1.GetPosition();

  //�������̔�r
  if (!IsCompareLengthOBB(obb1, obb2, obb1.GetAxisX(), distance)) {
    MY_LOG(L"�Փ˂��܂���ł���");
    return false;
  }
  if (!IsCompareLengthOBB(obb1, obb2, obb1.GetAxisY(), distance)) {
    MY_LOG(L"�Փ˂��܂���ł���");
    return false;
  }
  if (!IsCompareLengthOBB(obb1, obb2, obb1.GetAxisZ(), distance)) {
    MY_LOG(L"�Փ˂��܂���ł���");
    return false;
  }
  if (!IsCompareLengthOBB(obb1, obb2, obb2.GetAxisX(), distance)) {
    MY_LOG(L"�Փ˂��܂���ł���");
    return false;
  }
  if (!IsCompareLengthOBB(obb1, obb2, obb2.GetAxisY(), distance)) {
    MY_LOG(L"�Փ˂��܂���ł���");
    return false;
  }
  if (!IsCompareLengthOBB(obb1, obb2, obb2.GetAxisZ(), distance)) {
    MY_LOG(L"�Փ˂��܂���ł���");
    return false;
  }

  //���������m�̊O�ς��r
  math::Vector3 vSep;
  vSep = math::Vector3::Cross(obb1.GetAxisX(), obb2.GetAxisX());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"�Փ˂��܂���ł���");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisX(), obb2.GetAxisY());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"�Փ˂��܂���ł���");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisX(), obb2.GetAxisZ());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"�Փ˂��܂���ł���");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisY(), obb2.GetAxisX());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"�Փ˂��܂���ł���");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisY(), obb2.GetAxisY());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"�Փ˂��܂���ł���");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisY(), obb2.GetAxisZ());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"�Փ˂��܂���ł���");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisZ(), obb2.GetAxisX());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"�Փ˂��܂���ł���");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisZ(), obb2.GetAxisY());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"�Փ˂��܂���ł���");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisZ(), obb2.GetAxisZ());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"�Փ˂��܂���ł���");
    return false;
  }

  return true;
}

bool Collision::Collision_OBB_Plane(BoundingBox& obb, Plane& plane) {
  //���ʂ̖@���ɑ΂���OBB�̎ˉe���̒������Z�o
  //�ߐڋ���
  float proximity_distance = 0;
  std::array<float, 3> scale_size;
  scale_size[0] = obb.GetScale().x;
  scale_size[1] = obb.GetScale().y;
  scale_size[2] = obb.GetScale().z;
  math::Matrix4x4 rotate_matrix =
      math::Matrix4x4::CreateRotation(obb.GetRotation());

  for (i32 i = 0; i < 3; i++) {
    math::Vector3 axis = obb.GetDirection(i) * obb.GetLength(i) * scale_size[i];
    proximity_distance += fabs(
        math::Vector3::Dot(math::Matrix4x4::MultiplyCoord(axis, rotate_matrix),
                           plane.GetNormal()));
  }

  //���ʂ�OBB�̋������Z�o
  float distance = math::Vector3::Dot(obb.GetPosition() - plane.GetPosition(),
                                      plane.GetNormal());

  if (fabs(distance) - proximity_distance >= 0) {
    MY_LOG(L"�Փ˂��܂���ł���");
    return false;
  }

  ////�߂�����
  // float return_distance = 0;
  // if (distance > 0) {
  //  return_distance = proximity_distance - fabs(distance);
  //} else {
  //  return_distance = proximity_distance + fabs(distance);
  //}

  return true;
}

// OBB�̓��e������r
bool Collision::IsCompareLengthOBB(BoundingBox& obb1, BoundingBox& obb2,
                                   math::Vector3 vSep, math::Vector3 distance) {
  //��������̋���
  float length = fabsf(math::Vector3::Dot(vSep, distance));

  //��������ōł�����obb1�̒��_�܂ł̋���
  float len_a =
      fabsf(math::Vector3::Dot(obb1.GetAxisX(), vSep) * obb1.GetLength(0) +
            math::Vector3::Dot(obb1.GetAxisY(), vSep) * obb1.GetLength(1) +
            math::Vector3::Dot(obb1.GetAxisZ(), vSep) * obb1.GetLength(2));

  //��������ōł�����obb2�̒��_�܂ł̋���
  float len_b =
      fabsf(math::Vector3::Dot(obb2.GetAxisX(), vSep) * obb2.GetLength(0) +
            math::Vector3::Dot(obb2.GetAxisY(), vSep) * obb2.GetLength(1) +
            math::Vector3::Dot(obb2.GetAxisZ(), vSep) * obb2.GetLength(2));

  if (length > len_a + len_b) {
    //�Փ˂��Ă��Ȃ�
    return false;
  }

  //�Փ˂��Ă���
  return true;
}
}  // namespace physics
}  // namespace legend