#include "src/math/vector_4.h"

#include "src/math/math_util.h"

namespace legend {
namespace math {
//�萔
const Vector4 Vector4::kZeroVector = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
const Vector4 Vector4::kUnitVector = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

//�R���X�g���N�^
Vector4::Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
//�R���X�g���N�^
Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

//���Z������Z�q
Vector4& Vector4::operator+=(const Vector4& v) {
  this->x += v.x;
  this->y += v.y;
  this->z += v.z;
  this->w += v.w;
  return *this;
}

//���Z������Z�q
Vector4& Vector4::operator-=(const Vector4& v) {
  this->x -= v.x;
  this->y -= v.y;
  this->z -= v.z;
  this->w -= v.w;
  return *this;
}

//��Z������Z�q
Vector4& Vector4::operator*=(float k) {
  this->x *= k;
  this->y *= k;
  this->z *= k;
  this->w *= k;
  return *this;
}

//���Z������Z�q
Vector4& Vector4::operator/=(float k) {
  this->x /= k;
  this->y /= k;
  this->z /= k;
  this->z /= k;
  return *this;
}

//�傫���̓��
float Vector4::MagnitudeSquared() const {
  return x * x + y * y + z * z + w * w;
}

//�傫��
float Vector4::Magnitude() const { return util::Sqrt(MagnitudeSquared()); }

//���K���x�N�g��
Vector4 Vector4::Normalized() const {
  const float magSq = MagnitudeSquared();
  // 0���Z�΍�
  if (magSq > 0.0f) {
    const float mag = util::Sqrt(magSq);
    return Vector4(x / mag, y / mag, z / mag, w / mag);
  }
  return Vector4::kZeroVector;
}

//����
float Vector4::Dot(const Vector4& v, const Vector4& w) {
  return v.x * w.x + v.y * w.y + v.z * w.z + v.w * w.w;
}

}  // namespace math
}  // namespace legend