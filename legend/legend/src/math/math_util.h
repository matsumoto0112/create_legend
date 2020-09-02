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

//! �~���� ��
static inline const float PI = std::acosf(-1);
//! 2 * ��
static inline const float PI2 = 2 * PI;

//! �x���烉�W�A���ɕϊ�����W��
static inline const float DEG_2_RAD = PI / 180.0f;

//! ���W�A������x�ɕϊ�����W��
static inline const float RAD_2_DEG = 180.0f / PI;

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
 * @brief X�̃��[�g�����߂�
 */
inline float Sqrt(float X) { return std::sqrtf(X); }

/**
 * @brief ��Βl
 */
inline float Abs(float X) { return std::abs(X); }

/**
 * @brief X^P���v�Z����
 */
inline float Pow(float X, float P) { return std::pow(X, P); }

/**
 * @brief x��y�̂����������ق���Ԃ�
 */
template <class T>
inline constexpr T Min(T x, T y) {
  return std::min(x, y);
}

/**
 * @brief x��y�̂����傫���ق���Ԃ�
 */
template <class T>
inline constexpr T Max(T x, T y) {
  return std::max(x, y);
}

/**
 * @brief X�̒l�̐����𔻒肷��
 * @param X ���肷�鐔
 * @param eps ���e�덷
 * @return
 * X��eps���傫���Ȃ�1�A-eps��菬�����Ȃ�-1�A-eps�ȏ�eps�ȉ��Ȃ�0��Ԃ�
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
 * @brief 2�̗ݏ�ɃA���C�����g����
 * @return num��alignment�ŃA���C�����g���ꂽ�l��Ԃ�
 * @details ��)
 AlignPow2(16,256)��256��
 AlignPow2(300,256)��512��Ԃ�
 */
constexpr u32 AlignPow2(u32 num, u32 alignment) {
  return (num + (alignment - 1)) & ~(alignment - 1);
}

/**
 * @brief value��min�ȏ�max�������ǂ������肷��
 * @return ����𖞂����Ă�����true��Ԃ�
 */
template <typename T>
constexpr bool IsInRange(T value, T min, T max) {
  return min <= value && value < max;
}

/**
 * @brief �͈͓��ɒl��}����
 * @param t �N�����v����l
 * @param min �ŏ��l
 * @param max �ő�l
 * @return �N�����v���ꂽ�l
 */
constexpr float Clamp(float t, float min, float max) {
  return std::clamp(t, min, max);
}

/**
 * @brief �������w�肵�Ďl�̌ܓ�����
 * @param value �l�̌ܓ�����l
 * @param digit �w�肷�錅
 * @return ����ɂ���ē���ꂽ�l
 * @details �� value=0.123456,digit=1�̎��Aans=0.1(�������ʂ܂łŎl�̌ܓ�)
 value=123.4567,digit=2�̎��Aans=123.46
 */
inline float Round(float value, i32 digit = 0) {
  // 10^n�������Ďl�̌ܓ����A10^n�Ŋ��邱�Ƃœ��������߂�
  const float p = std::powf(10, static_cast<float>(digit));
  const float tmp = value * p;
  const float res = std::round(tmp);
  return res / p;
}

/**
 * @brief ���
 * @param a �p�����[�^1
 * @param b �p�����[�^2
 * @param t ��ԌW�� 0~1�ɃN�����v����Ďg�p�����
 * @return a��b���ԌW��t�ŕ�Ԃ��ꂽ�l��Ԃ�
 */
inline constexpr float Lerp(float a, float b, float t) {
  t = Clamp(t, 0.0f, 1.0f);
  return b * t + a * (1.0f - t);
}

}  // namespace util
}  // namespace math
}  // namespace legend
#endif  // !LEGEND_MATH_MATH_UTIL_H_
