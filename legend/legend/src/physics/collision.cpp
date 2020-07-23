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

// OBB�̓��e������r
bool Collision::IsCompareLengthOBB(BoundingBox& obb1, BoundingBox& obb2,
                                   math::Vector3 v_sep,
                                   math::Vector3 distance) {
  //��������̋���
  float length = math::util::Abs(math::Vector3::Dot(v_sep, distance));

  //��������ōł�����obb1�̒��_�܂ł̋���
  float len_a = math::util::Abs(
      math::Vector3::Dot(obb1.GetAxisX(), v_sep) * obb1.GetLength(0) +
      math::Vector3::Dot(obb1.GetAxisY(), v_sep) * obb1.GetLength(1) +
      math::Vector3::Dot(obb1.GetAxisZ(), v_sep) * obb1.GetLength(2));

  //��������ōł�����obb2�̒��_�܂ł̋���
  float len_b = math::util::Abs(
      math::Vector3::Dot(obb2.GetAxisX(), v_sep) * obb2.GetLength(0) +
      math::Vector3::Dot(obb2.GetAxisY(), v_sep) * obb2.GetLength(1) +
      math::Vector3::Dot(obb2.GetAxisZ(), v_sep) * obb2.GetLength(2));

  if (length > len_a + len_b) {
    //�Փ˂��Ă��Ȃ�
    return false;
  }

  //�Փ˂��Ă���
  return true;
}

//�����̂ƕ��ʂ̏Փ˔���
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

  if (math::util::Abs(distance) - proximity_distance > 0) {
    MY_LOG(L"�Փ˂��܂���ł���");
    return false;
  }

  //�߂�����
  float return_distance = 0;
  if (distance > 0) {
    return_distance = proximity_distance - fabs(distance);
  } else {
    return_distance = proximity_distance + fabs(distance);
  }

  //���W�̏C��
  math::Vector3 normal = plane.GetNormal();
  math::Vector3 return_vector = math::Vector3::kZeroVector;
  if (normal.x > 0) {
    return_vector = math::Vector3(return_distance, 0, 0) + obb.GetPosition();
  } else if (normal.y > 0) {
    return_vector = math::Vector3(0, return_distance, 0) + obb.GetPosition();
  } else if (normal.z > 0) {
    return_vector = math::Vector3(0, 0, return_distance) + obb.GetPosition();
  }
  obb.SetPosition(return_vector);

  return true;
}

//���ƕ��ʂ̏Փ˔���
bool Collision::Collision_Sphere_Plane(Sphere& sphere, Plane& plane) {
  //���ʂƋ��̋������Z�o
  float distance = math::Vector3::Dot(
      sphere.GetPosition() - plane.GetPosition(), plane.GetNormal());

  for (i32 i = 0; i < 3; i++) {
    if (math::util::Abs(distance) <= sphere.GetRadius()) {
      return true;
    }
  }

  MY_LOG(L"�Փ˂��܂���ł���");
  return false;
}

//���C�̒����𒲂ׂ�
float Collision::GetRayLength(Ray& ray) const {
  return math::Vector3::Dot(ray.GetDirection(), ray.GetDirection());
}

//���C�ƒ����̂̏Փ˔���
bool Collision::Collision_Ray_OBB(Ray& ray, BoundingBox& obb) {
  float length = GetRayLength(ray);
  if (length == 0.0f) {
    MY_LOG(L"���C�̒�����0�ł�");
    return false;
  }

  obb.SetAxis();
  math::Vector3 distance = obb.GetPosition() - ray.GetStartPosition();
  if (!IsCheckLength_Ray_Obb(ray, obb, obb.GetAxisX(), distance)) {
      MY_LOG(L"�Փ˂��܂���ł���");
      return false;
  }
  if (!IsCheckLength_Ray_Obb(ray, obb, obb.GetAxisY(), distance)) {
      MY_LOG(L"�Փ˂��܂���ł���");
      return false;
  }
  if (!IsCheckLength_Ray_Obb(ray, obb, obb.GetAxisZ(), distance)) {
      MY_LOG(L"�Փ˂��܂���ł���");
      return false;
  }

  //�Փ˂��Ă���
  return true;
}

bool Collision::IsCheckLength_Ray_Obb(Ray& ray, BoundingBox& obb,
                                      math::Vector3 v_sep,
                                      math::Vector3 distance) {
  //��������̋���
  float length = math::util::Abs(math::Vector3::Dot(v_sep, distance));

  //��������ōł�����obb�̒��_�܂ł̋���
  float len_a = math::util::Abs(
      math::Vector3::Dot(obb.GetAxisX(), v_sep) * obb.GetLength(0) +
      math::Vector3::Dot(obb.GetAxisY(), v_sep) * obb.GetLength(1) +
      math::Vector3::Dot(obb.GetAxisZ(), v_sep) * obb.GetLength(2));

  float len_b = math::util::Abs(math::Vector3::Dot(ray.GetDirection(), v_sep) *
                                ray.GetDistance());

  if (length > len_a + len_b) {
    //�Փ˂��Ă��Ȃ�
    return false;
  }

  //�Փ˂��Ă���
  return true;
}

//���C�ƕ��ʂ̏Փ˔���
bool Collision::Collision_Ray_Plane(Ray& ray, Plane& plane) {
  float length = GetRayLength(ray);
  if (length == 0.0f) {
    MY_LOG(L"���C�̒�����0�ł�");
    return false;
  }
  float x = plane.GetPosition().x - ray.GetStartPosition().x;
  float y = plane.GetPosition().y - ray.GetStartPosition().y;
  float z = plane.GetPosition().z - ray.GetStartPosition().z;

  return true;
}

//���C�Ƌ��̏Փ˔���
bool Collision::Collision_Ray_Sphere(Ray& ray, Sphere& sphere) {
  float length = GetRayLength(ray);
  if (length == 0.0f) {
    MY_LOG(L"���C�̒�����0�ł�");
    return false;
  }
  float x = sphere.GetPosition().x - ray.GetStartPosition().x;
  float y = sphere.GetPosition().y - ray.GetStartPosition().y;
  float z = sphere.GetPosition().z - ray.GetStartPosition().z;
  math::Vector3 sphere_position = math::Vector3(x, y, z);

  float b = math::Vector3::Dot(ray.GetDirection(), sphere_position);
  float c = math::Vector3::Dot(sphere_position, sphere_position) -
            sphere.GetSquareRadius();

  float s = b * b - length * c;
  if (s < 0.0f) {
    MY_LOG(L"�Փ˂��܂���ł���");
    return false;
  }

  s = sqrtf(s);
  float a1 = (b - s) / length;
  float a2 = (b + s) / length;

  if (a1 < 0.0f || a2 < 0.0f) {
    MY_LOG(L"���C�̔��΂ŏՓ˂��܂���");
    return false;
  }

  return true;
}
}  // namespace physics
}  // namespace legend