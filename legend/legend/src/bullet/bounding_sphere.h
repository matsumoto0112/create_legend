#ifndef LEGEND_BULLET_BOUNDING_SPHERE_H_
#define LEGEND_BULLET_BOUNDING_SPHERE_H_

#include "src/bullet/collider.h"
/**
 * @file bounding_box.h
 * @brief �����̂̕�������N���X
 */
namespace legend {
namespace bullet {

class BoundingSphere : public Collider {
 public:
  /**
   * @brief �������p�����[�^
   */
  struct InitializeParameter {
    btVector3 position = btVector3(0, 0, 0);        //���W
    btQuaternion rotation = btQuaternion(0, 0, 0);  //��]
    btScalar radius = 1.0f;                         //�傫��
    btScalar mass = 1.0f;                           //����
    btScalar restitution = 0.5f;                    //����
    btScalar friction = 0.75f;                       //���C
  };

 public:
  BoundingSphere(const InitializeParameter& parameter);
  ~BoundingSphere();
  bool Update() override;
};
}  // namespace bullet
}  // namespace legend

#endif  //! LEGEND_BULLET_BOUNDING_SPHERE_H_
