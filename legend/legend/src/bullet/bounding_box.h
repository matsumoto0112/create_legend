#ifndef LEGEND_BULLET_BOUNDING_BOX_H_
#define LEGEND_BULLET_BOUNDING_BOX_H_

#include "src/bullet/collider.h"
/**
 * @file bounding_box.h
 * @brief �����̂̕�������N���X
 */
namespace legend {
namespace bullet {

class BoundingBox : public Collider {
 public:
  /**
   * @brief �������p�����[�^
   */
  struct InitializeParameter {
    math::Vector3 position = math::Vector3::kZeroVector;      //���W
    math::Quaternion rotation = math::Quaternion::kIdentity;  //��]
    math::Vector3 scale = math::Vector3::kUnitVector;         //�傫��
    float mass = 1.0f;                                        //����
    float restitution = 0.5f;                                 //����
    float friction = 0.75f;                                   //���C
  };

 public:
  BoundingBox(actor::Actor* owner, const InitializeParameter& parameter);
  virtual ~BoundingBox();
  bool Update() override;
  void SetScale(math::Vector3 scale);
};
}  // namespace bullet
}  // namespace legend

#endif  //! LEGEND_BULLET_BOUNDING_BOX_H_
