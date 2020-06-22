#include "src/math/vector_3.h"

#include "src/math/math_util.h"

namespace dx12_lib {
namespace math {
//定数
const Vector3 Vector3::kZeroVector = Vector3(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::kLeftVector = Vector3(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::kRightVector = Vector3(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::kUpVector = Vector3(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::kDownVector = Vector3(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::kForwardVector = Vector3(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::kBackwardVector = Vector3(0.0f, 0.0f, -1.0f);
const Vector3 Vector3::kUnitVector = Vector3(1.0f, 1.0f, 1.0f);

//デフォルトコンストラクタ
Vector3::Vector3() : x(0.0f), y(0.0f), z(0.0f) {}

//コンストラクタ
Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

//加算代入演算子
Vector3& Vector3::operator+=(const Vector3& v) {
  this->x += v.x;
  this->y += v.y;
  this->z += v.z;
  return *this;
}

//減算代入演算子
Vector3& Vector3::operator-=(const Vector3& v) {
  this->x -= v.x;
  this->y -= v.y;
  this->z -= v.z;
  return *this;
}

//乗算代入演算子
Vector3& Vector3::operator*=(float k) {
  this->x *= k;
  this->y *= k;
  this->z *= k;
  return *this;
}

//除算代入演算子
Vector3& Vector3::operator/=(float k) {
  this->x /= k;
  this->y /= k;
  this->z /= k;
  return *this;
}

//大きさの二乗
float Vector3::MagnitudeSquared() const { return x * x + y * y + z * z; }

//大きさ
float Vector3::Magnitude() const { return util::Sqrt(MagnitudeSquared()); }

//正規化ベクトル
Vector3 Vector3::Normalized() const {
  const float magSq = MagnitudeSquared();
  // 0除算対策
  if (magSq > 0.0f) {
    const float mag = util::Sqrt(magSq);
    return Vector3(x / mag, y / mag, z / mag);
  }
  return Vector3::kZeroVector;
}

//内積
float Vector3::Dot(const Vector3& v, const Vector3& w) {
  return v.x * w.x + v.y * w.y + v.z * w.z;
}

//外積
Vector3 Vector3::Cross(const Vector3& v, const Vector3& w) {
  return Vector3(v.y * w.z - v.z * w.y, v.z * w.x - v.x * w.z,
                 v.x * w.y - v.y * w.x);
}

}  // namespace math
}  // namespace dx12_lib