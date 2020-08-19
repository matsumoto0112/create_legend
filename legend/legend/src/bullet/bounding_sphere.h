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
    btVector3 position = btVector3(0, 0, 0);        //座標
    btQuaternion rotation = btQuaternion(0, 0, 0);  //回転
    btScalar radius = 1.0f;                         //大きさ
    btScalar mass = 1.0f;                           //質量
    btScalar restitution = 0.5f;                    //反発
    btScalar friction = 0.75f;                       //摩擦
  };

 public:
  BoundingSphere(const InitializeParameter& parameter);
  ~BoundingSphere();
  bool Update() override;
};
}  // namespace bullet
}  // namespace legend

#endif  //! LEGEND_BULLET_BOUNDING_SPHERE_H_
