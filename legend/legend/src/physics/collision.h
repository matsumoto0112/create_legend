#ifndef LEGEND_PHYSICS_COLLISION_H_
#define LEGEND_PHYSICS_COLLISION_H_

#include "src/physics/bounding_box.h"
#include "src/physics/plane.h"
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
   * @brief 直方体と平面の衝突判定
   * @param 直方体
   * @param 平面
   */
  bool Collision_OBB_Plane(BoundingBox& obb, Plane& plane);
  /**
   * @brief 分離軸に投影された軸成分から投影線分長を算出
   * @param 分離軸
   * @param x軸
   * @param y軸
   * @param z軸
   */
  float GetSeparateAxis(math::Vector3 sep, math::Vector3 x, math::Vector3 y,
                        math::Vector3 z = math::Vector3::kZeroVector);
};
}  // namespace physics
}  // namespace legend

#endif  //! LEGEND_PHYSICS_COLLISION_H_
