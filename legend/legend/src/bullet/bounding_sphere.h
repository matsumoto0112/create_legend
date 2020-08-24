#ifndef LEGEND_BULLET_BOUNDING_SPHERE_H_
#define LEGEND_BULLET_BOUNDING_SPHERE_H_

#include "src/bullet/collider.h"
/**
 * @file bounding_box.h
 * @brief 立方体の物理判定クラス
 */
namespace legend {
namespace bullet {

class BoundingSphere : public Collider {
 public:
  /**
   * @brief 初期化パラメータ
   */
  struct InitializeParameter {
    math::Vector3 position = math::Vector3::kZeroVector;      //座標
    math::Quaternion rotation = math::Quaternion::kIdentity;  //回転
    float radius = 1.0f;                                      //大きさ
    float mass = 1.0f;                                        //質量
    float restitution = 0.5f;                                 //反発
    float friction = 0.75f;                                   //摩擦
  };

 public:
  BoundingSphere(actor::Actor* owner, const InitializeParameter& parameter);
  virtual ~BoundingSphere();
  bool Update() override;
  void SetScale(float radius);
};
}  // namespace bullet
}  // namespace legend

#endif  //! LEGEND_BULLET_BOUNDING_SPHERE_H_
