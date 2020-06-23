#ifndef LEGEND_MATH_INT_VECTOR_2_H_
#define LEGEND_MATH_INT_VECTOR_2_H_

namespace legend {
namespace math {
/**
 * @class IntVector2
 * @brief �����^�񎟌��x�N�g��
 */
class IntVector2 {
 public:
  //! x
  i32 x;
  //! y
  i32 y;

 public:
  //! �[���x�N�g��
  static const IntVector2 kZeroVector;
  //! ���x�N�g��
  static const IntVector2 kLeftVector;
  //! �E�x�N�g��
  static const IntVector2 kRightVector;
  //! ��x�N�g��
  static const IntVector2 kUpVector;
  //! ���x�N�g��
  static const IntVector2 kDownVector;
  //! �P�ʃx�N�g��
  static const IntVector2 kUnitVector;

 public:
  /**
   * @brief �f�t�H���g�R���X�g���N�^
   */
  IntVector2();
  /**
   * @brief �R���X�g���N�^
   */
  IntVector2(i32 x, i32 y);
  /**
   * @brief �R�s�[�R���X�g���N�^
   */
  IntVector2(const IntVector2& v) = default;
  /**
   * @brief �R�s�[������Z�q
   */
  IntVector2& operator=(const IntVector2& v) = default;
  /**
   * @brief ���Z������Z�q
   */
  IntVector2& operator+=(const IntVector2& v);
  /**
   * @brief ���Z������Z�q
   */
  IntVector2& operator-=(const IntVector2& v);
  /**
   * @brief ��Z������Z�q
   */
  IntVector2& operator*=(i32 k);
  /**
   * @brief ���Z������Z�q
   */
  IntVector2& operator/=(i32 k);
};

/**
 * @brief ��r���Z�q
 */
inline bool operator==(const IntVector2& v, const IntVector2& w) {
  return (v.x == w.x && v.y == w.y);
}

/**
 * @brief ��r���Z�q
 */
inline bool operator!=(const IntVector2& v, const IntVector2& w) {
  return !(v == w);
}

/**
 * @brief ���Z���Z�q
 */
inline IntVector2 operator+(const IntVector2& v, const IntVector2& w) {
  return IntVector2(v) += w;
}

/**
 * @brief ���Z���Z�q
 */
inline IntVector2 operator-(const IntVector2& v, const IntVector2& w) {
  return IntVector2(v) -= w;
}

/**
 * @brief ��Z���Z�q
 */
inline IntVector2 operator*(const IntVector2& v, i32 k) {
  return IntVector2(v) *= k;
}

/**
 * @brief ��Z���Z�q
 */
inline IntVector2 operator*(i32 k, const IntVector2& v) {
  return IntVector2(v) *= k;
}

/**
 * @brief ���Z���Z�q
 */
inline IntVector2 operator/(const IntVector2& v, i32 k) {
  return IntVector2(v) /= k;
}

}  // namespace math
}  // namespace legend

#endif  // !LEGEND_MATH_INT_VECTOR_2_H_
