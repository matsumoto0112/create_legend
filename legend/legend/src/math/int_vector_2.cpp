#include "src/math/int_vector_2.h"

namespace legend {
namespace math {
    
//�萔
const IntVector2 IntVector2::kZeroVector = IntVector2(0, 0);
const IntVector2 IntVector2::kLeftVector = IntVector2(-1, 0);
const IntVector2 IntVector2::kRightVector = IntVector2(1, 0);
const IntVector2 IntVector2::kUpVector = IntVector2(0, 1);
const IntVector2 IntVector2::kDownVector = IntVector2(0, -1);
const IntVector2 IntVector2::kUnitVector = IntVector2(1, 1);

//�f�t�H���g�R���X�g���N�^
IntVector2::IntVector2() : x(0), y(0) {}

//�R���X�g���N�^
IntVector2::IntVector2(i32 x, i32 y) : x(x), y(y) {}

//���Z���
IntVector2& IntVector2::operator+=(const IntVector2& v) {
  this->x += v.x;
  this->y += v.y;
  return *this;
}

//���Z���
IntVector2& IntVector2::operator-=(const IntVector2& v) {
  this->x -= v.x;
  this->y -= v.y;
  return *this;
}

//��Z���
IntVector2& IntVector2::operator*=(i32 k) {
  this->x *= k;
  this->y *= k;
  return *this;
}

//���Z���
IntVector2& IntVector2::operator/=(i32 k) {
  this->x /= k;
  this->y /= k;
  return *this;
}

}  // namespace math
}  // namespace legend
