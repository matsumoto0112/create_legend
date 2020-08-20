#ifndef LEGEND_BULLET_BULLET_HELPER_H
#define LEGEND_BULLET_BULLET_HELPER_H

/**
 * @file bullet_helper.h
 * @brief
 */

#include <btBulletDynamicsCommon.h>

#include "src/math/quaternion.h"
#include "src/math/vector_3.h"

namespace legend {
namespace bullet {
namespace helper {
inline math::Vector3 ToVector3(const btVector3& v) {
  return math::Vector3(v.x(), v.y(), v.z());
}

inline btVector3 TobtVector3(const math::Vector3& v) {
  return btVector3(btScalar(v.x), btScalar(v.y), btScalar(v.z));
}
}  // namespace helper
}  // namespace bullet
}  // namespace legend
#endif  //! LEGEND_BULLET_BULLET_HELPER_H
