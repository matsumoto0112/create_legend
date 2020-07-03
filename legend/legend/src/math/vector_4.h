#ifndef LEGEND_MATH_VECTOR_4_H_
#define LEGEND_MATH_VECTOR_4_H_

/**
 * @file vector_4.h
 * @brief 4次元ベクトルクラス定義
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
  //! ゼロベクトル
  static const Vector4 kZeroVector;
  //! 単位ベクトル
  static const Vector4 kUnitVector;

 public:
  /**
   * @brief コンストラクタ
   */
  Vector4();
  /**
   * @brief コンストラクタ
   */
  Vector4(float x, float y, float z, float w);
  /**
   * @brief コピーコンストラクタ
   */
  Vector4(const Vector4& v) = default;
  /**
   * @brief コピー代入演算子
   */
  Vector4& operator=(const Vector4& v) = default;
  /**
   * @brief 加算代入演算子
   */
  Vector4& operator+=(const Vector4& v);
  /**
   * @brief 減算代入演算子
   */
  Vector4& operator-=(const Vector4& v);
  /**
   * @brief 乗算代入演算子
   */
  Vector4& operator*=(float k);
  /**
   * @brief 除算代入演算子
   */
  Vector4& operator/=(float k);
  /**
   * @brief 大きさの二乗を求める
   */
  float MagnitudeSquared() const;
  /**
   * @brief 大きさを求める
   */
  float Magnitude() const;
  /**
   * @brief 正規化ベクトルを求める
   */
  Vector4 Normalized() const;
  /**
   * @brief 内積
   */
  static float Dot(const Vector4& v, const Vector4& w);
};
/**
 * @brief 比較演算子
 */
inline bool operator==(const Vector4& v, const Vector4& w) {
  return (v.x == w.x && v.y == w.y && v.z == w.z);
}

/**
 * @brief 比較演算子
 */
inline bool operator!=(const Vector4& v, const Vector4& w) { return !(v == w); }

/**
 * @brief 加算演算子
 */
inline Vector4 operator+(const Vector4& v, const Vector4& w) {
  return Vector4(v) += w;
}

/**
 * @brief 減算演算子
 */
inline Vector4 operator-(const Vector4& v, const Vector4& w) {
  return Vector4(v) -= w;
}

/**
 * @brief 乗算演算子
 */
inline Vector4 operator*(const Vector4& v, float k) { return Vector4(v) *= k; }

/**
 * @brief 乗算演算子
 */
inline Vector4 operator*(float k, const Vector4& v) { return Vector4(v) *= k; }

/**
 * @brief 除算演算子
 */
inline Vector4 operator/(const Vector4& v, float k) { return Vector4(v) /= k; }

}  // namespace math
}  // namespace legend
#endif  //! LEGEND_MATH_VECTOR_4_H_
