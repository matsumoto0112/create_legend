#ifndef LEGEND_BULLET_SHAPE_TYPE_H_
#define LEGEND_BULLET_SHAPE_TYPE_H_
#include "src/math/quaternion.h"

namespace legend {
namespace bullet {

/**
 * @brief 立方体の初期化パラメータ
 */
struct BoxInitializeParameter {
  math::Vector3 position = math::Vector3::kZeroVector;      //座標
  math::Quaternion rotation = math::Quaternion::kIdentity;  //回転
  math::Vector3 scale = math::Vector3::kUnitVector;         //大きさ
  float mass = 1.0f;                                        //質量
  float restitution = 0.5f;                                 //反発
  float friction = 0.75f;                                   //摩擦
};

/**
 * @brief トリガー立方体の初期化パラメータ
 */
struct TriggerGhostInitializeParameter {
  math::Vector3 position = math::Vector3::kZeroVector;      //座標
  math::Quaternion rotation = math::Quaternion::kIdentity;  //回転
  math::Vector3 scale = math::Vector3::kUnitVector;         //大きさ
};

/**
 * @brief 球体の初期化パラメータ
 */
struct SphereInitializeParameter {
  math::Vector3 position = math::Vector3::kZeroVector;      //座標
  math::Quaternion rotation = math::Quaternion::kIdentity;  //回転
  float radius = 1.0f;                                      //大きさ
  float mass = 1.0f;                                        //質量
  float restitution = 0.5f;                                 //反発
  float friction = 0.75f;                                   //摩擦
};

/**
 * @brief トリガー球体の初期化パラメータ
 */
struct TriggerSphereInitializeParameter {
  math::Vector3 position = math::Vector3::kZeroVector;      //座標
  math::Quaternion rotation = math::Quaternion::kIdentity;  //回転
  float radius = 1.0f;                                      //大きさ
};

}  // namespace bullet
}  // namespace legend

#endif  //! LEGEND_BULLET_SHAPE_TYPE_H_
