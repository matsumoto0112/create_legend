#include "src/physics/collision.h"

#include "src/math/quaternion.h"

namespace legend {
namespace physics {

//直方体同士の衝突判定
bool Collision::Collision_OBB_OBB(BoundingBox& obb1, BoundingBox& obb2) {
  //分離軸の更新
  obb1.SetAxis();
  obb2.SetAxis();
  //距離を算出
  math::Vector3 distance = obb2.GetPosition() - obb1.GetPosition();

  //分離軸の比較
  if (!IsCompareLengthOBB(obb1, obb2, obb1.GetAxisX(), distance)) {
    MY_LOG(L"衝突しませんでした");
    return false;
  }
  if (!IsCompareLengthOBB(obb1, obb2, obb1.GetAxisY(), distance)) {
    MY_LOG(L"衝突しませんでした");
    return false;
  }
  if (!IsCompareLengthOBB(obb1, obb2, obb1.GetAxisZ(), distance)) {
    MY_LOG(L"衝突しませんでした");
    return false;
  }
  if (!IsCompareLengthOBB(obb1, obb2, obb2.GetAxisX(), distance)) {
    MY_LOG(L"衝突しませんでした");
    return false;
  }
  if (!IsCompareLengthOBB(obb1, obb2, obb2.GetAxisY(), distance)) {
    MY_LOG(L"衝突しませんでした");
    return false;
  }
  if (!IsCompareLengthOBB(obb1, obb2, obb2.GetAxisZ(), distance)) {
    MY_LOG(L"衝突しませんでした");
    return false;
  }

  //分離軸同士の外積を比較
  math::Vector3 vSep;
  vSep = math::Vector3::Cross(obb1.GetAxisX(), obb2.GetAxisX());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"衝突しませんでした");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisX(), obb2.GetAxisY());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"衝突しませんでした");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisX(), obb2.GetAxisZ());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"衝突しませんでした");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisY(), obb2.GetAxisX());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"衝突しませんでした");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisY(), obb2.GetAxisY());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"衝突しませんでした");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisY(), obb2.GetAxisZ());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"衝突しませんでした");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisZ(), obb2.GetAxisX());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"衝突しませんでした");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisZ(), obb2.GetAxisY());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"衝突しませんでした");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisZ(), obb2.GetAxisZ());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"衝突しませんでした");
    return false;
  }

  return true;
}

//直方体と平面の衝突判定
bool Collision::Collision_OBB_Plane(BoundingBox& obb, Plane& plane) {
  //平面の法線に対するOBBの射影線の長さを算出
  //近接距離
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

  //平面とOBBの距離を算出
  float distance = math::Vector3::Dot(obb.GetPosition() - plane.GetPosition(),
                                      plane.GetNormal());

  if (math::util::Abs(distance) - proximity_distance > 0) {
    MY_LOG(L"衝突しませんでした");
    return false;
  }

  //戻し距離
  float return_distance = 0;
  if (distance > 0) {
    return_distance = proximity_distance - fabs(distance);
  } else {
    return_distance = proximity_distance + fabs(distance);
  }

  //座標の修正
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

//球と平面の衝突判定
bool Collision::Collision_Sphere_Plane(Sphere& sphere, Plane& plane) {
  //平面と球の距離を算出
  float distance = math::Vector3::Dot(
      sphere.GetPosition() - plane.GetPosition(), plane.GetNormal());

  if (math::util::Abs(distance) > sphere.GetRadius()) {
    MY_LOG(L"衝突しませんでした");
    return false;
  }

  return true;
}

// OBBの投影距離比較
bool Collision::IsCompareLengthOBB(BoundingBox& obb1, BoundingBox& obb2,
                                   math::Vector3 vSep, math::Vector3 distance) {
  //分離軸上の距離
  float length = math::util::Abs(math::Vector3::Dot(vSep, distance));

  //分離軸上で最も遠いobb1の頂点までの距離
  float len_a = math::util::Abs(
      math::Vector3::Dot(obb1.GetAxisX(), vSep) * obb1.GetLength(0) +
      math::Vector3::Dot(obb1.GetAxisY(), vSep) * obb1.GetLength(1) +
      math::Vector3::Dot(obb1.GetAxisZ(), vSep) * obb1.GetLength(2));

  //分離軸上で最も遠いobb2の頂点までの距離
  float len_b = math::util::Abs(
      math::Vector3::Dot(obb2.GetAxisX(), vSep) * obb2.GetLength(0) +
      math::Vector3::Dot(obb2.GetAxisY(), vSep) * obb2.GetLength(1) +
      math::Vector3::Dot(obb2.GetAxisZ(), vSep) * obb2.GetLength(2));

  if (length > len_a + len_b) {
    //衝突していない
    return false;
  }

  //衝突している
  return true;
}
}  // namespace physics
}  // namespace legend