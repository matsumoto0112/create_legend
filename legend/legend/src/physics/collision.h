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
   * @brief OBB�̓��e������r
   * @param ������1
   * @param ������2
   * @param ������
   * @param ����
   */
  bool IsCompareLengthOBB(BoundingBox& obb1, BoundingBox& obb2,
                          math::Vector3 v_sep, math::Vector3 distance);
  /**
   * @brief �����̂ƕ��ʂ̏Փ˔���
   * @param ������
   * @param ����
   */
  bool Collision_OBB_Plane(BoundingBox& obb, Plane& plane);
  /**
   * @brief ���ƒ����̂̏Փ˔���
   * @param ��
   * @param ������
   */
  bool Collision_Sphere_OBB(Sphere& sphere, BoundingBox& obb);
  /**
   * @brief ���Ɛ����̋�����r
   * @param ��
   * @param �n�_
   * @param �I�_
   */
  bool IsCheckLength_Point_Segment(Sphere& sphere, math::Vector3 start_position,
                                math::Vector3 end_position);
  /**
   * @brief ���ƕ��ʂ̏Փ˔���
   * @param ��
   * @param ����
   */
  bool Collision_Sphere_Plane(Sphere& sphere, Plane& plane);
  /**
   * @brief ���C�̒����𒲂ׂ�
   * @param ���C
   */
  float GetRayLength(Ray& ray) const;
  /**
   * @brief ���C�ƒ����̂̏Փ˔���
   * @param ���C
   * @param ������
   */
  bool Collision_Ray_OBB(Ray& ray, BoundingBox& obb);
  /**
   * @brief ���C�ƒ����̂̋�����r
   * @param ���C
   * @param ������
   * @param ������
   * @param ����
   */
  bool IsCheckLength_Ray_Obb(Ray& ray, BoundingBox& obb, math::Vector3 v_sep,
                             math::Vector3 distance);
  /**
   * @brief ���C�ƕ��ʂ̏Փ˔���
   * @param ���C
   * @param ����
   */
  bool Collision_Ray_Plane(Ray& ray, Plane& plane);
  /**
   * @brief ���C�Ƌ��̏Փ˔���
   * @param ���C
   * @param ��
   */
  bool Collision_Ray_Sphere(Ray& ray, Sphere& sphere);
};
}  // namespace physics
}  // namespace legend

#endif  //! LEGEND_PHYSICS_COLLISION_H_
