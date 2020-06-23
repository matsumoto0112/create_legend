#ifndef LEGEND_MATH_MATRIX_4X4_H_
#define LEGEND_MATH_MATRIX_4X4_H_

#include "src/math/vector_2.h"
#include "src/math/vector_3.h"

/**
 * @file matrix_4x4.h
 * @brief 4x4�s���`
 */

namespace legend {
namespace math {
/**
 * @brief 4x4�s��
 */
class Matrix4x4 {
 public:
  //! �s��
  std::array<std::array<float, 4>, 4> m;

 public:
  //! �P�ʍs��
  static const Matrix4x4 kIdentity;
  //! �[���s��
  static const Matrix4x4 kZero;

 public:
  /**
   * @brief �R���X�g���N�^
   */
  Matrix4x4();
  /**
   * @brief �R���X�g���N�^
   */
  Matrix4x4(float m11, float m12, float m13, float m14, float m21, float m22,
            float m23, float m24, float m31, float m32, float m33, float m34,
            float m41, float m42, float m43, float m44);
  /**
   * @brief �R���X�g���N�^
   */
  Matrix4x4(const std::array<std::array<float, 4>, 4>& m);
  /**
   * @brief �R���X�g���N�^
   */
  Matrix4x4(const Matrix4x4& m);
  /**
   * @brief ������Z�q
   */
  Matrix4x4& operator=(const Matrix4x4& mat) &;
  /**
   * @brief ������Z�q
   */
  Matrix4x4& operator=(Matrix4x4&& mat) & noexcept;
  /**
   * @brief �P���v���X���Z�q
   */
  Matrix4x4 operator+();
  /**
   * @brief �P���}�C�i�X���Z�q
   */
  Matrix4x4 operator-();
  /**
   * @brief ���Z������Z�q
   */
  Matrix4x4& operator+=(const Matrix4x4& mat);
  /**
   * @brief ���Z������Z�q
   */
  Matrix4x4& operator-=(const Matrix4x4& mat);
  /**
   * @brief ��Z������Z�q
   */
  Matrix4x4& operator*=(float k);
  /**
   * @brief ��Z������Z�q
   */
  Matrix4x4& operator*=(const Matrix4x4& mat);
  /**
   * @brief ���Z������Z�q
   */
  Matrix4x4& operator/=(float k);
  /**
   * @brief ���s�ړ��s��̍쐬
   * @param v �ړ���
   */
  static Matrix4x4 CreateTranslate(const Vector3& v);
  /**
   * @brief X����]�s��̍쐬
   * @param rad ��]��
   */
  static Matrix4x4 CreateRotationX(float rad);
  /**
   * @brief Y����]�s��̍쐬
   * @param rad ��]��
   */
  static Matrix4x4 CreateRotationY(float rad);
  /**
   * @brief Z����]�s��̍쐬
   * @param rad ��]��
   */
  static Matrix4x4 CreateRotationZ(float rad);
  /**
   * @brief ��]�s��̍쐬
   * @param r �e���̉�]��
   */
  static Matrix4x4 CreateRotation(const Vector3& r);
  /**
   * @brief �g��E�k���s��̍쐬
   * @param s �e���̊g��E�k���̑傫��
   */
  static Matrix4x4 CreateScale(const Vector3& s);
  /**
   * @brief �r���[�s��̍쐬
   * @param eye ���_
   * @param at �����_
   * @param up ������̃x�N�g��
   */
  static Matrix4x4 CreateView(const Vector3& eye, const Vector3& at,
                              const Vector3& up);
  /**
   * @brief �v���W�F�N�V�����s��̍쐬
   * @param fov_y ����p(rad)
   * @param aspect �A�X�y�N�g��
   * @param near_z �ŋߓ_
   * @param far_z �ŉ��_
   */
  static Matrix4x4 CreateProjection(float fov_y, float aspect, float near_z,
                                    float far_z);
  /**
   * @brief �����e�s��̍쐬
   * @param screen_size ��ʂ̑傫��
   */
  static Matrix4x4 CreateOrthographic(const Vector2& screen_size);
  /**
   * @brief �]�u�s����擾����
   */
  Matrix4x4 Transpose() const;
  /**
   * @brief �s�񎮂����߂�
   */
  float Determinant() const;
  /**
   * @brief �t�s������߂�
   * @param mat ���߂�s��
   */
  Matrix4x4 Inverse() const;
  /**
   * @brief �s��̕��
   * @param mat1 �s��1
   * @param mat2 �s��2
   * @param t ��ԌW��
   */
  static Matrix4x4 Lerp(const Matrix4x4& mat1, const Matrix4x4& mat2, float t);
  /**
   * @brief �x�N�g���Ƃ̊|���Z(�@���x�N�g���p�ŉ�]�̂�)
   */
  Vector3 TransformNormal(const Vector3& v);

  /**
   * @brief �x�N�g���ƍs��̐ς����߁Aw�ŏ��Z���ꂽ�l��Ԃ�
   */
  static Vector3 MultiplyCoord(const Vector3& v, const Matrix4x4& m);
};

/**
 * @brief ������r���Z�q
 */
inline bool operator==(const Matrix4x4& m1, const Matrix4x4& m2) {
  return m1.m == m2.m;
}
/**
 * @brief ������r���Z�q
 */
inline bool operator!=(const Matrix4x4& m1, const Matrix4x4& m2) {
  return !(m1 == m2);
}
/**
 * @brief ���Z
 */
Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2);
/**
 * @brief ���Z
 */
Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2);
/**
 * @brief ��Z
 */
Matrix4x4 operator*(const Matrix4x4& m, float s);
/**
 * @brief ��Z
 */
Matrix4x4 operator*(float s, const Matrix4x4& m);
/**
 * @brief ��Z
 */
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);
/**
 * @brief ���Z
 */
Matrix4x4 operator/(const Matrix4x4& m, float s);

}  // namespace math
}  // namespace legend

#endif  //! LEGEND_MATH_MATRIX_4X4_H_
