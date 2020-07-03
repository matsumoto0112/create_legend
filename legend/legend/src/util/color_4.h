#ifndef LEGEND_UTIL_COLOR_4_H_
#define LEGEND_UTIL_COLOR_4_H_

/**
 * @file Color4.h
 * @brief RGBA�̐F�Ǘ��N���X��`
 */

namespace legend {
namespace util {

/**
 * @class Color4
 * @brief �F�Ǘ��N���X
 */
class Color4 {
 public:
  //! r����
  float r;
  //! g����
  float g;
  //! b����
  float b;
  //! a����
  float a;

 public:
  //! ��
  static const Color4 BLACK;
  //! ��
  static const Color4 WHITE;

 public:
  /**
   * @brief �R���X�g���N�^
   */
  Color4();
  /**
   * @brief �R���X�g���N�^
   * @param r r����(0.0�`1.0)
   * @param g g����(0.0�`1.0)
   * @param b b����(0.0�`1.0)
   * @param a a����(0.0�`1.0)
   */
  Color4(float r, float g, float b, float a);
  /**
   * @brief �f�X�g���N�^
   */
  ~Color4();
  /**
   * @brief �R�s�[�R���X�g���N�^
   */
  Color4(const Color4& c) = default;
  /**
   * @brief ������Z�q
   */
  Color4& operator=(const Color4& c) = default;
  /**
   * v�P���v���X���Z�q
   */
  Color4 operator+();
  /**
   * @brief �P���}�C�i�X���Z�q
   */
  Color4 operator-();
  /**
   * @brief ���Z������Z�q
   */
  Color4& operator+=(const Color4& c);
  /**
   * @brief ���Z������Z�q
   */
  Color4& operator-=(const Color4& c);
  /**
   * @brief ��Z������Z�q
   */
  Color4& operator*=(const Color4& c);
  /**
   * @brief ��Z������Z�q
   */
  Color4& operator*=(float s);
  /**
   * @brief ���Z������Z�q
   */
  Color4& operator/=(float s);

  /**
   * @brief �F��z��Ŏ擾
   * @return r,g,b,a�̏��Ɋi�[���ꂽ�z��
   */
  std::array<float, 4> Get() const;
  /**
   * @brief 0�`1�̊ԂɃN�����v����
   */
  Color4& Saturate();
  /**
   * @brief ���
   * @param a �F1
   * @param b �F2
   * @param t ��ԌW��
   */
  static Color4 Lerp(const Color4& a, const Color4& b, float t);
  /**
   * @brief �O���[�X�P�[���ɕϊ�����
   */
  static Color4 GrayScale(const Color4& c);
};

/**
 * @brief ������r
 */
inline bool operator==(const Color4& c1, const Color4& c2) {
  return (c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a);
}

/**
 * @brief ������r
 */
inline bool operator!=(const Color4& c1, const Color4& c2) {
  return !(c1 == c2);
}

/**
 * @brief ���Z
 */
inline Color4 operator+(const Color4& c1, const Color4& c2) {
  return Color4(c1) += c2;
}

/**
 * @brief ���Z
 */
inline Color4 operator-(const Color4& c1, const Color4& c2) {
  return Color4(c1) -= c2;
}

/**
 * @brief ��Z
 */
inline Color4 operator*(const Color4& c1, const Color4& c2) {
  return Color4(c1) *= c2;
}

/**
 * @brief ��Z
 */
inline Color4 operator*(const Color4& c, float s) { return Color4(c) *= s; }

/**
 * @brief ��Z
 */
inline Color4 operator*(float s, const Color4& c) { return Color4(c) *= s; }

/**
 * @brief ���Z
 */
inline Color4 operator/(const Color4& c, float s) { return Color4(c) /= s; }

}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_COLOR_4_H_
