#include "src/physics/collision.h"

#include "src/math/quaternion.h"

namespace legend {
namespace physics {

//’¼•û‘Ì“¯m‚ÌÕ“Ë”»’è
bool Collision::Collision_OBB_OBB(BoundingBox& obb1, BoundingBox& obb2) {
  //‰ñ“]s—ñ‚Ìæ“¾
  math::Matrix4x4 rotate_matrix1 =
      math::Matrix4x4::CreateRotation(obb1.GetRotation());
  math::Matrix4x4 rotate_matrix2 =
      math::Matrix4x4::CreateRotation(obb2.GetRotation());

  //Še•ûŒüƒxƒNƒgƒ‹‚ÌŠm•Û
  math::Vector3 n_ae1 =
      math::Matrix4x4::MultiplyCoord(obb1.GetDirection(0), rotate_matrix1);
  math::Vector3 ae1 = n_ae1 * obb1.GetLength(0) * obb1.GetScale().x;
  math::Vector3 n_ae2 =
      math::Matrix4x4::MultiplyCoord(obb1.GetDirection(1), rotate_matrix1);
  math::Vector3 ae2 = n_ae2 * obb1.GetLength(1) * obb1.GetScale().y;
  math::Vector3 n_ae3 =
      math::Matrix4x4::MultiplyCoord(obb1.GetDirection(2), rotate_matrix1);
  math::Vector3 ae3 = n_ae3 * obb1.GetLength(2) * obb1.GetScale().z;

  math::Vector3 n_be1 =
      math::Matrix4x4::MultiplyCoord(obb2.GetDirection(0), rotate_matrix2);
  math::Vector3 be1 = n_be1 * obb2.GetLength(0) * obb2.GetScale().x;
  math::Vector3 n_be2 =
      math::Matrix4x4::MultiplyCoord(obb2.GetDirection(1), rotate_matrix2);
  math::Vector3 be2 = n_be2 * obb2.GetLength(1) * obb2.GetScale().y;
  math::Vector3 n_be3 =
      math::Matrix4x4::MultiplyCoord(obb2.GetDirection(2), rotate_matrix2);
  math::Vector3 be3 = n_be3 * obb2.GetLength(2) * obb2.GetScale().z;

  math::Vector3 distance = obb1.GetPosition() - obb2.GetPosition();

  //•ª—£²
  float projection_line_a, projection_line_b, length;
  math::Vector3 cross;

  // ae1
  projection_line_a = ae1.MagnitudeSquared();
  projection_line_b = GetSeparateAxis(n_ae1, be1, be2, be3);
  length = fabs(math::Vector3::Dot(distance, n_ae1));
  if (length > projection_line_a + projection_line_b) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }

  // ae2
  projection_line_a = ae2.MagnitudeSquared();
  projection_line_b = GetSeparateAxis(n_ae2, be1, be2, be3);
  length = fabs(math::Vector3::Dot(distance, n_ae2));
  if (length > projection_line_a + projection_line_b) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }

  // ae3
  projection_line_a = ae3.MagnitudeSquared();
  projection_line_b = GetSeparateAxis(n_ae3, be1, be2, be3);
  length = fabs(math::Vector3::Dot(distance, n_ae3));
  if (length > projection_line_a + projection_line_b) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }

  // be1
  projection_line_a = be1.MagnitudeSquared();
  projection_line_b = GetSeparateAxis(n_be1, ae1, ae2, ae3);
  length = fabs(math::Vector3::Dot(distance, n_be1));
  if (length > projection_line_a + projection_line_b) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }

  // be2
  projection_line_a = be2.MagnitudeSquared();
  projection_line_b = GetSeparateAxis(n_be2, ae1, ae2, ae3);
  length = fabs(math::Vector3::Dot(distance, n_be2));
  if (length > projection_line_a + projection_line_b) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }

  // be3
  projection_line_a = be3.MagnitudeSquared();
  projection_line_b = GetSeparateAxis(n_be3, ae1, ae2, ae3);
  length = fabs(math::Vector3::Dot(distance, n_be3));
  if (length > projection_line_a + projection_line_b) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }

  // c11
  cross = math::Vector3::Cross(n_ae1, n_be1);
  projection_line_a = GetSeparateAxis(cross, ae2, ae3);
  projection_line_b = GetSeparateAxis(cross, be2, be3);
  length = fabs(math::Vector3::Dot(distance, cross));
  if (length > projection_line_a + projection_line_b) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }

  // c12
  cross = math::Vector3::Cross(n_ae1, n_be2);
  projection_line_a = GetSeparateAxis(cross, ae2, ae3);
  projection_line_b = GetSeparateAxis(cross, be1, be3);
  length = fabs(math::Vector3::Dot(distance, cross));
  if (length > projection_line_a + projection_line_b) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }

  // c13
  cross = math::Vector3::Cross(n_ae1, n_be3);
  projection_line_a = GetSeparateAxis(cross, ae2, ae3);
  projection_line_b = GetSeparateAxis(cross, be1, be2);
  length = fabs(math::Vector3::Dot(distance, cross));
  if (length > projection_line_a + projection_line_b) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }

  // c21
  cross = math::Vector3::Cross(n_ae2, n_be1);
  projection_line_a = GetSeparateAxis(cross, ae1, ae3);
  projection_line_b = GetSeparateAxis(cross, be2, be3);
  length = fabs(math::Vector3::Dot(distance, cross));
  if (length > projection_line_a + projection_line_b) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }

  // c22
  cross = math::Vector3::Cross(n_ae2, n_be2);
  projection_line_a = GetSeparateAxis(cross, ae1, ae3);
  projection_line_b = GetSeparateAxis(cross, be1, be3);
  length = fabs(math::Vector3::Dot(distance, cross));
  if (length > projection_line_a + projection_line_b) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }

  // c23
  cross = math::Vector3::Cross(n_ae2, n_be3);
  projection_line_a = GetSeparateAxis(cross, ae1, ae3);
  projection_line_b = GetSeparateAxis(cross, be1, be2);
  length = fabs(math::Vector3::Dot(distance, cross));
  if (length > projection_line_a + projection_line_b) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }

  // c31
  cross = math::Vector3::Cross(n_ae3, n_be1);
  projection_line_a = GetSeparateAxis(cross, ae1, ae2);
  projection_line_b = GetSeparateAxis(cross, be2, be3);
  length = fabs(math::Vector3::Dot(distance, cross));
  if (length > projection_line_a + projection_line_b) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }

  // c32
  cross = math::Vector3::Cross(n_ae3, n_be2);
  projection_line_a = GetSeparateAxis(cross, ae1, ae2);
  projection_line_b = GetSeparateAxis(cross, be1, be3);
  length = fabs(math::Vector3::Dot(distance, cross));
  if (length > projection_line_a + projection_line_b) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }

  // c33
  cross = math::Vector3::Cross(n_ae3, n_be3);
  projection_line_a = GetSeparateAxis(cross, ae1, ae2);
  projection_line_b = GetSeparateAxis(cross, be1, be2);
  length = fabs(math::Vector3::Dot(distance, cross));
  if (length > projection_line_a + projection_line_b) {
    MY_LOG(L"Õ“Ë‚µ‚Ü‚¹‚ñ‚Å‚µ‚½");
    return false;
  }

  return true;
}

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

  if (fabs(distance) - proximity_distance >= 0) {
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

  return true;
}

//•ª—£²‚É“Š‰e‚³‚ê‚½²¬•ª‚©‚ç“Š‰eü•ª’·‚ğZo
float Collision::GetSeparateAxis(math::Vector3 sep, math::Vector3 x,
                                 math::Vector3 y, math::Vector3 z) {
  float r1 = fabs(math::Vector3::Dot(sep, x));
  float r2 = fabs(math::Vector3::Dot(sep, y));
  float r3 = 0;
  if (z != math::Vector3::kZeroVector) {
    r3 = fabs(math::Vector3::Dot(sep, z));
  }

  return r1 + r2 + r3;
}
}  // namespace physics
}  // namespace legend