#include "src/physics/collision.h"

#include "src/math/quaternion.h"

namespace legend {
namespace physics {

//�����̓��m�̏Փ˔���
bool Collision::Collision_OBB_OBB(BoundingBox& obb1, BoundingBox& obb2,
                                  bool obb1_move, bool obb2_move) {
  //�������̍X�V
  obb1.SetAxis();
  obb2.SetAxis();

  //�������Z�o
  math::Vector3 distance = obb2.GetPosition() - obb1.GetPosition();

  //�������̔�r
  if (!IsCompareLengthOBB(obb1, obb2, obb1.GetAxisX(), distance)) {
    return false;
  }
  if (!IsCompareLengthOBB(obb1, obb2, obb1.GetAxisY(), distance)) {
    return false;
  }
  if (!IsCompareLengthOBB(obb1, obb2, obb1.GetAxisZ(), distance)) {
    return false;
  }
  if (!IsCompareLengthOBB(obb1, obb2, obb2.GetAxisX(), distance)) {
    return false;
  }
  if (!IsCompareLengthOBB(obb1, obb2, obb2.GetAxisY(), distance)) {
    return false;
  }
  if (!IsCompareLengthOBB(obb1, obb2, obb2.GetAxisZ(), distance)) {
    return false;
  }

  //���������m�̊O�ς��r
  math::Vector3 v_sep;
  v_sep = math::Vector3::Cross(obb1.GetAxisX(), obb2.GetAxisX());
  if (!IsCompareLengthOBB(obb1, obb2, v_sep, distance)) {
    return false;
  }
  v_sep = math::Vector3::Cross(obb1.GetAxisX(), obb2.GetAxisY());
  if (!IsCompareLengthOBB(obb1, obb2, v_sep, distance)) {
    return false;
  }
  v_sep = math::Vector3::Cross(obb1.GetAxisX(), obb2.GetAxisZ());
  if (!IsCompareLengthOBB(obb1, obb2, v_sep, distance)) {
    return false;
  }
  v_sep = math::Vector3::Cross(obb1.GetAxisY(), obb2.GetAxisX());
  if (!IsCompareLengthOBB(obb1, obb2, v_sep, distance)) {
    return false;
  }
  v_sep = math::Vector3::Cross(obb1.GetAxisY(), obb2.GetAxisY());
  if (!IsCompareLengthOBB(obb1, obb2, v_sep, distance)) {
    return false;
  }
  v_sep = math::Vector3::Cross(obb1.GetAxisY(), obb2.GetAxisZ());
  if (!IsCompareLengthOBB(obb1, obb2, v_sep, distance)) {
    return false;
  }
  v_sep = math::Vector3::Cross(obb1.GetAxisZ(), obb2.GetAxisX());
  if (!IsCompareLengthOBB(obb1, obb2, v_sep, distance)) {
    return false;
  }
  v_sep = math::Vector3::Cross(obb1.GetAxisZ(), obb2.GetAxisY());
  if (!IsCompareLengthOBB(obb1, obb2, v_sep, distance)) {
    return false;
  }
  v_sep = math::Vector3::Cross(obb1.GetAxisZ(), obb2.GetAxisZ());
  if (!IsCompareLengthOBB(obb1, obb2, v_sep, distance)) {
    return false;
  }

  if (obb1.GetIsTrigger() || obb2.GetIsTrigger()) {
    return true;
  }

  math::Vector3 adjust_pos = math::Vector3::kZeroVector;
  math::Vector3 obb1_len = math::Matrix4x4::MultiplyCoord(
      obb1.GetLength(), obb1.GetRotation().ToMatrix());
  math::Vector3 obb2_len = math::Matrix4x4::MultiplyCoord(
      obb2.GetLength(), obb2.GetRotation().ToMatrix());
  float obb1_left = obb1.GetPosition().x - obb1_len.x;
  float obb1_right = obb1.GetPosition().x + obb1_len.x;
  float obb1_back = obb1.GetPosition().z - obb1_len.z;
  float obb1_front = obb1.GetPosition().z + obb1_len.z;
  float obb2_left = obb2.GetPosition().x - obb2_len.x;
  float obb2_right = obb2.GetPosition().x + obb2_len.x;
  float obb2_back = obb2.GetPosition().z - obb2_len.z;
  float obb2_front = obb2.GetPosition().z + obb2_len.z;
  AdjustPosition(adjust_pos, obb1_left, obb1_right, obb1_front, obb1_back,
                 obb2_left, obb2_right, obb2_front, obb2_back);
  if (obb1_move && obb2_move) {
      adjust_pos /= 2.0f;
      obb1.SetPosition(obb1.GetPosition() - adjust_pos);
      obb2.SetPosition(obb2.GetPosition() + adjust_pos);
  }
  else if(obb1_move) {
      obb1.SetPosition(obb1.GetPosition() - adjust_pos);
  }
  else if (obb2_move) {
      obb2.SetPosition(obb2.GetPosition() + adjust_pos);
  }
  return true;
}

// OBB�̓��e������r
bool Collision::IsCompareLengthOBB(BoundingBox& obb1, BoundingBox& obb2,
                                   math::Vector3 v_sep,
                                   math::Vector3 distance) {
  //��������̋���
  float length = math::util::Abs(math::Vector3::Dot(v_sep, distance));

  float obb1_x =
      math::Vector3::Dot(obb1.GetAxisX(), v_sep) * obb1.GetLength().x;
  float obb1_y =
      math::Vector3::Dot(obb1.GetAxisY(), v_sep) * obb1.GetLength().y;
  float obb1_z =
      math::Vector3::Dot(obb1.GetAxisZ(), v_sep) * obb1.GetLength().z;
  float obb2_x =
      math::Vector3::Dot(obb2.GetAxisX(), v_sep) * obb2.GetLength().x;
  float obb2_y =
      math::Vector3::Dot(obb2.GetAxisY(), v_sep) * obb2.GetLength().y;
  float obb2_z =
      math::Vector3::Dot(obb2.GetAxisZ(), v_sep) * obb2.GetLength().z;

  //��������ōł�����obb1�̒��_�܂ł̋���
  float len_a = math::util::Abs(obb1_x + obb1_y + obb1_z);

  //��������ōł�����obb2�̒��_�܂ł̋���
  float len_b = math::util::Abs(obb2_x + obb2_y + obb2_z);

  if (length > len_a + len_b) {
    //�Փ˂��Ă��Ȃ�
    return false;
  }

  //�Փ˂��Ă���
  return true;
}

void Collision::AdjustPosition(math::Vector3& adjust_pos, float left1,
                               float right1, float front1, float back1,
                               float left2, float right2, float front2,
                               float back2) {
  if (left1 < right2) {
    adjust_pos.x = left1 - right2;
  } else if (right1 > left2) {
    adjust_pos.x = right1 - left2;
  }
  if (back1 < front2) {
    adjust_pos.z = back1 - front2;
  } else if (front1 > back2) {
    adjust_pos.z = front1 - back2;
  }

  if (math::util::Abs(adjust_pos.x) <= math::util::Abs(adjust_pos.z)) {
    adjust_pos.z = 0;
  } else {
    adjust_pos.x = 0;
  }
}

//�����̂ƕ��ʂ̏Փ˔���
bool Collision::Collision_OBB_Plane(BoundingBox& obb, Plane& plane) {
  //���ʂ̖@���ɑ΂���OBB�̎ˉe���̒������Z�o
  //�ߐڋ���
  float proximity_distance = 0;
  math::Matrix4x4 rotate_matrix = obb.GetRotation().ToMatrix();

  float length = 0;
  for (i32 i = 0; i < 3; i++) {
    if (i == 0)
      length = obb.GetLength().x;
    else if (i == 1)
      length = obb.GetLength().y;
    else
      length = obb.GetLength().z;
    math::Vector3 axis = obb.GetDirection(i) * length;
    proximity_distance += math::util::Abs(
        math::Vector3::Dot(math::Matrix4x4::MultiplyCoord(axis, rotate_matrix),
                           plane.GetNormal()));
  }

  //���ʂ�OBB�̋������Z�o
  float distance = math::Vector3::Dot(obb.GetPosition() - plane.GetPosition(),
                                      plane.GetNormal());

  if (math::util::Abs(distance) - proximity_distance > 0) {
    // MY_LOG(L"�Փ˂��܂���ł���");
    return false;
  }

  if (obb.GetIsTrigger()) {
    //�g���K�[�Ȃ�΂����܂�
    // MY_LOG(L"�g���K�[����ł�");
    return true;
  }

  //�߂�����
  float return_distance = 0;
  if (distance > 0) {
    return_distance = proximity_distance - math::util::Abs(distance);
  } else {
    return_distance = proximity_distance + math::util::Abs(distance);
  }

  //���W�̏C��
  math::Vector3 normal = plane.GetNormal();
  math::Vector3 return_vector = math::Vector3::kZeroVector;
  if (normal.x > 0) {
    return_vector += math::Vector3(return_distance, 0, 0);
  } else if (normal.y > 0) {
    return_vector += math::Vector3(0, return_distance, 0);
  } else if (normal.z > 0) {
    return_vector += math::Vector3(0, 0, return_distance);
  }
  obb.SetPosition(obb.GetPosition() + return_vector);

  return true;
}

bool Collision::Collision_OBB_DeskOBB(BoundingBox& obb, BoundingBox& desk_obb) {
  //�ߐڋ���
  float proximity_distance = 0;
  math::Matrix4x4 rotate_matrix = obb.GetRotation().ToMatrix();

  math::Vector3 normal = math::Vector3::kUpVector;
  math::Vector3 desk_surface = math::Vector3(0, desk_obb.GetLength().y, 0);
  float length = 0;
  for (i32 i = 0; i < 3; i++) {
    if (i == 0)
      length = obb.GetLength().x;
    else if (i == 1)
      length = obb.GetLength().y;
    else
      length = obb.GetLength().z;
    math::Vector3 axis = obb.GetDirection(i) * length;
    proximity_distance += math::util::Abs(
        math::Vector3::Dot(math::Matrix4x4::MultiplyCoord(axis, rotate_matrix),
                           normal) +
        desk_obb.GetLength().y);
  }

  //�����̂Ɗ��̋������Z�o
  float distance = math::Vector3::Dot(
      obb.GetPosition() - desk_obb.GetPosition() + desk_surface, normal);

  float desk_right = desk_obb.GetPosition().x + desk_obb.GetLength().x;
  float desk_left = desk_obb.GetPosition().x - desk_obb.GetLength().x;
  float desk_forward = desk_obb.GetPosition().z + desk_obb.GetLength().z;
  float desk_back = desk_obb.GetPosition().z - desk_obb.GetLength().z;

  if (obb.GetPosition().x < desk_left || obb.GetPosition().x > desk_right ||
      obb.GetPosition().z < desk_back || obb.GetPosition().z > desk_forward) {
    // MY_LOG(L"���̊O�ɂ��܂�");
    obb.SetOnGround(false);
    return false;
  }

  if (math::util::Abs(distance) - proximity_distance > 0) {
    // MY_LOG(L"�Փ˂��܂���ł���");
    obb.SetOnGround(false);
    return false;
  }

  if (obb.GetIsTrigger()) {
    //�g���K�[�Ȃ�΂����܂�
    // MY_LOG(L"�g���K�[����ł�");
    return true;
  }

  //�߂�����
  float return_distance = 0;
  if (distance > 0) {
    return_distance = proximity_distance - math::util::Abs(distance);
  } else {
    return_distance = proximity_distance + math::util::Abs(distance);
  }

  //���W�̏C��
  math::Vector3 return_vector = math::Vector3::kZeroVector;
  if (normal.x > 0) {
    return_vector += math::Vector3(return_distance, 0, 0);
  } else if (normal.y > 0) {
    return_vector += math::Vector3(0, return_distance, 0);
  } else if (normal.z > 0) {
    return_vector += math::Vector3(0, 0, return_distance);
  }
  obb.SetPosition(obb.GetPosition() + return_vector);
  obb.SetOnGround(true);

  return true;
}

//���ƒ����̂̏Փ˔���
bool Collision::Collision_Sphere_OBB(Sphere& sphere, BoundingBox& obb) {
  float left = obb.GetPosition().x - obb.GetLength().x;
  float right = obb.GetPosition().x + obb.GetLength().x;
  float down = obb.GetPosition().y - obb.GetLength().y;
  float up = obb.GetPosition().y + obb.GetLength().y;
  float front = obb.GetPosition().z - obb.GetLength().z;
  float back = obb.GetPosition().z + obb.GetLength().z;

  std::vector<math::Vector3> vertices(8);
  vertices[0] = {left, down, front};
  vertices[1] = {left, down, back};
  vertices[2] = {right, down, back};
  vertices[3] = {right, down, front};
  vertices[4] = {left, up, front};
  vertices[5] = {left, up, back};
  vertices[6] = {right, up, back};
  vertices[7] = {right, up, front};

  math::Matrix4x4 rotate_matrix = obb.GetRotation().ToMatrix();
  for (i32 i = 0; i < vertices.size(); i++) {
    vertices[i] = math::Matrix4x4::MultiplyCoord(vertices[i], rotate_matrix);
  }

  //�e�����Ƌ��̔��a���画��𓾂�
  if (IsCheckLength_Point_Segment(sphere, vertices[0], vertices[1])) {
    return true;
  }
  if (IsCheckLength_Point_Segment(sphere, vertices[1], vertices[2])) {
    return true;
  }
  if (IsCheckLength_Point_Segment(sphere, vertices[2], vertices[3])) {
    return true;
  }
  if (IsCheckLength_Point_Segment(sphere, vertices[3], vertices[0])) {
    return true;
  }
  if (IsCheckLength_Point_Segment(sphere, vertices[4], vertices[5])) {
    return true;
  }
  if (IsCheckLength_Point_Segment(sphere, vertices[5], vertices[6])) {
    return true;
  }
  if (IsCheckLength_Point_Segment(sphere, vertices[6], vertices[7])) {
    return true;
  }
  if (IsCheckLength_Point_Segment(sphere, vertices[7], vertices[4])) {
    return true;
  }
  if (IsCheckLength_Point_Segment(sphere, vertices[0], vertices[4])) {
    return true;
  }
  if (IsCheckLength_Point_Segment(sphere, vertices[1], vertices[5])) {
    return true;
  }
  if (IsCheckLength_Point_Segment(sphere, vertices[2], vertices[6])) {
    return true;
  }
  if (IsCheckLength_Point_Segment(sphere, vertices[3], vertices[7])) {
    return true;
  }

  MY_LOG(L"�Փ˂��܂���ł���");
  return false;
}

//���Ɛ����̋�����r
bool Collision::IsCheckLength_Point_Segment(Sphere& sphere,
                                            math::Vector3 start_position,
                                            math::Vector3 end_position) {
  //�������v�Z
  math::Vector3 v = end_position - start_position;
  float r2 = sphere.GetSquareRadius();

  //���̍��W�Ǝn�_�̋������v�Z
  math::Vector3 sp = sphere.GetPosition() - start_position;
  float t = math::Vector3::Dot(v, sp) / v.Magnitude();

  if ((t < 0) && (sp.MagnitudeSquared() <= r2)) {
    //�n�_���O���ŏՓ�
    MY_LOG(L"�n�_���O���ŏՓ�");
    return true;
  }

  //���̍��W�ƏI�_�̋������v�Z
  math::Vector3 ep = sphere.GetPosition() - end_position;
  if ((1 < t) && (ep.MagnitudeSquared() <= r2)) {
    //�I�_���O���ŏՓ�
    MY_LOG(L"�I�_���O���ŏՓ�");
    return true;
  }

  math::Vector3 h = sp - t * v;
  if ((0 <= t) && (t <= 1) && (h.MagnitudeSquared() <= r2)) {
    //�n�_�ƏI�_�̊ԂŏՓ�
    MY_LOG(L"�n�_�ƏI�_�̊ԂŏՓ�");
    return true;
  }

  return false;
}

//���ƕ��ʂ̏Փ˔���
bool Collision::Collision_Sphere_Plane(Sphere& sphere, Plane& plane) {
  //���ʂƋ��̋������Z�o
  float distance = math::Vector3::Dot(
      sphere.GetPosition() - plane.GetPosition(), plane.GetNormal());

  if (math::util::Abs(distance) <= sphere.GetRadius()) {
    return true;
  }

  MY_LOG(L"�Փ˂��܂���ł���");
  return false;
}

//���ƃJ�v�Z���̏Փ˔���
bool Collision::Collision_Sphere_Capsule(Sphere& sphere, Capsule& capsule) {
  //�������v�Z
  math::Vector3 v = capsule.GetEndPosition() - capsule.GetStartPosition();
  float r = sphere.GetRadius() + capsule.GetRadius();
  float r2 = r * r;

  //���̍��W�Ǝn�_�̋������v�Z
  math::Vector3 sp = sphere.GetPosition() - capsule.GetStartPosition();
  float t = math::Vector3::Dot(v, sp) / v.Magnitude();

  if ((t < 0) && (sp.MagnitudeSquared() <= r2)) {
    //�n�_���O���ŏՓ�
    MY_LOG(L"�n�_���O���ŏՓ�");
    return true;
  }

  //���̍��W�ƏI�_�̋������v�Z
  math::Vector3 ep = sphere.GetPosition() - capsule.GetEndPosition();
  if ((1 < t) && (ep.MagnitudeSquared() <= r2)) {
    //�I�_���O���ŏՓ�
    MY_LOG(L"�I�_���O���ŏՓ�");
    return true;
  }

  math::Vector3 h = sp - t * v;
  if ((0 <= t) && (t <= 1) && (h.MagnitudeSquared() <= r2)) {
    //�n�_�ƏI�_�̊ԂŏՓ�
    MY_LOG(L"�n�_�ƏI�_�̊ԂŏՓ�");
    return true;
  }

  return false;
}

//���ƃJ�v�Z���̏Փ˔���
bool Collision::Collision_Sphere_Capsule(math::Vector3 center, float radius1,
                                         math::Vector3 start_position,
                                         math::Vector3 end_position,
                                         float radius2) {
  //�������v�Z
  math::Vector3 v = end_position - start_position;
  float r = radius1 + radius2;
  float r2 = r * r;

  //���̍��W�Ǝn�_�̋������v�Z
  math::Vector3 sp = center - start_position;
  float t = math::Vector3::Dot(v, sp) / v.Magnitude();

  if ((t < 0) && (sp.MagnitudeSquared() <= r2)) {
    //�n�_���O���ŏՓ�
    MY_LOG(L"�n�_���O���ŏՓ�");
    return true;
  }

  //���̍��W�ƏI�_�̋������v�Z
  math::Vector3 ep = center - end_position;
  if ((1 < t) && (ep.MagnitudeSquared() <= r2)) {
    //�I�_���O���ŏՓ�
    MY_LOG(L"�I�_���O���ŏՓ�");
    return true;
  }

  math::Vector3 h = sp - t * v;
  if ((0 <= t) && (t <= 1) && (h.MagnitudeSquared() <= r2)) {
    //�n�_�ƏI�_�̊ԂŏՓ�
    MY_LOG(L"�n�_�ƏI�_�̊ԂŏՓ�");
    return true;
  }

  return false;
}

//�J�v�Z���ƃJ�v�Z���̏Փ˔���
bool Collision::Collision_Capsule_Capsule(Capsule& capsule1,
                                          Capsule& capsule2) {
  math::Vector3 a = capsule1.GetEndPosition() - capsule1.GetStartPosition();
  math::Vector3 b = capsule2.GetEndPosition() - capsule2.GetStartPosition();
  math::Vector3 c = capsule2.GetStartPosition() - capsule1.GetStartPosition();
  // 2�̃J�v�Z���̔��a�̍��v
  float total_radius = capsule1.GetRadius() + capsule2.GetRadius();

  //�@���x�N�g��
  math::Vector3 normal = math::Vector3::Cross(a, b).Normalized();
  //���S���Ԃ̋���
  float distance = math::Vector3::Dot(c, normal);
  //�J�v�Z��1�̖@���x�N�g��
  math::Vector3 a_normal = math::Vector3::Cross(normal, a).Normalized();
  //�J�v�Z��2�̖@���x�N�g��
  math::Vector3 b_normal = math::Vector3::Cross(normal, b).Normalized();

  math::Vector3 a_plane_point1 =
      capsule2.GetStartPosition() - distance * normal;
  math::Vector3 a_plane_point2 = capsule2.GetEndPosition() - distance * normal;

  //�[�_�܂ł̃x�N�g��
  math::Vector3 v1 = capsule1.GetStartPosition() - capsule2.GetStartPosition();
  math::Vector3 v2 = capsule1.GetEndPosition() - capsule2.GetStartPosition();
  math::Vector3 v3 = capsule2.GetStartPosition() - capsule2.GetStartPosition();
  math::Vector3 v4 = capsule2.GetEndPosition() - capsule1.GetStartPosition();
  float l1 = math::Vector3::Dot(b_normal, v1);
  float l2 = math::Vector3::Dot(b_normal, v2);
  float l3 = math::Vector3::Dot(a_normal, v3);
  float l4 = math::Vector3::Dot(a_normal, v4);

  //��_�܂ł̔�����߂�
  float t1 = l3 / (l3 - l4);
  float t2 = l1 / (l1 - l2);

  // distance��total_radius��菬������ΏՓ˂��Ă���
  //�J�v�Z�����S���ԂŏՓ˂��Ă���Ƃ��̏���
  if ((math::util::Abs(distance) < total_radius) && (0 < t1 && t1 < 1) &&
      (0 < t2 && t2 < 1)) {
    return true;
  }

  //�J�v�Z��1�̎n�_���J�v�Z��2�ɏՓ�
  if (t2 < 0 && Collision_Sphere_Capsule(
                    capsule1.GetStartPosition(), capsule1.GetRadius(),
                    capsule2.GetStartPosition(), capsule2.GetEndPosition(),
                    capsule2.GetRadius())) {
    return true;
  }
  //�J�v�Z��1�̏I�_���J�v�Z��2�ɏՓ�
  if (t2 > 1 && Collision_Sphere_Capsule(
                    capsule1.GetEndPosition(), capsule1.GetRadius(),
                    capsule2.GetStartPosition(), capsule2.GetEndPosition(),
                    capsule2.GetRadius())) {
    return true;
  }
  //�J�v�Z��2�̎n�_���J�v�Z��1�ɏՓ�
  if (t1 < 0 && Collision_Sphere_Capsule(
                    capsule2.GetStartPosition(), capsule2.GetRadius(),
                    capsule1.GetStartPosition(), capsule1.GetEndPosition(),
                    capsule1.GetRadius())) {
    return true;
  }
  //�J�v�Z��1�̏I�_���J�v�Z��1�ɏՓ�
  if (t1 > 1 && Collision_Sphere_Capsule(
                    capsule2.GetEndPosition(), capsule2.GetRadius(),
                    capsule1.GetStartPosition(), capsule1.GetEndPosition(),
                    capsule1.GetRadius())) {
    return true;
  }

  MY_LOG(L"�Փ˂��܂���ł���");
  return false;
}

//�J�v�Z���ƕ��ʂ̏Փ˔���
bool Collision::Collision_Capsule_Plane(Capsule& capsule, Plane& plane) {
  //�J�v�Z���̐����ƕ��ʂ̏Փ˔���
  math::Vector3 v1 = capsule.GetStartPosition() - plane.GetPosition();
  math::Vector3 v2 = capsule.GetEndPosition() - plane.GetPosition();

  if (math::Vector3::Dot(v1, plane.GetNormal()) *
          math::Vector3::Dot(v2, plane.GetNormal()) <=
      0) {
    return true;
  }

  //�J�v�Z���̋��ƕ��ʂ̏Փ˔���

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

//���C�ƒ����̂̋�����r
bool Collision::IsCheckLength_Ray_Obb(Ray& ray, BoundingBox& obb,
                                      math::Vector3 v_sep,
                                      math::Vector3 distance) {
  //��������̋���
  float length = math::util::Abs(math::Vector3::Dot(v_sep, distance));

  //��������ōł�����obb�̒��_�܂ł̋���
  float len_a = math::util::Abs(
      math::Vector3::Dot(obb.GetAxisX(), v_sep) * obb.GetLength().z +
      math::Vector3::Dot(obb.GetAxisY(), v_sep) * obb.GetLength().y +
      math::Vector3::Dot(obb.GetAxisZ(), v_sep) * obb.GetLength().z);

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

  if (a1 > ray.GetDistance()) {
    MY_LOG(L"���C�͈̔͊O�ł���");
    return false;
  }

  return true;
}
}  // namespace physics
}  // namespace legend