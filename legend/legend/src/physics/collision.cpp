#include "src/physics/collision.h"

namespace legend {
namespace physics {

//直方体同士の衝突判定
bool Collision::Collision_OBB_OBB(BoundingBox& obb1, BoundingBox& obb2) {
  //各方向ベクトルの確保
  math::Vector3 n_ae1 = obb1.GetDirection(0);
  math::Vector3 ae1 = n_ae1 * obb1.GetLength(0);
  math::Vector3 n_ae2 = obb1.GetDirection(1);
  math::Vector3 ae2 = n_ae2 * obb1.GetLength(1);
  math::Vector3 n_ae3 = obb1.GetDirection(2);
  math::Vector3 ae3 = n_ae3 * obb1.GetLength(2);

  math::Vector3 n_be1 = obb2.GetDirection(0);
  math::Vector3 be1 = n_be1 * obb2.GetLength(0);
  math::Vector3 n_be2 = obb2.GetDirection(1);
  math::Vector3 be2 = n_be2 * obb2.GetLength(1);
  math::Vector3 n_be3 = obb2.GetDirection(2);
  math::Vector3 be3 = n_be3 * obb2.GetLength(2);

  math::Vector3 distance = obb1.GetPosition() - obb2.GetPosition();

  //分離軸
  float projection_line_a, projection_line_b, length;
  math::Vector3 cross;

  // ae1
  projection_line_a = ae1.MagnitudeSquared();
  projection_line_b = GetSeparateAxis(n_ae1, be1, be2, be3);
  length = fabs(math::Vector3::Dot(distance, n_ae1));
  if (length > projection_line_a + projection_line_b) {
      MY_LOG(L"衝突しませんでした");
      return false;
  }

  // ae2
  projection_line_a = ae2.MagnitudeSquared();
  projection_line_b = GetSeparateAxis(n_ae2, be1, be2, be3);
  length = fabs(math::Vector3::Dot(distance, n_ae2));
  if (length > projection_line_a + projection_line_b) {
      MY_LOG(L"衝突しませんでした");
      return false;
  }

  // ae3
  projection_line_a = ae3.MagnitudeSquared();
  projection_line_b = GetSeparateAxis(n_ae3, be1, be2, be3);
  length = fabs(math::Vector3::Dot(distance, n_ae3));
  if (length > projection_line_a + projection_line_b) {
      MY_LOG(L"衝突しませんでした");
      return false;
  }

  // be1
  projection_line_a = be1.MagnitudeSquared();
  projection_line_b = GetSeparateAxis(n_be1, ae1, ae2, ae3);
  length = fabs(math::Vector3::Dot(distance, n_be1));
  if (length > projection_line_a + projection_line_b) {
      MY_LOG(L"衝突しませんでした");
      return false;
  }

  // be2
  projection_line_a = be2.MagnitudeSquared();
  projection_line_b = GetSeparateAxis(n_be2, ae1, ae2, ae3);
  length = fabs(math::Vector3::Dot(distance, n_be2));
  if (length > projection_line_a + projection_line_b) {
      MY_LOG(L"衝突しませんでした");
      return false;
  }

  // be3
  projection_line_a = be3.MagnitudeSquared();
  projection_line_b = GetSeparateAxis(n_be3, ae1, ae2, ae3);
  length = fabs(math::Vector3::Dot(distance, n_be3));
  if (length > projection_line_a + projection_line_b) {
      MY_LOG(L"衝突しませんでした");
      return false;
  }

  // c11
  cross = math::Vector3::Cross(n_ae1, n_be1);
  projection_line_a = GetSeparateAxis(cross, ae2, ae3);
  projection_line_b = GetSeparateAxis(cross, be2, be3);
  length = fabs(math::Vector3::Dot(distance, cross));
  if (length > projection_line_a + projection_line_b) {
      MY_LOG(L"衝突しませんでした");
      return false;
  }

  // c12
  cross = math::Vector3::Cross(n_ae1, n_be2);
  projection_line_a = GetSeparateAxis(cross, ae2, ae3);
  projection_line_b = GetSeparateAxis(cross, be1, be3);
  length = fabs(math::Vector3::Dot(distance, cross));
  if (length > projection_line_a + projection_line_b) {
      MY_LOG(L"衝突しませんでした");
      return false;
  }

  // c13
  cross = math::Vector3::Cross(n_ae1, n_be3);
  projection_line_a = GetSeparateAxis(cross, ae2, ae3);
  projection_line_b = GetSeparateAxis(cross, be1, be2);
  length = fabs(math::Vector3::Dot(distance, cross));
  if (length > projection_line_a + projection_line_b) {
      MY_LOG(L"衝突しませんでした");
      return false;
  }

  // c21
  cross = math::Vector3::Cross(n_ae2, n_be1);
  projection_line_a = GetSeparateAxis(cross, ae1, ae3);
  projection_line_b = GetSeparateAxis(cross, be2, be3);
  length = fabs(math::Vector3::Dot(distance, cross));
  if (length > projection_line_a + projection_line_b) {
      MY_LOG(L"衝突しませんでした");
      return false;
  }

  // c22
  cross = math::Vector3::Cross(n_ae2, n_be2);
  projection_line_a = GetSeparateAxis(cross, ae1, ae3);
  projection_line_b = GetSeparateAxis(cross, be1, be3);
  length = fabs(math::Vector3::Dot(distance, cross));
  if (length > projection_line_a + projection_line_b) {
      MY_LOG(L"衝突しませんでした");
      return false;
  }

  // c23
  cross = math::Vector3::Cross(n_ae2, n_be3);
  projection_line_a = GetSeparateAxis(cross, ae1, ae3);
  projection_line_b = GetSeparateAxis(cross, be1, be2);
  length = fabs(math::Vector3::Dot(distance, cross));
  if (length > projection_line_a + projection_line_b) {
      MY_LOG(L"衝突しませんでした");
      return false;
  }

  // c31
  cross = math::Vector3::Cross(n_ae3, n_be1);
  projection_line_a = GetSeparateAxis(cross, ae1, ae2);
  projection_line_b = GetSeparateAxis(cross, be2, be3);
  length = fabs(math::Vector3::Dot(distance, cross));
  if (length > projection_line_a + projection_line_b) {
      MY_LOG(L"衝突しませんでした");
      return false;
  }

  // c32
  cross = math::Vector3::Cross(n_ae3, n_be2);
  projection_line_a = GetSeparateAxis(cross, ae1, ae2);
  projection_line_b = GetSeparateAxis(cross, be1, be3);
  length = fabs(math::Vector3::Dot(distance, cross));
  if (length > projection_line_a + projection_line_b) {
      MY_LOG(L"衝突しませんでした");
      return false;
  }

  // c33
  cross = math::Vector3::Cross(n_ae3, n_be3);
  projection_line_a = GetSeparateAxis(cross, ae1, ae2);
  projection_line_b = GetSeparateAxis(cross, be1, be2);
  length = fabs(math::Vector3::Dot(distance, cross));
  if (length > projection_line_a + projection_line_b) {
      MY_LOG(L"衝突しませんでした");
    return false;
  }

  //衝突している
  MY_LOG(L"衝突しました");
  return true;
}

//分離軸に投影された軸成分から投影線分長を算出
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