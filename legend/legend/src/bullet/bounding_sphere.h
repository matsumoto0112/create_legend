#ifndef LEGEND_BULLET_BOUNDING_SPHERE_H_
#define LEGEND_BULLET_BOUNDING_SPHERE_H_
/**
* @file bounding_sphere.h
* @brief ���̂̕�������N���X
*/

#include "src/bullet/collider.h"
namespace legend {
namespace bullet {

class BoundingSphere : public Collider {
 public:
  /**
   * @brief �������p�����[�^
   */
  struct InitializeParameter {
    math::Vector3 position = math::Vector3::kZeroVector;      //���W
    math::Quaternion rotation = math::Quaternion::kIdentity;  //��]
    float radius = 1.0f;                                      //�傫��
    float mass = 1.0f;                                        //����
    float restitution = 0.5f;                                 //����
    float friction = 0.75f;                                   //���C
  };

 public:
  BoundingSphere(actor::Actor* owner, const InitializeParameter& parameter);
  virtual ~BoundingSphere();
  bool Update() override;
  void SetScale(float radius, float mass);
};
}  // namespace bullet
}  // namespace legend

#endif  //! LEGEND_BULLET_BOUNDING_SPHERE_H_
