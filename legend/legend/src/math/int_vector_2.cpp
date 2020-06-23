#include "src/math/int_vector_2.h"

namespace legend {
namespace math {
    
//定数
const IntVector2 IntVector2::kZeroVector = IntVector2(0, 0);
const IntVector2 IntVector2::kLeftVector = IntVector2(-1, 0);
const IntVector2 IntVector2::kRightVector = IntVector2(1, 0);
const IntVector2 IntVector2::kUpVector = IntVector2(0, 1);
const IntVector2 IntVector2::kDownVector = IntVector2(0, -1);
const IntVector2 IntVector2::kUnitVector = IntVector2(1, 1);

//デフォルトコンストラクタ
IntVector2::IntVector2() : x(0), y(0) {}

//コンストラクタ
IntVector2::IntVector2(i32 x, i32 y) : x(x), y(y) {}

//加算代入
IntVector2& IntVector2::operator+=(const IntVector2& v) {
  this->x += v.x;
  this->y += v.y;
  return *this;
}

//減算代入
IntVector2& IntVector2::operator-=(const IntVector2& v) {
  this->x -= v.x;
  this->y -= v.y;
  return *this;
}

//乗算代入
IntVector2& IntVector2::operator*=(i32 k) {
  this->x *= k;
  this->y *= k;
  return *this;
}

//除算代入
IntVector2& IntVector2::operator/=(i32 k) {
  this->x /= k;
  this->y /= k;
  return *this;
}

}  // namespace math
}  // namespace legend
