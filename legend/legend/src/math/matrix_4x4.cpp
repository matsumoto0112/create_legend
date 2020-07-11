#include "src/math/matrix_4x4.h"

#include "src/math/math_util.h"

namespace legend {
namespace math {
//定数宣言
const Matrix4x4 Matrix4x4::kIdentity =
    Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
              0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

const Matrix4x4 Matrix4x4::kZero =
    Matrix4x4(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
              0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

//コンストラクタ
Matrix4x4::Matrix4x4() : Matrix4x4(kIdentity) {}

//コンストラクタ
Matrix4x4::Matrix4x4(float m11, float m12, float m13, float m14, float m21,
                     float m22, float m23, float m24, float m31, float m32,
                     float m33, float m34, float m41, float m42, float m43,
                     float m44) {
  m[0] = {m11, m12, m13, m14};
  m[1] = {m21, m22, m23, m24};
  m[2] = {m31, m32, m33, m34};
  m[3] = {m41, m42, m43, m44};
}
//コンストラクタ
Matrix4x4::Matrix4x4(const std::array<std::array<float, 4>, 4>& m) : m(m) {}
//コンストラクタ
Matrix4x4::Matrix4x4(const Matrix4x4& m) : m(m.m) {}

//代入
Matrix4x4& Matrix4x4::operator=(const Matrix4x4& mat) & {
  this->m = mat.m;
  return *this;
}
//代入
Matrix4x4& Matrix4x4::operator=(Matrix4x4&& mat) & noexcept {
  this->m = mat.m;
  return *this;
}
//単項+
Matrix4x4 Matrix4x4::operator+() {
  Matrix4x4 mat(m);
  return mat;
}
//単項-
Matrix4x4 Matrix4x4::operator-() {
  Matrix4x4 mat(m);
  mat *= -1;
  return mat;
}

//加算代入
Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& mat) {
  *this = *this + mat;
  return *this;
}
//減算代入
Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& mat) {
  *this = *this - mat;
  return *this;
}
//乗算代入
Matrix4x4& Matrix4x4::operator*=(float k) {
  *this = *this * k;
  return *this;
}
//乗算代入
Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& mat) {
  *this = *this * mat;
  return *this;
}
//減算代入
Matrix4x4& Matrix4x4::operator/=(float k) {
  *this = *this / k;
  return *this;
}

//移動行列の作成
Matrix4x4 Matrix4x4::CreateTranslate(const Vector3& v) {
  return Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                   1.0f, 0.0f, v.x, v.y, v.z, 1.0f);
}

// X軸に回転する回転行列を作成
Matrix4x4 Matrix4x4::CreateRotationX(float rad) {
  const float sin = util::Sin(rad);
  const float cos = util::Cos(rad);
  return Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, cos, sin, 0.0f, 0.0f, -sin,
                   cos, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}
// Y軸に回転する回転行列を作成
Matrix4x4 Matrix4x4::CreateRotationY(float rad) {
  const float sin = util::Sin(rad);
  const float cos = util::Cos(rad);
  return Matrix4x4(cos, 0.0f, -sin, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, sin, 0.0f,
                   cos, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

// Z軸に回転する回転行列を作成
Matrix4x4 Matrix4x4::CreateRotationZ(float rad) {
  const float sin = util::Sin(rad);
  const float cos = util::Cos(rad);
  return Matrix4x4(cos, sin, 0.0f, 0.0f, -sin, cos, 0.0f, 0.0f, 0.0f, 0.0f,
                   1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

// 3軸に回転する回転行列を作成
Matrix4x4 Matrix4x4::CreateRotation(const Vector3& r) {
  const Matrix4x4 mx(CreateRotationX(r.x));
  const Matrix4x4 my(CreateRotationY(r.y));
  const Matrix4x4 mz(CreateRotationZ(r.z));
  return mx * my * mz;
}
//スケーリング行列を作成
Matrix4x4 Matrix4x4::CreateScale(const Vector3& s) {
  return Matrix4x4(s.x, 0.0f, 0.0f, 0.0f, 0.0f, s.y, 0.0f, 0.0f, 0.0f, 0.0f,
                   s.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

//ビュー行列を作成
Matrix4x4 Matrix4x4::CreateView(const Vector3& eye, const Vector3& at,
                                const Vector3& up) {
  const Vector3 zaxis = (at - eye).Normalized();
  const Vector3 xaxis = Vector3::Cross(up, zaxis).Normalized();
  const Vector3 yaxis = Vector3::Cross(zaxis, xaxis).Normalized();
  return Matrix4x4(xaxis.x, yaxis.x, zaxis.x, 0.0f, xaxis.y, yaxis.y, zaxis.y,
                   0.0f, xaxis.z, yaxis.z, zaxis.z, 0.0f,
                   -Vector3::Dot(xaxis, eye), -Vector3::Dot(yaxis, eye),
                   -Vector3::Dot(zaxis, eye), 1.0f);
}

//プロジェクション行列を作成
Matrix4x4 Matrix4x4::CreateProjection(float fov_y, float aspect, float nearZ,
                                      float farZ) {
  MY_ASSERTION(aspect != 0.0f, L"0除算が発生しました");
  MY_ASSERTION(util::Tan(fov_y / 2.0f) != 0.0f, L"0除算が発生しました");

  const float y_scale = 1.0f / util::Tan(fov_y / 2.0f);
  const float x_scale = y_scale / aspect;
  const float sub_z = farZ - nearZ;
  MY_ASSERTION(sub_z != 0.0f, L"0除算が発生しました");

  return Matrix4x4(x_scale, 0.0f, 0.0f, 0.0f, 0.0f, y_scale, 0.0f, 0.0f, 0.0f,
                   0.0f, farZ / sub_z, 1.0f, 0.0f, 0.0f, -nearZ * farZ / sub_z,
                   0.0f);
}

//平行投影行列を作成
Matrix4x4 Matrix4x4::CreateOrthographic(const Vector2& screen_size) {
  return Matrix4x4(2.0f / screen_size.x, 0.0f, 0.0f, 0.0f, 0.0f,
                   -2.0f / screen_size.y, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                   -1.0f, 1.0f, 0.0f, 1.0f);
}
//転置行列
Matrix4x4 Matrix4x4::Transpose() const {
  return Matrix4x4(m[0][0], m[1][0], m[2][0], m[3][0], m[0][1], m[1][1],
                   m[2][1], m[3][1], m[0][2], m[1][2], m[2][2], m[3][2],
                   m[0][3], m[1][3], m[2][3], m[3][3]);
}
//行列式
float Matrix4x4::Determinant() const {
  float buf = 0.0f;
  Matrix4x4 m(*this);
  for (i32 i = 0; i < 4; i++) {
    for (i32 j = 0; j < 4; j++) {
      if (i < j) {
        buf = m.m[j][i] / m.m[i][i];
        for (i32 k = 0; k < 4; k++) {
          m.m[j][k] -= m.m[i][k] * buf;
        }
      }
    }
  }
  float det = 1.0f;
  for (i32 i = 0; i < 4; i++) {
    det *= m.m[i][i];
  }
  return det;
}
//逆行列
Matrix4x4 Matrix4x4::Inverse() const {
  Matrix4x4 res = Matrix4x4::kIdentity;
  Matrix4x4 m(*this);
  float buf = 0.0;
  for (i32 i = 0; i < 4; i++) {
    buf = 1.0f / m.m[i][i];
    for (i32 j = 0; j < 4; j++) {
      m.m[i][j] *= buf;
      res.m[i][j] *= buf;
    }
    for (i32 j = 0; j < 4; j++) {
      if (i != j) {
        buf = m.m[j][i];
        for (i32 k = 0; k < 4; k++) {
          m.m[j][k] -= m.m[i][k] * buf;
          res.m[j][k] -= res.m[i][k] * buf;
        }
      }
    }
  }
  return res;
}
//行列の補間
Matrix4x4 Matrix4x4::Lerp(const Matrix4x4& mat1, const Matrix4x4& mat2,
                          float t) {
  return mat1 * (1.0f - t) + mat2 * t;
}
//主に法線ベクトル用となるベクトルを回転させる座標変換
Vector3 Matrix4x4::TransformNormal(const Vector3& v) {
  const float x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0];
  const float y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1];
  const float z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2];
  return Vector3(x, y, z);
}

//ベクトルとの積をさらにw除算した値を返す
Vector3 Matrix4x4::MultiplyCoord(const Vector3& v, const Matrix4x4& m) {
  const Matrix4x4 mat = CreateTranslate(v) * m;
  const Vector3 res(mat.m[3][0] / mat.m[3][3], mat.m[3][1] / mat.m[3][3],
                    mat.m[3][2] / mat.m[3][3]);
  return res;
}
//ベクトルとの乗算
Vector3 operator*(const Vector3& v, const Matrix4x4& mat) {
  const float x =
      v.x * mat.m[0][0] + v.y * mat.m[1][0] + v.z * mat.m[2][0] + mat.m[3][0];
  const float y =
      v.x * mat.m[0][1] + v.y * mat.m[1][1] + v.z * mat.m[2][1] + mat.m[3][1];
  const float z =
      v.x * mat.m[0][2] + v.y * mat.m[1][2] + v.z * mat.m[2][2] + mat.m[3][2];
  return Vector3(x, y, z);
}
//ベクトルとの乗算代入
Vector3& operator*=(Vector3& v, const Matrix4x4& mat) {
  v = v * mat;
  return v;
}

//加算
Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2) {
  Matrix4x4 result(m1);
  for (i32 i = 0; i < 4; i++) {
    for (i32 j = 0; j < 4; j++) {
      result.m[i][j] += m2.m[i][j];
    }
  }
  return result;
}
//減算
Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2) {
  Matrix4x4 result(m1);
  for (i32 i = 0; i < 4; i++) {
    for (i32 j = 0; j < 4; j++) {
      result.m[i][j] -= m2.m[i][j];
    }
  }
  return result;
}
//乗算
Matrix4x4 operator*(const Matrix4x4& m, float s) {
  Matrix4x4 result(m);
  for (i32 i = 0; i < 4; i++) {
    for (i32 j = 0; j < 4; j++) {
      result.m[i][j] *= s;
    }
  }
  return result;
}
//乗算
Matrix4x4 operator*(float s, const Matrix4x4& m) { return m * s; }
//乗算
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) {
  Matrix4x4 result;
  result.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] +
                   m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0];
  result.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] +
                   m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1];
  result.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] +
                   m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2];
  result.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] +
                   m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3];

  result.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] +
                   m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0];
  result.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] +
                   m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1];
  result.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] +
                   m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2];
  result.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] +
                   m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3];

  result.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] +
                   m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0];
  result.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] +
                   m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1];
  result.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] +
                   m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2];
  result.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] +
                   m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3];

  result.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] +
                   m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0];
  result.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] +
                   m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1];
  result.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] +
                   m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2];
  result.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] +
                   m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3];

  return result;
}
//除算
Matrix4x4 operator/(const Matrix4x4& m, float s) {
  MY_ASSERTION(s != 0, L"0除算が発生しました。");

  float oneOverK = 1.0f / s;
  return m * oneOverK;
}
}  // namespace math
}  // namespace legend
