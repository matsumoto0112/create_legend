#include "src/physics/collision.h"

#include "src/math/quaternion.h"

namespace legend {
namespace physics {

//’¼•û‘Ì“¯m‚ÌÕ“Ë”»’è
bool Collision::Collision_OBB_OBB(BoundingBox& obb1, BoundingBox& obb2) {
  //•ª—£²‚ÌXV
  obb1.SetAxis();
  obb2.SetAxis();
  //‹——£‚ğZo
  math::Vector3 distance = obb2.GetPosition() - obb1.GetPosition();

  //•ª—£²‚Ì”äŠr
  if (!IsCompareLengthOBB(obb1, obb2, obb1.GetAxisX(), distance)) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }
  if (!IsCompareLengthOBB(obb1, obb2, obb1.GetAxisY(), distance)) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }
  if (!IsCompareLengthOBB(obb1, obb2, obb1.GetAxisZ(), distance)) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }
  if (!IsCompareLengthOBB(obb1, obb2, obb2.GetAxisX(), distance)) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }
  if (!IsCompareLengthOBB(obb1, obb2, obb2.GetAxisY(), distance)) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }
  if (!IsCompareLengthOBB(obb1, obb2, obb2.GetAxisZ(), distance)) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }

  //•ª—£²“¯m‚ÌŠOÏ‚ğ”äŠr
  math::Vector3 vSep;
  vSep = math::Vector3::Cross(obb1.GetAxisX(), obb2.GetAxisX());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisX(), obb2.GetAxisY());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisX(), obb2.GetAxisZ());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisY(), obb2.GetAxisX());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisY(), obb2.GetAxisY());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisY(), obb2.GetAxisZ());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisZ(), obb2.GetAxisX());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisZ(), obb2.GetAxisY());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }
  vSep = math::Vector3::Cross(obb1.GetAxisZ(), obb2.GetAxisZ());
  if (!IsCompareLengthOBB(obb1, obb2, vSep, distance)) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }

  return true;
}

//’¼•û‘Ì‚Æ•½–Ê‚ÌÕ“Ë”»’è
bool Collision::Collision_OBB_Plane(BoundingBox& obb, Plane& plane) {
  //•½–Ê‚Ì–@ü‚É‘Î‚·‚éOBB‚ÌË‰eü‚Ì’·‚³‚ğZo
  //‹ßÚ‹——£
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

  //•½–Ê‚ÆOBB‚Ì‹——£‚ğZo
  float distance = math::Vector3::Dot(obb.GetPosition() - plane.GetPosition(),
                                      plane.GetNormal());

  if (math::util::Abs(distance) - proximity_distance > 0) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }

  //–ß‚µ‹——£
  float return_distance = 0;
  if (distance > 0) {
    return_distance = proximity_distance - fabs(distance);
  } else {
    return_distance = proximity_distance + fabs(distance);
  }

  //À•W‚ÌC³
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

//‹…‚Æ•½–Ê‚ÌÕ“Ë”»’è
bool Collision::Collision_Sphere_Plane(Sphere& sphere, Plane& plane) {
  //•½–Ê‚Æ‹…‚Ì‹——£‚ğZo
  float distance = math::Vector3::Dot(
      sphere.GetPosition() - plane.GetPosition(), plane.GetNormal());

  if (math::util::Abs(distance) > sphere.GetRadius()) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }

  return true;
}

// OBB‚Ì“Š‰e‹——£”äŠr
bool Collision::IsCompareLengthOBB(BoundingBox& obb1, BoundingBox& obb2,
                                   math::Vector3 vSep, math::Vector3 distance) {
  //•ª—£²ã‚Ì‹——£
  float length = math::util::Abs(math::Vector3::Dot(vSep, distance));

  //•ª—£²ã‚ÅÅ‚à‰“‚¢obb1‚Ì’¸“_‚Ü‚Å‚Ì‹——£
  float len_a = math::util::Abs(
      math::Vector3::Dot(obb1.GetAxisX(), vSep) * obb1.GetLength(0) +
      math::Vector3::Dot(obb1.GetAxisY(), vSep) * obb1.GetLength(1) +
      math::Vector3::Dot(obb1.GetAxisZ(), vSep) * obb1.GetLength(2));

  //•ª—£²ã‚ÅÅ‚à‰“‚¢obb2‚Ì’¸“_‚Ü‚Å‚Ì‹——£
  float len_b = math::util::Abs(
      math::Vector3::Dot(obb2.GetAxisX(), vSep) * obb2.GetLength(0) +
      math::Vector3::Dot(obb2.GetAxisY(), vSep) * obb2.GetLength(1) +
      math::Vector3::Dot(obb2.GetAxisZ(), vSep) * obb2.GetLength(2));

  if (length > len_a + len_b) {
    //Õ“Ë‚µ‚Ä‚¢‚È‚¢
    return false;
  }

  //Õ“Ë‚µ‚Ä‚¢‚é
  return true;
}
}  // namespace physics
}  // namespace legend