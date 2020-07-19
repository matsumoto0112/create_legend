#ifndef LEGEND_PHYSICS_COLLISION_H_
#define LEGEND_PHYSICS_COLLISION_H_

#include "src/physics/bounding_box.h"
#include "src/physics/plane.h"
#include "src/util/singleton.h"

namespace legend {
namespace physics {
/**
 * @class Collision
 * @brief �����蔻��𒲂ׂ�
 */
class Collision : public util::Singleton<Collision> {
 public:
  /**
   * @brief �����̓��m�̏Փ˔���
   * @param ������1
   * @param ������2
   */
  bool Collision_OBB_OBB(BoundingBox& obb1, BoundingBox& obb2);
  /**
   * @brief �����̂ƕ��ʂ̏Փ˔���
   * @param ������
   * @param ����
   */
  bool Collision_OBB_Plane(BoundingBox& obb, Plane& plane);
  /**
  * @brief OBB�̓��e������r
  * @param ������1
  * @param ������2
  * @param ������
  * @param ����
  */
  bool IsCompareLengthOBB(BoundingBox& obb1, BoundingBox& obb2,
                          math::Vector3 vSep, math::Vector3 distance);
};
}  // namespace physics
}  // namespace legend

#endif  //! LEGEND_PHYSICS_COLLISION_H_
