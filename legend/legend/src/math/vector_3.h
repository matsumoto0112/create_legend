#ifndef LEGEND_MATH_VECTOR_3_H_
#define LEGEND_MATH_VECTOR_3_H_

/**
 * @file vector_3.h
 * @brief 三次元ベクトルクラス定義
 */
namespace legend {
namespace math {

/**
 * @brief 三次元ベクトルクラス
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
  //! ゼロベクトル
  static const Vector3 kZeroVector;
  //! 左ベクトル
  static const Vector3 kLeftVector;
  //! 右ベクトル
  static const Vector3 kRightVector;
  //! 上ベクトル
  static const Vector3 kUpVector;
  //! 下ベクトル
  static const Vector3 kDownVector;
  //! 前ベクトル
  static const Vector3 kForwardVector;
  //! 後ベクトル
  static const Vector3 kBackwardVector;
  //! 単位ベクトル
  static const Vector3 kUnitVector;

 public:
  /**
   * @brief デフォルトコンストラクタ
   */
  Vector3();
  /**
   * @brief コンストラクタ
   */
  Vector3(float x, float y, float z);
  /**
   * @brief コピーコンストラクタ
   */
  Vector3(const Vector3& v) = default;
  /**
   * @brief コピー代入演算子
   */
  Vector3& operator=(const Vector3& v) = default;
  /**
   * @brief 加算代入演算子
   */
  Vector3& operator+=(const Vector3& v);
  /**
   * @brief 減算代入演算子
   */
  Vector3& operator-=(const Vector3& v);
  /**
   * @brief 乗算代入演算子
   */
  Vector3& operator*=(float k);
  /**
   * @brief 除算代入演算子
   */
  Vector3& operator/=(float k);
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
  Vector3 Normalized() const;
  /**
   * @brief 内積
   */
  static float Dot(const Vector3& v, const Vector3& w);
  /**
   * @brief 外積
   */
  static Vector3 Cross(const Vector3& v, const Vector3& w);
};

/**
 * @brief 比較演算子
 */
inline bool operator==(const Vector3& v, const Vector3& w) {
  return (v.x == w.x && v.y == w.y && v.z == w.z);
}

/**
 * @brief 比較演算子
 */
inline bool operator!=(const Vector3& v, const Vector3& w) { return !(v == w); }

/**
 * @brief 加算演算子
 */
inline Vector3 operator+(const Vector3& v, const Vector3& w) {
  return Vector3(v) += w;
}

/**
 * @brief 減算演算子
 */
inline Vector3 operator-(const Vector3& v, const Vector3& w) {
  return Vector3(v) -= w;
}

/**
 * @brief 乗算演算子
 */
inline Vector3 operator*(const Vector3& v, float k) { return Vector3(v) *= k; }

/**
 * @brief 乗算演算子
 */
inline Vector3 operator*(float k, const Vector3& v) { return Vector3(v) *= k; }

/**
 * @brief 除算演算子
 */
inline Vector3 operator/(const Vector3& v, float k) { return Vector3(v) /= k; }

}  // namespace math
}  // namespace legend
#endif  // !LEGEND_MATH_VECTOR_3_H_
