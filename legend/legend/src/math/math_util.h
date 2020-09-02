#ifndef LEGEND_MATH_MATH_UTIL_H_
#define LEGEND_MATH_MATH_UTIL_H_

#include <cmath>

/**
 * @file math_util.h
 * @brief 数学系のユーティリティ関数
 */

namespace legend {
namespace math {
namespace util {

//! 円周率 π
static inline const float PI = std::acosf(-1);
//! 2 * π
static inline const float PI2 = 2 * PI;

//! 度からラジアンに変換する係数
static inline const float DEG_2_RAD = PI / 180.0f;

//! ラジアンから度に変換する係数
static inline const float RAD_2_DEG = 180.0f / PI;

/**
 * @brief sin(X)
 * @param X 単位はラジアン
 */
inline float Sin(float X) { return std::sinf(X); }
/**
 * @brief cos(X)
 * @param X 単位はラジアン
 */
inline float Cos(float X) { return std::cosf(X); }
/**
 * @brief tan(X)
 * @param X 単位はラジアン
 */
inline float Tan(float X) { return std::tanf(X); }

/**
 * @brief ASin(X)
 */
inline float Asin(float X) { return std::asin(X); }
/**
 * @brief ACos(X)
 */
inline float Acos(float X) { return std::acos(X); }
/**
 * @brief atan2(Y,X)
 */
inline float Atan2(float Y, float X) { return std::atan2(Y, X); }
/**
 * @brief Xのルートを求める
 */
inline float Sqrt(float X) { return std::sqrtf(X); }

/**
 * @brief 絶対値
 */
inline float Abs(float X) { return std::abs(X); }

/**
 * @brief X^Pを計算する
 */
inline float Pow(float X, float P) { return std::pow(X, P); }

/**
 * @brief xとyのうち小さいほうを返す
 */
template <class T>
inline constexpr T Min(T x, T y) {
  return std::min(x, y);
}

/**
 * @brief xとyのうち大きいほうを返す
 */
template <class T>
inline constexpr T Max(T x, T y) {
  return std::max(x, y);
}

/**
 * @brief Xの値の正負を判定する
 * @param X 判定する数
 * @param eps 許容誤差
 * @return
 * Xがepsより大きいなら1、-epsより小さいなら-1、-eps以上eps以下なら0を返す
 */
inline constexpr i32 Sign(float X, float eps = 0.01f) {
  if (X > eps) {
    return 1;
  } else if (X < -eps) {
    return -1;
  } else {
    return 0;
  }
}

/**
 * @brief 2の累乗にアライメントする
 * @return numをalignmentでアライメントされた値を返す
 * @details 例)
 AlignPow2(16,256)は256を
 AlignPow2(300,256)は512を返す
 */
constexpr u32 AlignPow2(u32 num, u32 alignment) {
  return (num + (alignment - 1)) & ~(alignment - 1);
}

/**
 * @brief valueがmin以上max未満かどうか判定する
 * @return 上限を満たしていたらtrueを返す
 */
template <typename T>
constexpr bool IsInRange(T value, T min, T max) {
  return min <= value && value < max;
}

/**
 * @brief 範囲内に値を抑える
 * @param t クランプする値
 * @param min 最小値
 * @param max 最大値
 * @return クランプされた値
 */
constexpr float Clamp(float t, float min, float max) {
  return std::clamp(t, min, max);
}

/**
 * @brief 桁数を指定して四捨五入する
 * @param value 四捨五入する値
 * @param digit 指定する桁
 * @return 操作によって得られた値
 * @details 例 value=0.123456,digit=1の時、ans=0.1(小数第一位までで四捨五入)
 value=123.4567,digit=2の時、ans=123.46
 */
inline float Round(float value, i32 digit = 0) {
  // 10^nをかけて四捨五入し、10^nで割ることで答えを求める
  const float p = std::powf(10, static_cast<float>(digit));
  const float tmp = value * p;
  const float res = std::round(tmp);
  return res / p;
}

/**
 * @brief 補間
 * @param a パラメータ1
 * @param b パラメータ2
 * @param t 補間係数 0~1にクランプされて使用される
 * @return aとbを補間係数tで補間された値を返す
 */
inline constexpr float Lerp(float a, float b, float t) {
  t = Clamp(t, 0.0f, 1.0f);
  return b * t + a * (1.0f - t);
}

}  // namespace util
}  // namespace math
}  // namespace legend
#endif  // !LEGEND_MATH_MATH_UTIL_H_
