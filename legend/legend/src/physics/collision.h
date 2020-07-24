#ifndef LEGEND_PHYSICS_COLLISION_H_
#define LEGEND_PHYSICS_COLLISION_H_

#include "src/physics/bounding_box.h"
#include "src/physics/plane.h"
#include "src/physics/ray.h"
#include "src/physics/sphere.h"
#include "src/util/singleton.h"

namespace legend {
namespace physics {
/**
 * @class Collision
 * @brief あたり判定を調べる
 */
class Collision : public util::Singleton<Collision> {
 public:
  /**
   * @brief 直方体同士の衝突判定
   * @param 直方体1
   * @param 直方体2
   */
  bool Collision_OBB_OBB(BoundingBox& obb1, BoundingBox& obb2);
  /**
   * @brief OBBの投影距離比較
   * @param 直方体1
   * @param 直方体2
   * @param 分離軸
   * @param 距離
   */
  bool IsCompareLengthOBB(BoundingBox& obb1, BoundingBox& obb2,
                          math::Vector3 v_sep, math::Vector3 distance);
  /**
   * @brief 直方体と平面の衝突判定
   * @param 直方体
   * @param 平面
   */
  bool Collision_OBB_Plane(BoundingBox& obb, Plane& plane);
  /**
   * @brief 球と直方体の衝突判定
   * @param 球
   * @param 直方体
   */
  bool Collision_Sphere_OBB(Sphere& sphere, BoundingBox& obb);
  /**
   * @brief 球と線分の距離比較
   * @param 球
   * @param 始点
   * @param 終点
   */
  bool IsCheckLength_Point_Segment(Sphere& sphere, math::Vector3 start_position,
                                math::Vector3 end_position);
  /**
   * @brief 球と平面の衝突判定
   * @param 球
   * @param 平面
   */
  bool Collision_Sphere_Plane(Sphere& sphere, Plane& plane);
  /**
   * @brief レイの長さを調べる
   * @param レイ
   */
  float GetRayLength(Ray& ray) const;
  /**
   * @brief レイと直方体の衝突判定
   * @param レイ
   * @param 直方体
   */
  bool Collision_Ray_OBB(Ray& ray, BoundingBox& obb);
  /**
   * @brief レイと直方体の距離比較
   * @param レイ
   * @param 直方体
   * @param 分離軸
   * @param 距離
   */
  bool IsCheckLength_Ray_Obb(Ray& ray, BoundingBox& obb, math::Vector3 v_sep,
                             math::Vector3 distance);
  /**
   * @brief レイと平面の衝突判定
   * @param レイ
   * @param 平面
   */
  bool Collision_Ray_Plane(Ray& ray, Plane& plane);
  /**
   * @brief レイと球の衝突判定
   * @param レイ
   * @param 球
   */
  bool Collision_Ray_Sphere(Ray& ray, Sphere& sphere);
};
}  // namespace physics
}  // namespace legend

#endif  //! LEGEND_PHYSICS_COLLISION_H_
