#include "src/math/vector_2.h"

#include "src/math/math_util.h"

namespace dx12_lib {
namespace math {
//定数
const Vector2 Vector2::kZeroVector = Vector2(0.0f, 0.0f);
const Vector2 Vector2::kLeftVector = Vector2(-1.0f, 0.0f);
const Vector2 Vector2::kRightVector = Vector2(1.0f, 0.0f);
const Vector2 Vector2::kUpVector = Vector2(0.0f, 1.0f);
const Vector2 Vector2::kDownVector = Vector2(0.0f, -1.0f);
const Vector2 Vector2::kUnitVector = Vector2(1.0f, 1.0f);

//デフォルトコンストラクタ
Vector2::Vector2() : x(0.0f), y(0.0f) {}

//コンストラクタ
Vector2::Vector2(float x, float y) : x(x), y(y) {}

//加算代入演算子
Vector2& Vector2::operator+=(const Vector2& v) {
  this->x += v.x;
  this->y += v.y;
  return *this;
}

//減算代入演算子
Vector2& Vector2::operator-=(const Vector2& v) {
  this->x -= v.x;
  this->y -= v.y;
  return *this;
}

//乗算代入演算子
Vector2& Vector2::operator*=(float k) {
  this->x *= k;
  this->y *= k;
  return *this;
}

//除算代入演算子
Vector2& Vector2::operator/=(float k) {
  this->x /= k;
  this->y /= k;
  return *this;
}

//大きさの二乗
float Vector2::MagnitudeSquared() const { return x * x + y * y; }

//大きさ
float Vector2::Magnitude() const { return util::Sqrt(MagnitudeSquared()); }

//正規化ベクトル
Vector2 Vector2::Normalized() const {
  const float magSq = MagnitudeSquared();
  // 0除算対策
  if (magSq > 0.0f) {
    const float mag = util::Sqrt(magSq);
    return Vector2(x / mag, y / mag);
  }
  return Vector2::kZeroVector;
}

//内積
float Vector2::Dot(const Vector2& v, const Vector2& w) {
  return v.x * w.x + v.y * w.y;
}

//外積
float Vector2::Cross(const Vector2& v, const Vector2& w) {
  return v.x * w.y - v.y * w.x;
}

}  // namespace math
}  // namespace dx12_lib