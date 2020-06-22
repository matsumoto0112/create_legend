#ifndef LEGEND_MATH_VECTOR_2_H_
#define LEGEND_MATH_VECTOR_2_H_

/**
 * @file vector_2.h
 * @brief �񎟌��x�N�g���N���X��`
 */
namespace legend {
namespace math {

/**
 * @brief �񎟌��x�N�g���N���X
 */
class Vector2 {
 public:
  //! x
  float x;
  //! y
  float y;

 public:
  //! �[���x�N�g��
  static const Vector2 kZeroVector;
  //! ���x�N�g��
  static const Vector2 kLeftVector;
  //! �E�x�N�g��
  static const Vector2 kRightVector;
  //! ��x�N�g��
  static const Vector2 kUpVector;
  //! ���x�N�g��
  static const Vector2 kDownVector;
  //! �P�ʃx�N�g��
  static const Vector2 kUnitVector;

 public:
  /**
   * @brief �f�t�H���g�R���X�g���N�^
   */
  Vector2();
  /**
   * @brief �R���X�g���N�^
   */
  Vector2(float x, float y);
  /**
   * @brief �R�s�[�R���X�g���N�^
   */
  Vector2(const Vector2& v) = default;
  /**
   * @brief �R�s�[������Z�q
   */
  Vector2& operator=(const Vector2& v) = default;
  /**
   * @brief ���Z������Z�q
   */
  Vector2& operator+=(const Vector2& v);
  /**
   * @brief ���Z������Z�q
   */
  Vector2& operator-=(const Vector2& v);
  /**
   * @brief ��Z������Z�q
   */
  Vector2& operator*=(float k);
  /**
   * @brief ���Z������Z�q
   */
  Vector2& operator/=(float k);
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
  Vector2 Normalized() const;
  /**
   * @brief ����
   */
  static float Dot(const Vector2& v, const Vector2& w);
  /**
   * @brief �O��
   */
  static float Cross(const Vector2& v, const Vector2& w);
};

/**
 * @brief ��r���Z�q
 */
inline bool operator==(const Vector2& v, const Vector2& w) {
  return (v.x == w.x && v.y == w.y);
}

/**
 * @brief ��r���Z�q
 */
inline bool operator!=(const Vector2& v, const Vector2& w) { return !(v == w); }

/**
 * @brief ���Z���Z�q
 */
inline Vector2 operator+(const Vector2& v, const Vector2& w) {
  return Vector2(v) += w;
}

/**
 * @brief ���Z���Z�q
 */
inline Vector2 operator-(const Vector2& v, const Vector2& w) {
  return Vector2(v) -= w;
}

/**
 * @brief ��Z���Z�q
 */
inline Vector2 operator*(const Vector2& v, float k) { return Vector2(v) *= k; }

/**
 * @brief ��Z���Z�q
 */
inline Vector2 operator*(float k, const Vector2& v) { return Vector2(v) *= k; }

/**
 * @brief ���Z���Z�q
 */
inline Vector2 operator/(const Vector2& v, float k) { return Vector2(v) /= k; }

}  // namespace math
}  // namespace legend
#endif  // !LEGEND_MATH_VECTOR_2_H_
