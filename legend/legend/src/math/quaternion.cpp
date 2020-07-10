#include "src/math/quaternion.h"

#include "src/math/math_util.h"

namespace legend {
namespace math {

const Quaternion Quaternion::IDENTITY = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

//コンストラクタ
Quaternion::Quaternion() {}

//コンストラクタ
Quaternion::Quaternion(float x, float y, float z, float w)
    : x(x), y(y), z(z), w(w) {}

Quaternion::Quaternion(const Vector3& nv, float angle) {
  const float half_theta = angle * 0.5f;
  const float sin = util::Sin(half_theta);
  const float cos = util::Cos(half_theta);

  this->x = nv.x * sin;
  this->y = nv.y * sin;
  this->z = nv.z * sin;
  this->w = cos;
}

//デストラクタ
Quaternion::~Quaternion() {}

//正規化クォータニオンを取得する
Quaternion Quaternion::Normalized() const {
  Quaternion res(*this);
  const float len =
      util::Sqrt(res.x * res.x + res.y * res.y + res.z * res.z + res.w * res.w);
  if (len) {
    res.x /= len;
    res.y /= len;
    res.z /= len;
    res.w /= len;
  }
  return res;
}

Quaternion Quaternion::Conjugate() const { return Quaternion(-x, -y, -z, w); }

Quaternion Quaternion::FromEular(float roll, float pitch, float yaw) {
  const float c1 = util::Cos(roll / 2.0f);
  const float s1 = util::Sin(roll / 2.0f);
  const float c2 = util::Cos(pitch / 2.0f);
  const float s2 = util::Sin(pitch / 2.0f);
  const float c3 = util::Cos(yaw / 2.0f);
  const float s3 = util::Sin(yaw / 2.0f);
  return Quaternion(s1 * c2 * c3 + c1 * s2 * s3, c1 * s2 * c3 - s1 * c2 * s3,
                    c1 * c2 * s3 - s1 * s2 * c3, c1 * c2 * c3 + s1 * s2 * s3);
}

Quaternion Quaternion::FromEular(const Vector3& v) {
  return FromEular(v.x, v.y, v.z);
}

Vector3 Quaternion::ToEular(const Quaternion& q) {
  constexpr float EPSILON = 0.001f;

  const float x = q.x;
  const float y = q.y;
  const float z = q.z;
  const float w = q.w;
  const float x2 = x * x;
  const float y2 = y * y;
  const float z2 = z * z;
  const float xy = x * y;
  const float xz = x * z;
  const float yz = y * z;
  const float wx = w * x;
  const float wy = w * y;
  const float wz = w * z;

  const float m00 = 1.0f - (2.0f * y2) - (2.0f * z2);
  const float m01 = (2.0f * xy) + (2.0f * wz);
  const float m10 = (2.0f * xy) - (2.0f * wz);
  const float m11 = 1.0f - (2.0f * x2) - (2.0f * z2);
  const float m20 = (2.f * xz) + (2.0f * wy);
  const float m21 = (2.0f * yz) - (2.0f * wx);
  const float m22 = 1.0f - (2.0f * x2) - (2.0f * y2);

  float tx, ty, tz;
  if (util::Abs(m21 - 1.0f) < EPSILON) {
    tx = -util::PI / 2.0f;
    ty = 0.0f;
    tz = util::Atan2(m10, m00);
  } else if (util::Abs(m21 + 1.0f) < EPSILON) {
    tx = util::PI / 2.0f;
    ty = 0.0f;
    tz = util::Atan2(m10, m00);
  } else {
    tx = util::Asin(-m21);
    ty = util::Atan2(m20, m22);
    tz = util::Atan2(m01, m11);
  }

  return Vector3(tx, ty, tz);
}

Quaternion operator*(const Quaternion& q1, const Quaternion& q2) {
  const float x = q1.w * q2.x - q1.z * q2.y + q1.y * q2.z + q1.x * q2.w;
  const float y = q1.z * q2.x + q1.w * q2.y - q1.x * q2.z + q1.y * q2.w;
  const float z = -q1.y * q2.x + q1.x * q2.y + q1.w * q2.z + q1.z * q2.w;
  const float w = -q1.x * q2.x - q1.y * q2.y - q1.z * q2.z + q1.w * q2.w;
  return Quaternion(x, y, z, w);
}

Vector3 operator*(const Quaternion& q, const Vector3& v) {
  const Quaternion r(v.x, v.y, v.z, 0.0f);
  const Quaternion res = q * r * q.Conjugate();
  return Vector3(res.x, res.y, res.z);
}

}  // namespace math
}  // namespace legend
