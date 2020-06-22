#ifndef LEGEND_MATH_INT_VECTOR_2_H_
#define LEGEND_MATH_INT_VECTOR_2_H_

namespace dx12_lib {
namespace math {
/**
 * @class IntVector2
 * @brief 整数型二次元ベクトル
 */
class IntVector2 {
 public:
  //! x
  int x;
  //! y
  int y;

 public:
  //! ゼロベクトル
  static const IntVector2 kZeroVector;
  //! 左ベクトル
  static const IntVector2 kLeftVector;
  //! 右ベクトル
  static const IntVector2 kRightVector;
  //! 上ベクトル
  static const IntVector2 kUpVector;
  //! 下ベクトル
  static const IntVector2 kDownVector;
  //! 単位ベクトル
  static const IntVector2 kUnitVector;

 public:
  /**
   * @brief デフォルトコンストラクタ
   */
  IntVector2();
  /**
   * @brief コンストラクタ
   */
  IntVector2(int x, int y);
  /**
   * @brief コピーコンストラクタ
   */
  IntVector2(const IntVector2& v) = default;
  /**
   * @brief コピー代入演算子
   */
  IntVector2& operator=(const IntVector2& v) = default;
  /**
   * @brief 加算代入演算子
   */
  IntVector2& operator+=(const IntVector2& v);
  /**
   * @brief 減算代入演算子
   */
  IntVector2& operator-=(const IntVector2& v);
  /**
   * @brief 乗算代入演算子
   */
  IntVector2& operator*=(int k);
  /**
   * @brief 除算代入演算子
   */
  IntVector2& operator/=(int k);
};

/**
 * @brief 比較演算子
 */
inline bool operator==(const IntVector2& v, const IntVector2& w) {
  return (v.x == w.x && v.y == w.y);
}

/**
 * @brief 比較演算子
 */
inline bool operator!=(const IntVector2& v, const IntVector2& w) {
  return !(v == w);
}

/**
 * @brief 加算演算子
 */
inline IntVector2 operator+(const IntVector2& v, const IntVector2& w) {
  return IntVector2(v) += w;
}

/**
 * @brief 減算演算子
 */
inline IntVector2 operator-(const IntVector2& v, const IntVector2& w) {
  return IntVector2(v) -= w;
}

/**
 * @brief 乗算演算子
 */
inline IntVector2 operator*(const IntVector2& v, int k) {
  return IntVector2(v) *= k;
}

/**
 * @brief 乗算演算子
 */
inline IntVector2 operator*(int k, const IntVector2& v) {
  return IntVector2(v) *= k;
}

/**
 * @brief 除算演算子
 */
inline IntVector2 operator/(const IntVector2& v, int k) {
  return IntVector2(v) /= k;
}

}  // namespace math
}  // namespace dx12_lib

#endif  // !LEGEND_MATH_INT_VECTOR_2_H_
