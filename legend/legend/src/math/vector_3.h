#ifndef LEGEND_MATH_VECTOR_3_H_
#define LEGEND_MATH_VECTOR_3_H_

/**
 * @file vector_3.h
 * @brief �O�����x�N�g���N���X��`
 */
namespace legend {
namespace math {

/**
 * @brief �O�����x�N�g���N���X
 */
class Vector3 {
 public:
  //! x
  float x;
  //! y
  float y;
  //! z
  float z;

 public:
  //! �[���x�N�g��
  static const Vector3 kZeroVector;
  //! ���x�N�g��
  static const Vector3 kLeftVector;
  //! �E�x�N�g��
  static const Vector3 kRightVector;
  //! ��x�N�g��
  static const Vector3 kUpVector;
  //! ���x�N�g��
  static const Vector3 kDownVector;
  //! �O�x�N�g��
  static const Vector3 kForwardVector;
  //! ��x�N�g��
  static const Vector3 kBackwardVector;
  //! �P�ʃx�N�g��
  static const Vector3 kUnitVector;

 public:
  /**
   * @brief �f�t�H���g�R���X�g���N�^
   */
  Vector3();
  /**
   * @brief �R���X�g���N�^
   */
  Vector3(float x, float y, float z);
  /**
   * @brief �R�s�[�R���X�g���N�^
   */
  Vector3(const Vector3& v) = default;
  /**
   * @brief �R�s�[������Z�q
   */
  Vector3& operator=(const Vector3& v) = default;
  /**
   * @brief ���Z������Z�q
   */
  Vector3& operator+=(const Vector3& v);
  /**
   * @brief ���Z������Z�q
   */
  Vector3& operator-=(const Vector3& v);
  /**
   * @brief ��Z������Z�q
   */
  Vector3& operator*=(float k);
  /**
   * @brief ���Z������Z�q
   */
  Vector3& operator/=(float k);
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
  Vector3 Normalized() const;
  /**
   * @brief ����
   */
  static float Dot(const Vector3& v, const Vector3& w);
  /**
   * @brief �O��
   */
  static Vector3 Cross(const Vector3& v, const Vector3& w);
};

/**
 * @brief ��r���Z�q
 */
inline bool operator==(const Vector3& v, const Vector3& w) {
  return (v.x == w.x && v.y == w.y && v.z == w.z);
}

/**
 * @brief ��r���Z�q
 */
inline bool operator!=(const Vector3& v, const Vector3& w) { return !(v == w); }

/**
 * @brief ���Z���Z�q
 */
inline Vector3 operator+(const Vector3& v, const Vector3& w) {
  return Vector3(v) += w;
}

/**
 * @brief ���Z���Z�q
 */
inline Vector3 operator-(const Vector3& v, const Vector3& w) {
  return Vector3(v) -= w;
}

/**
 * @brief ��Z���Z�q
 */
inline Vector3 operator*(const Vector3& v, float k) { return Vector3(v) *= k; }

/**
 * @brief ��Z���Z�q
 */
inline Vector3 operator*(float k, const Vector3& v) { return Vector3(v) *= k; }

/**
 * @brief ���Z���Z�q
 */
inline Vector3 operator/(const Vector3& v, float k) { return Vector3(v) /= k; }

}  // namespace math
}  // namespace legend
#endif  // !LEGEND_MATH_VECTOR_3_H_
