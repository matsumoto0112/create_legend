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

}  // namespace util
}  // namespace math
}  // namespace legend
#endif  // !LEGEND_MATH_MATH_UTIL_H_
