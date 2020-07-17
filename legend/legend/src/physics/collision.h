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
   * @brief �������ɓ��e���ꂽ���������瓊�e���������Z�o
   * @param ������
   * @param x��
   * @param y��
   * @param z��
   */
  float GetSeparateAxis(math::Vector3 sep, math::Vector3 x, math::Vector3 y,
                        math::Vector3 z = math::Vector3::kZeroVector);
};
}  // namespace physics
}  // namespace legend

#endif  //! LEGEND_PHYSICS_COLLISION_H_
