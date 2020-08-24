#ifndef LEGEND_BULLET_SHAPE_TYPE_H_
#define LEGEND_BULLET_SHAPE_TYPE_H_
#include "src/math/quaternion.h"

namespace legend {
namespace bullet {

/**
 * @brief �����̂̏������p�����[�^
 */
struct BoxInitializeParameter {
  math::Vector3 position = math::Vector3::kZeroVector;      //���W
  math::Quaternion rotation = math::Quaternion::kIdentity;  //��]
  math::Vector3 scale = math::Vector3::kUnitVector;         //�傫��
  float mass = 1.0f;                                        //����
  float restitution = 0.5f;                                 //����
  float friction = 0.75f;                                   //���C
};

/**
 * @brief �g���K�[�����̂̏������p�����[�^
 */
struct TriggerGhostInitializeParameter {
  math::Vector3 position = math::Vector3::kZeroVector;      //���W
  math::Quaternion rotation = math::Quaternion::kIdentity;  //��]
  math::Vector3 scale = math::Vector3::kUnitVector;         //�傫��
};

/**
 * @brief ���̂̏������p�����[�^
 */
struct SphereInitializeParameter {
  math::Vector3 position = math::Vector3::kZeroVector;      //���W
  math::Quaternion rotation = math::Quaternion::kIdentity;  //��]
  float radius = 1.0f;                                      //�傫��
  float mass = 1.0f;                                        //����
  float restitution = 0.5f;                                 //����
  float friction = 0.75f;                                   //���C
};

/**
 * @brief �g���K�[���̂̏������p�����[�^
 */
struct TriggerSphereInitializeParameter {
  math::Vector3 position = math::Vector3::kZeroVector;      //���W
  math::Quaternion rotation = math::Quaternion::kIdentity;  //��]
  float radius = 1.0f;                                      //�傫��
};

}  // namespace bullet
}  // namespace legend

#endif  //! LEGEND_BULLET_SHAPE_TYPE_H_
