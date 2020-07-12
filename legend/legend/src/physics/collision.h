#ifndef LEGEND_PHYSICS_COLLISION_H_
#define LEGEND_PHYSICS_COLLISION_H_

#include "src/physics/bounding_box.h"
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
   * @brief •ª—£²‚É“Š‰e‚³‚ê‚½²¬•ª‚©‚ç“Š‰eü•ª’·‚ğZo
   * @param •ª—£²
   * @param x²
   * @param y²
   * @param z²
   */
  float GetSeparateAxis(math::Vector3 sep, math::Vector3 x, math::Vector3 y,
                        math::Vector3 z = math::Vector3::kZeroVector);
};
}  // namespace physics
}  // namespace legend

#endif  //! LEGEND_PHYSICS_COLLISION_H_
