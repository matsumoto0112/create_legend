#include "src/util/color_4.h"

#include "src/math/math_util.h"

namespace legend {
namespace util {

//コンストラクタ
Color4::Color4() : r(0.0f), g(0.0f), b(0.0f), a(0.0f) {}

//コンストラクタ
Color4::Color4(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

//デストラクタ
Color4::~Color4() {}

//単項プラス演算子
Color4 Color4::operator+() { return *this; }

//単項マイナス演算子
Color4 Color4::operator-() { return *this * -1.0f; }

//加算
Color4& Color4::operator+=(const Color4& c) {
  this->r += c.r;
  this->g += c.g;
  this->b += c.b;
  this->a += c.a;

  return *this;
}

//減算
Color4& Color4::operator-=(const Color4& c) {
  this->r -= c.r;
  this->g -= c.g;
  this->b -= c.b;
  this->a -= c.a;

  return *this;
}

//乗算
Color4& Color4::operator*=(const Color4& c) {
  this->r *= c.r;
  this->g *= c.g;
  this->b *= c.b;
  this->a *= c.a;

  return *this;
}

//乗算
Color4& Color4::operator*=(float s) {
  this->r *= s;
  this->g *= s;
  this->b *= s;
  this->a *= s;

  return *this;
}

//除算
Color4& Color4::operator/=(float s) {
  s = 1.0f / s;
  this->r *= s;
  this->g *= s;
  this->b *= s;
  this->a *= s;

  return *this;
}

//配列で取得する
std::array<float, 4> Color4::Get() const { return {r, g, b, a}; }

// 01クランプ
Color4& Color4::Saturate() {
  r = math::util::Clamp(r, 0.0f, 1.0f);
  g = math::util::Clamp(g, 0.0f, 1.0f);
  b = math::util::Clamp(b, 0.0f, 1.0f);
  a = math::util::Clamp(a, 0.0f, 1.0f);

  return *this;
}

//補間
Color4 Color4::Lerp(const Color4& a, const Color4& b, float t) {
  t = math::util::Clamp(t, 0.0f, 1.0f);
  const float one_minus_t = 1.0f - t;
  const float nr = a.r * one_minus_t + b.r * t;
  const float ng = a.g * one_minus_t + b.g * t;
  const float nb = a.b * one_minus_t + b.b * t;
  const float na = a.a * one_minus_t + b.a * t;
  return Color4(nr, ng, nb, na);
}

//グレイスケール変換
Color4 Color4::GrayScale(const Color4& c) {
  return Color4(c.r * 0.2125f, c.g * 0.7154f, c.b * 0.0721f, c.a);
}

}  // namespace util
}  // namespace legend
