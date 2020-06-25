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
