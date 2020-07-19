#ifndef LEGEND_PHYSICS_COLLISION_H_
#define LEGEND_PHYSICS_COLLISION_H_

#include "src/physics/bounding_box.h"
#include "src/physics/plane.h"
#include "src/util/singleton.h"

namespace legend {
namespace physics {
/**
 * @class Collision
 * @brief ‚ ‚½‚è”»’è‚ğ’²‚×‚é
 */
class Collision : public util::Singleton<Collision> {
 public:
  /**
   * @brief ’¼•û‘Ì“¯m‚ÌÕ“Ë”»’è
   * @param ’¼•û‘Ì1
   * @param ’¼•û‘Ì2
   */
  bool Collision_OBB_OBB(BoundingBox& obb1, BoundingBox& obb2);
  /**
   * @brief ’¼•û‘Ì‚Æ•½–Ê‚ÌÕ“Ë”»’è
   * @param ’¼•û‘Ì
   * @param •½–Ê
   */
  bool Collision_OBB_Plane(BoundingBox& obb, Plane& plane);
  /**
  * @brief OBB‚Ì“Š‰e‹——£”äŠr
  * @param ’¼•û‘Ì1
  * @param ’¼•û‘Ì2
  * @param •ª—£²
  * @param ‹——£
  */
  bool IsCompareLengthOBB(BoundingBox& obb1, BoundingBox& obb2,
                          math::Vector3 vSep, math::Vector3 distance);
};
}  // namespace physics
}  // namespace legend

#endif  //! LEGEND_PHYSICS_COLLISION_H_
