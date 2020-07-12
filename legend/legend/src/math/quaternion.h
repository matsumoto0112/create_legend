#ifndef LEGEND_MATH_QUATERNION_H_
#define LEGEND_MATH_QUATERNION_H_

/**
 * @file quaternion.h
 * @brief  �l�����Ǘ��N���X��`
 */

#include "src/math/matrix_4x4.h"
#include "src/math/vector_3.h"

namespace legend {
namespace math {

/**
 * @class Quaternion
 * @brief �l����
 */
class Quaternion {
 public:
  //! �P�ʎl����
  static const Quaternion kIdentity;

 public:
  //! x�x�N�g��
  float x;
  //! y�x�N�g��
  float y;
  //! z�x�N�g��
  float z;
  //! �Ίp
  float w;

 public:
  /**
   * @brief �R���X�g���N�^
   */
  Quaternion();
  /**
   * @brief �R���X�g���N�^
   */
  Quaternion(float x, float y, float z, float w);
  /**
   * @brief �R���X�g���N�^
   * @param nv ���K�����ꂽ��]��
   * @param angle ��]��
   */
  Quaternion(const Vector3& nv, float angle);
  /**
   * @brief �f�X�g���N�^
   */
  ~Quaternion();
  /**
   * @brief ���K���l�������擾����
   */
  Quaternion Normalized() const;
  /**
   * @brief �����̎l���������߂�
   */
  Quaternion Conjugate() const;
  /**
   * @brief ���[���E�s�b�`�E���[����l�����𐶐�����
   */
  static Quaternion FromEular(float roll, float pitch, float yaw);
  /**
   * @brief ���[���E�s�b�`�E���[����l�����𐶐�����
   */
  static Quaternion FromEular(const Vector3& v);
  /**
   * @brief �I�C���[�p���擾����
   */
  Vector3 ToEular() const;
  /**
   * @brief �l�������I�C���[�p�ɕϊ�����
   */
  static Vector3 ToEular(const Quaternion& q);
  /**
   * @brief ��]�s��ɕϊ�����
   */
  static Matrix4x4 ToMatrix(const Quaternion& q);
};

/**
 * @brief ��Z���Z�q
 */
Quaternion operator*(const Quaternion& q1, const Quaternion& q2);
/**
 * @brief �x�N�g���Ƃ̐�
 */
Vector3 operator*(const Quaternion& q, const Vector3& v);

}  // namespace math
}  // namespace legend

#endif  //! LEGEND_MATH_QUATERNION_H_
