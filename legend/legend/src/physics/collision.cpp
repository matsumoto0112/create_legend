#include "src/physics/collision.h"

#include "src/math/quaternion.h"

namespace legend {
namespace physics {

//íºï˚ëÃìØémÇÃè’ìÀîªíË
bool Collision::Collision_OBB_OBB(BoundingBox& obb1, BoundingBox& obb2) {
  //ï™ó£é≤ÇÃçXêV
  obb1.SetAxis();
  obb2.SetAxis();
  //ãóó£ÇéZèo
  math::Vector3 distance = obb2.GetPosition() - obb1.GetPosition();

  //ï™ó£é≤ÇÃî‰är
  if (!IsCompareLengthOBB(obb1, obb2, obb1.GetAxisX(), distance)) {
    MY_LOG(L"è’ìÀÇµÇ‹ÇπÇÒÇ≈ÇµÇΩ");
    return false;
  }
  if (!IsCompareLengthOBB(obb1, obb2, obb1.GetAxisY(), distance)) {
    MY_LOG(L"è’ìÀÇµÇ‹ÇπÇÒÇ≈ÇµÇΩ");
    return false;
  }
  if (!IsCompareLengthOBB(obb1, obb2, obb1.GetAxisZ(), distance)) {
    MY_LOG(L"è’ìÀÇµÇ‹ÇπÇÒÇ≈ÇµÇΩ");
    return false;
  }
  if (!IsCompareLengthOBB(obb1, obb2, obb2.GetAxisX(), distance)) {
    MY_LOG(L"è’ìÀÇµÇ‹ÇπÇÒÇ≈ÇµÇΩ");
    return false;
  }
  if (!IsCompareLengthOBB(obb1, obb2, obb2.GetAxisY(), distance)) {
    MY_LOG(L"è’ìÀÇµÇ‹ÇπÇÒÇ≈ÇµÇΩ");
    return false;
  }
  if (!IsCompareLengthOBB(obb1, obb2, obb2.GetAxisZ(), distance)) {
    MY_LOG(L"è’ìÀÇµÇ‹ÇπÇÒÇ≈ÇµÇΩ");
    return false;
  }

  //ï™ó£é≤ìØémÇÃäOêœÇî‰är
  math::Vector3 vSep;
  vSep = math::Vector3::Cross(obb1.GetAxisX(), obb2.GetAxisX());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"è’ìÀÇµÇ‹ÇπÇÒÇ≈ÇµÇΩ");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisX(), obb2.GetAxisY());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"è’ìÀÇµÇ‹ÇπÇÒÇ≈ÇµÇΩ");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisX(), obb2.GetAxisZ());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"è’ìÀÇµÇ‹ÇπÇÒÇ≈ÇµÇΩ");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisY(), obb2.GetAxisX());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"è’ìÀÇµÇ‹ÇπÇÒÇ≈ÇµÇΩ");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisY(), obb2.GetAxisY());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"è’ìÀÇµÇ‹ÇπÇÒÇ≈ÇµÇΩ");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisY(), obb2.GetAxisZ());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"è’ìÀÇµÇ‹ÇπÇÒÇ≈ÇµÇΩ");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisZ(), obb2.GetAxisX());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"è’ìÀÇµÇ‹ÇπÇÒÇ≈ÇµÇΩ");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisZ(), obb2.GetAxisY());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"è’ìÀÇµÇ‹ÇπÇÒÇ≈ÇµÇΩ");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisZ(), obb2.GetAxisZ());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"è’ìÀÇµÇ‹ÇπÇÒÇ≈ÇµÇΩ");
    return false;
  }

  return true;
}

bool Collision::Collision_OBB_Plane(BoundingBox& obb, Plane& plane) {
  //ïΩñ ÇÃñ@ê¸Ç…ëŒÇ∑ÇÈOBBÇÃéÀâeê¸ÇÃí∑Ç≥ÇéZèo
  //ãﬂê⁄ãóó£
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

  //ïΩñ Ç∆OBBÇÃãóó£ÇéZèo
  float distance = math::Vector3::Dot(obb.GetPosition() - plane.GetPosition(),
                                      plane.GetNormal());

  if (fabs(distance) - proximity_distance >= 0) {
    MY_LOG(L"è’ìÀÇµÇ‹ÇπÇÒÇ≈ÇµÇΩ");
    return false;
  }

  ////ñﬂÇµãóó£
  // float return_distance = 0;
  // if (distance > 0) {
  //  return_distance = proximity_distance - fabs(distance);
  //} else {
  //  return_distance = proximity_distance + fabs(distance);
  //}

  return true;
}

// OBBÇÃìäâeãóó£î‰är
bool Collision::IsCompareLengthOBB(BoundingBox& obb1, BoundingBox& obb2,
                                   math::Vector3 vSep, math::Vector3 distance) {
  //ï™ó£é≤è„ÇÃãóó£
  float length = fabsf(math::Vector3::Dot(vSep, distance));

  //ï™ó£é≤è„Ç≈ç≈Ç‡âìÇ¢obb1ÇÃí∏ì_Ç‹Ç≈ÇÃãóó£
  float len_a =
      fabsf(math::Vector3::Dot(obb1.GetAxisX(), vSep) * obb1.GetLength(0) +
            math::Vector3::Dot(obb1.GetAxisY(), vSep) * obb1.GetLength(1) +
            math::Vector3::Dot(obb1.GetAxisZ(), vSep) * obb1.GetLength(2));

  //ï™ó£é≤è„Ç≈ç≈Ç‡âìÇ¢obb2ÇÃí∏ì_Ç‹Ç≈ÇÃãóó£
  float len_b =
      fabsf(math::Vector3::Dot(obb2.GetAxisX(), vSep) * obb2.GetLength(0) +
            math::Vector3::Dot(obb2.GetAxisY(), vSep) * obb2.GetLength(1) +
            math::Vector3::Dot(obb2.GetAxisZ(), vSep) * obb2.GetLength(2));

  if (length > len_a + len_b) {
    //è’ìÀÇµÇƒÇ¢Ç»Ç¢
    return false;
  }

  //è’ìÀÇµÇƒÇ¢ÇÈ
  return true;
}
}  // namespace physics
}  // namespace legend