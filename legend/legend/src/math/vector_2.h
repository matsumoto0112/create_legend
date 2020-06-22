#ifndef LEGEND_MATH_VECTOR_2_H_
#define LEGEND_MATH_VECTOR_2_H_

/**
 * @file vector_2.h
 * @brief 二次元ベクトルクラス定義
 */
namespace legend {
namespace math {

/**
 * @brief 二次元ベクトルクラス
 */
class Vector2 {
 public:
  //! x
  float x;
  //! y
  float y;

 public:
  //! ゼロベクトル
  static const Vector2 kZeroVector;
  //! 左ベクトル
  static const Vector2 kLeftVector;
  //! 右ベクトル
  static const Vector2 kRightVector;
  //! 上ベクトル
  static const Vector2 kUpVector;
  //! 下ベクトル
  static const Vector2 kDownVector;
  //! 単位ベクトル
  static const Vector2 kUnitVector;

 public:
  /**
   * @brief デフォルトコンストラクタ
   */
  Vector2();
  /**
   * @brief コンストラクタ
   */
  Vector2(float x, float y);
  /**
   * @brief コピーコンストラクタ
   */
  Vector2(const Vector2& v) = default;
  /**
   * @brief コピー代入演算子
   */
  Vector2& operator=(const Vector2& v) = default;
  /**
   * @brief 加算代入演算子
   */
  Vector2& operator+=(const Vector2& v);
  /**
   * @brief 減算代入演算子
   */
  Vector2& operator-=(const Vector2& v);
  /**
   * @brief 乗算代入演算子
   */
  Vector2& operator*=(float k);
  /**
   * @brief 除算代入演算子
   */
  Vector2& operator/=(float k);
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
  Vector2 Normalized() const;
  /**
   * @brief 内積
   */
  static float Dot(const Vector2& v, const Vector2& w);
  /**
   * @brief 外積
   */
  static float Cross(const Vector2& v, const Vector2& w);
};

/**
 * @brief 比較演算子
 */
inline bool operator==(const Vector2& v, const Vector2& w) {
  return (v.x == w.x && v.y == w.y);
}

/**
 * @brief 比較演算子
 */
inline bool operator!=(const Vector2& v, const Vector2& w) { return !(v == w); }

/**
 * @brief 加算演算子
 */
inline Vector2 operator+(const Vector2& v, const Vector2& w) {
  return Vector2(v) += w;
}

/**
 * @brief 減算演算子
 */
inline Vector2 operator-(const Vector2& v, const Vector2& w) {
  return Vector2(v) -= w;
}

/**
 * @brief 乗算演算子
 */
inline Vector2 operator*(const Vector2& v, float k) { return Vector2(v) *= k; }

/**
 * @brief 乗算演算子
 */
inline Vector2 operator*(float k, const Vector2& v) { return Vector2(v) *= k; }

/**
 * @brief 除算演算子
 */
inline Vector2 operator/(const Vector2& v, float k) { return Vector2(v) /= k; }

}  // namespace math
}  // namespace legend
#endif  // !LEGEND_MATH_VECTOR_2_H_
