#include "src/math/vector_3.h"

#include "src/math/math_util.h"

namespace dx12_lib {
namespace math {
//�萔
const Vector3 Vector3::kZeroVector = Vector3(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::kLeftVector = Vector3(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::kRightVector = Vector3(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::kUpVector = Vector3(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::kDownVector = Vector3(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::kForwardVector = Vector3(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::kBackwardVector = Vector3(0.0f, 0.0f, -1.0f);
const Vector3 Vector3::kUnitVector = Vector3(1.0f, 1.0f, 1.0f);

//�f�t�H���g�R���X�g���N�^
Vector3::Vector3() : x(0.0f), y(0.0f), z(0.0f) {}

//�R���X�g���N�^
Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

//���Z������Z�q
Vector3& Vector3::operator+=(const Vector3& v) {
  this->x += v.x;
  this->y += v.y;
  this->z += v.z;
  return *this;
}

//���Z������Z�q
Vector3& Vector3::operator-=(const Vector3& v) {
  this->x -= v.x;
  this->y -= v.y;
  this->z -= v.z;
  return *this;
}

//��Z������Z�q
Vector3& Vector3::operator*=(float k) {
  this->x *= k;
  this->y *= k;
  this->z *= k;
  return *this;
}

//���Z������Z�q
Vector3& Vector3::operator/=(float k) {
  this->x /= k;
  this->y /= k;
  this->z /= k;
  return *this;
}

//�傫���̓��
float Vector3::MagnitudeSquared() const { return x * x + y * y + z * z; }

//�傫��
float Vector3::Magnitude() const { return util::Sqrt(MagnitudeSquared()); }

//���K���x�N�g��
Vector3 Vector3::Normalized() const {
  const float magSq = MagnitudeSquared();
  // 0���Z�΍�
  if (magSq > 0.0f) {
    const float mag = util::Sqrt(magSq);
    return Vector3(x / mag, y / mag, z / mag);
  }
  return Vector3::kZeroVector;
}

//����
float Vector3::Dot(const Vector3& v, const Vector3& w) {
  return v.x * w.x + v.y * w.y + v.z * w.z;
}

//�O��
Vector3 Vector3::Cross(const Vector3& v, const Vector3& w) {
  return Vector3(v.y * w.z - v.z * w.y, v.z * w.x - v.x * w.z,
                 v.x * w.y - v.y * w.x);
}

}  // namespace math
}  // namespace dx12_lib