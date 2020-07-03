#ifndef LEGEND_MATH_VECTOR_4_H_
#define LEGEND_MATH_VECTOR_4_H_

/**
 * @file vector_4.h
 * @brief 4�����x�N�g���N���X��`
 */
namespace legend {
namespace math {
class Vector4 {
 public:
  //! x
  float x;
  //! y
  float y;
  //! z
  float z;
  //! w
  float w;

 public:
  //! �[���x�N�g��
  static const Vector4 kZeroVector;
  //! �P�ʃx�N�g��
  static const Vector4 kUnitVector;

 public:
  /**
   * @brief �R���X�g���N�^
   */
  Vector4();
  /**
   * @brief �R���X�g���N�^
   */
  Vector4(float x, float y, float z, float w);
  /**
   * @brief �R�s�[�R���X�g���N�^
   */
  Vector4(const Vector4& v) = default;
  /**
   * @brief �R�s�[������Z�q
   */
  Vector4& operator=(const Vector4& v) = default;
  /**
   * @brief ���Z������Z�q
   */
  Vector4& operator+=(const Vector4& v);
  /**
   * @brief ���Z������Z�q
   */
  Vector4& operator-=(const Vector4& v);
  /**
   * @brief ��Z������Z�q
   */
  Vector4& operator*=(float k);
  /**
   * @brief ���Z������Z�q
   */
  Vector4& operator/=(float k);
  /**
   * @brief �傫���̓������߂�
   */
  float MagnitudeSquared() const;
  /**
   * @brief �傫�������߂�
   */
  float Magnitude() const;
  /**
   * @brief ���K���x�N�g�������߂�
   */
  Vector4 Normalized() const;
  /**
   * @brief ����
   */
  static float Dot(const Vector4& v, const Vector4& w);
};
/**
 * @brief ��r���Z�q
 */
inline bool operator==(const Vector4& v, const Vector4& w) {
  return (v.x == w.x && v.y == w.y && v.z == w.z);
}

/**
 * @brief ��r���Z�q
 */
inline bool operator!=(const Vector4& v, const Vector4& w) { return !(v == w); }

/**
 * @brief ���Z���Z�q
 */
inline Vector4 operator+(const Vector4& v, const Vector4& w) {
  return Vector4(v) += w;
}

/**
 * @brief ���Z���Z�q
 */
inline Vector4 operator-(const Vector4& v, const Vector4& w) {
  return Vector4(v) -= w;
}

/**
 * @brief ��Z���Z�q
 */
inline Vector4 operator*(const Vector4& v, float k) { return Vector4(v) *= k; }

/**
 * @brief ��Z���Z�q
 */
inline Vector4 operator*(float k, const Vector4& v) { return Vector4(v) *= k; }

/**
 * @brief ���Z���Z�q
 */
inline Vector4 operator/(const Vector4& v, float k) { return Vector4(v) /= k; }

}  // namespace math
}  // namespace legend
#endif  //! LEGEND_MATH_VECTOR_4_H_
