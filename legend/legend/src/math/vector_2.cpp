#include "src/math/vector_2.h"

#include "src/math/math_util.h"

namespace dx12_lib {
namespace math {
//�萔
const Vector2 Vector2::kZeroVector = Vector2(0.0f, 0.0f);
const Vector2 Vector2::kLeftVector = Vector2(-1.0f, 0.0f);
const Vector2 Vector2::kRightVector = Vector2(1.0f, 0.0f);
const Vector2 Vector2::kUpVector = Vector2(0.0f, 1.0f);
const Vector2 Vector2::kDownVector = Vector2(0.0f, -1.0f);
const Vector2 Vector2::kUnitVector = Vector2(1.0f, 1.0f);

//�f�t�H���g�R���X�g���N�^
Vector2::Vector2() : x(0.0f), y(0.0f) {}

//�R���X�g���N�^
Vector2::Vector2(float x, float y) : x(x), y(y) {}

//���Z������Z�q
Vector2& Vector2::operator+=(const Vector2& v) {
  this->x += v.x;
  this->y += v.y;
  return *this;
}

//���Z������Z�q
Vector2& Vector2::operator-=(const Vector2& v) {
  this->x -= v.x;
  this->y -= v.y;
  return *this;
}

//��Z������Z�q
Vector2& Vector2::operator*=(float k) {
  this->x *= k;
  this->y *= k;
  return *this;
}

//���Z������Z�q
Vector2& Vector2::operator/=(float k) {
  this->x /= k;
  this->y /= k;
  return *this;
}

//�傫���̓��
float Vector2::MagnitudeSquared() const { return x * x + y * y; }

//�傫��
float Vector2::Magnitude() const { return util::Sqrt(MagnitudeSquared()); }

//���K���x�N�g��
Vector2 Vector2::Normalized() const {
  const float magSq = MagnitudeSquared();
  // 0���Z�΍�
  if (magSq > 0.0f) {
    const float mag = util::Sqrt(magSq);
    return Vector2(x / mag, y / mag);
  }
  return Vector2::kZeroVector;
}

//����
float Vector2::Dot(const Vector2& v, const Vector2& w) {
  return v.x * w.x + v.y * w.y;
}

//�O��
float Vector2::Cross(const Vector2& v, const Vector2& w) {
  return v.x * w.y - v.y * w.x;
}

}  // namespace math
}  // namespace dx12_lib