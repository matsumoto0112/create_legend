#ifndef LEGEND_UTIL_COLOR_4_H_
#define LEGEND_UTIL_COLOR_4_H_

/**
 * @file Color4.h
 * @brief RGBAの色管理クラス定義
 */

namespace legend {
namespace util {

/**
 * @class Color4
 * @brief 色管理クラス
 */
class Color4 {
 public:
  //! r成分
  float r;
  //! g成分
  float g;
  //! b成分
  float b;
  //! a成分
  float a;

 public:
  //! 黒
  static const Color4 BLACK;
  //! 白
  static const Color4 WHITE;

 public:
  /**
   * @brief コンストラクタ
   */
  Color4();
  /**
   * @brief コンストラクタ
   * @param r r成分(0.0～1.0)
   * @param g g成分(0.0～1.0)
   * @param b b成分(0.0～1.0)
   * @param a a成分(0.0～1.0)
   */
  Color4(float r, float g, float b, float a);
  /**
   * @brief デストラクタ
   */
  ~Color4();
  /**
   * @brief コピーコンストラクタ
   */
  Color4(const Color4& c) = default;
  /**
   * @brief 代入演算子
   */
  Color4& operator=(const Color4& c) = default;
  /**
   * v単項プラス演算子
   */
  Color4 operator+();
  /**
   * @brief 単項マイナス演算子
   */
  Color4 operator-();
  /**
   * @brief 加算代入演算子
   */
  Color4& operator+=(const Color4& c);
  /**
   * @brief 減算代入演算子
   */
  Color4& operator-=(const Color4& c);
  /**
   * @brief 乗算代入演算子
   */
  Color4& operator*=(const Color4& c);
  /**
   * @brief 乗算代入演算子
   */
  Color4& operator*=(float s);
  /**
   * @brief 除算代入演算子
   */
  Color4& operator/=(float s);

  /**
   * @brief 色を配列で取得
   * @return r,g,b,aの順に格納された配列
   */
  std::array<float, 4> Get() const;
  /**
   * @brief 0～1の間にクランプする
   */
  Color4& Saturate();
  /**
   * @brief 補間
   * @param a 色1
   * @param b 色2
   * @param t 補間係数
   */
  static Color4 Lerp(const Color4& a, const Color4& b, float t);
  /**
   * @brief グレースケールに変換する
   */
  static Color4 GrayScale(const Color4& c);
};

/**
 * @brief 等価比較
 */
inline bool operator==(const Color4& c1, const Color4& c2) {
  return (c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a);
}

/**
 * @brief 等価比較
 */
inline bool operator!=(const Color4& c1, const Color4& c2) {
  return !(c1 == c2);
}

/**
 * @brief 加算
 */
inline Color4 operator+(const Color4& c1, const Color4& c2) {
  return Color4(c1) += c2;
}

/**
 * @brief 減算
 */
inline Color4 operator-(const Color4& c1, const Color4& c2) {
  return Color4(c1) -= c2;
}

/**
 * @brief 乗算
 */
inline Color4 operator*(const Color4& c1, const Color4& c2) {
  return Color4(c1) *= c2;
}

/**
 * @brief 乗算
 */
inline Color4 operator*(const Color4& c, float s) { return Color4(c) *= s; }

/**
 * @brief 乗算
 */
inline Color4 operator*(float s, const Color4& c) { return Color4(c) *= s; }

/**
 * @brief 除算
 */
inline Color4 operator/(const Color4& c, float s) { return Color4(c) /= s; }

}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_COLOR_4_H_
