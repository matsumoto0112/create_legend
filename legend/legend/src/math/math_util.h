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
 * @brief Xのルートを求める
 */
inline float Sqrt(float X) { return std::sqrtf(X); }

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
 * @brief 範囲内に値を抑える
 * @param t クランプする値
 * @param min 最小値
 * @param max 最大値
 * @return クランプされた値
 */
constexpr float Clamp(float t, float min, float max) {
  if (t < min) t = min;
  if (t > max) t = max;
  return t;
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

}  // namespace util
}  // namespace math
}  // namespace legend
#endif  // !LEGEND_MATH_MATH_UTIL_H_
