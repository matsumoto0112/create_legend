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
inline const float Sin(float X) { return std::sinf(X); }
/**
 * @brief cos(X)
 * @param X �P�ʂ̓��W�A��
 */
inline const float Cos(float X) { return std::cosf(X); }
/**
 * @brief tan(X)
 * @param X �P�ʂ̓��W�A��
 */
inline const float Tan(float X) { return std::tanf(X); }
/**
 * @brief X�̃��[�g�����߂�
 */
inline const float Sqrt(float X) { return std::sqrtf(X); }

}  // namespace util
}  // namespace math
}  // namespace legend
#endif  // !LEGEND_MATH_MATH_UTIL_H_
