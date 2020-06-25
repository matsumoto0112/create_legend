#ifndef LEGEND_MATH_MATH_UTIL_H_
#define LEGEND_MATH_MATH_UTIL_H_

#include <cmath>

/**
 * @file math_util.h
 * @brief ���w�n�̃��[�e�B���e�B�֐�
 */

namespace legend {
namespace math {
namespace util {

/**
 * @brief sin(X)
 * @param X �P�ʂ̓��W�A��
 */
inline float Sin(float X) { return std::sinf(X); }
/**
 * @brief cos(X)
 * @param X �P�ʂ̓��W�A��
 */
inline float Cos(float X) { return std::cosf(X); }
/**
 * @brief tan(X)
 * @param X �P�ʂ̓��W�A��
 */
inline float Tan(float X) { return std::tanf(X); }
/**
 * @brief X�̃��[�g�����߂�
 */
inline float Sqrt(float X) { return std::sqrtf(X); }

/**
 * @brief 2�̗ݏ�ɃA���C�����g����
 * @return num��alignment�ŃA���C�����g���ꂽ�l��Ԃ�
 * @details ��)
 AlignPow2(16,256)��256��
 AlignPow2(300,256)��512��Ԃ�
 */
constexpr u32 AlignPow2(u32 num, u32 alignment) {
  return (num + (alignment - 1)) & ~(alignment - 1);
}

}  // namespace util
}  // namespace math
}  // namespace legend
#endif  // !LEGEND_MATH_MATH_UTIL_H_
