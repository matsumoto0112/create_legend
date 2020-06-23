#ifndef LEGEND_MATH_MATRIX_4X4_H_
#define LEGEND_MATH_MATRIX_4X4_H_

#include "src/math/vector_2.h"
#include "src/math/vector_3.h"

/**
 * @file matrix_4x4.h
 * @brief 4x4行列定義
 */

namespace legend {
namespace math {
/**
 * @brief 4x4行列
 */
class Matrix4x4 {
 public:
  //! 行列
  std::array<std::array<float, 4>, 4> m;

 public:
  //! 単位行列
  static const Matrix4x4 kIdentity;
  //! ゼロ行列
  static const Matrix4x4 kZero;

 public:
  /**
   * @brief コンストラクタ
   */
  Matrix4x4();
  /**
   * @brief コンストラクタ
   */
  Matrix4x4(float m11, float m12, float m13, float m14, float m21, float m22,
            float m23, float m24, float m31, float m32, float m33, float m34,
            float m41, float m42, float m43, float m44);
  /**
   * @brief コンストラクタ
   */
  Matrix4x4(const std::array<std::array<float, 4>, 4>& m);
  /**
   * @brief コンストラクタ
   */
  Matrix4x4(const Matrix4x4& m);
  /**
   * @brief 代入演算子
   */
  Matrix4x4& operator=(const Matrix4x4& mat) &;
  /**
   * @brief 代入演算子
   */
  Matrix4x4& operator=(Matrix4x4&& mat) & noexcept;
  /**
   * @brief 単項プラス演算子
   */
  Matrix4x4 operator+();
  /**
   * @brief 単項マイナス演算子
   */
  Matrix4x4 operator-();
  /**
   * @brief 加算代入演算子
   */
  Matrix4x4& operator+=(const Matrix4x4& mat);
  /**
   * @brief 減算代入演算子
   */
  Matrix4x4& operator-=(const Matrix4x4& mat);
  /**
   * @brief 乗算代入演算子
   */
  Matrix4x4& operator*=(float k);
  /**
   * @brief 乗算代入演算子
   */
  Matrix4x4& operator*=(const Matrix4x4& mat);
  /**
   * @brief 除算代入演算子
   */
  Matrix4x4& operator/=(float k);
  /**
   * @brief 平行移動行列の作成
   * @param v 移動量
   */
  static Matrix4x4 CreateTranslate(const Vector3& v);
  /**
   * @brief X軸回転行列の作成
   * @param rad 回転量
   */
  static Matrix4x4 CreateRotationX(float rad);
  /**
   * @brief Y軸回転行列の作成
   * @param rad 回転量
   */
  static Matrix4x4 CreateRotationY(float rad);
  /**
   * @brief Z軸回転行列の作成
   * @param rad 回転量
   */
  static Matrix4x4 CreateRotationZ(float rad);
  /**
   * @brief 回転行列の作成
   * @param r 各軸の回転量
   */
  static Matrix4x4 CreateRotation(const Vector3& r);
  /**
   * @brief 拡大・縮小行列の作成
   * @param s 各軸の拡大・縮小の大きさ
   */
  static Matrix4x4 CreateScale(const Vector3& s);
  /**
   * @brief ビュー行列の作成
   * @param eye 視点
   * @param at 注視点
   * @param up 上方向のベクトル
   */
  static Matrix4x4 CreateView(const Vector3& eye, const Vector3& at,
                              const Vector3& up);
  /**
   * @brief プロジェクション行列の作成
   * @param fov_y 視野角(rad)
   * @param aspect アスペクト比
   * @param near_z 最近点
   * @param far_z 最遠点
   */
  static Matrix4x4 CreateProjection(float fov_y, float aspect, float near_z,
                                    float far_z);
  /**
   * @brief 正投影行列の作成
   * @param screen_size 画面の大きさ
   */
  static Matrix4x4 CreateOrthographic(const Vector2& screen_size);
  /**
   * @brief 転置行列を取得する
   */
  Matrix4x4 Transpose() const;
  /**
   * @brief 行列式を求める
   */
  float Determinant() const;
  /**
   * @brief 逆行列を求める
   * @param mat 求める行列
   */
  Matrix4x4 Inverse() const;
  /**
   * @brief 行列の補間
   * @param mat1 行列1
   * @param mat2 行列2
   * @param t 補間係数
   */
  static Matrix4x4 Lerp(const Matrix4x4& mat1, const Matrix4x4& mat2, float t);
  /**
   * @brief ベクトルとの掛け算(法線ベクトル用で回転のみ)
   */
  Vector3 TransformNormal(const Vector3& v);

  /**
   * @brief ベクトルと行列の積を求め、wで除算された値を返す
   */
  static Vector3 MultiplyCoord(const Vector3& v, const Matrix4x4& m);
};

/**
 * @brief 等価比較演算子
 */
inline bool operator==(const Matrix4x4& m1, const Matrix4x4& m2) {
  return m1.m == m2.m;
}
/**
 * @brief 等価比較演算子
 */
inline bool operator!=(const Matrix4x4& m1, const Matrix4x4& m2) {
  return !(m1 == m2);
}
/**
 * @brief 加算
 */
Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2);
/**
 * @brief 減算
 */
Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2);
/**
 * @brief 乗算
 */
Matrix4x4 operator*(const Matrix4x4& m, float s);
/**
 * @brief 乗算
 */
Matrix4x4 operator*(float s, const Matrix4x4& m);
/**
 * @brief 乗算
 */
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);
/**
 * @brief 除算
 */
Matrix4x4 operator/(const Matrix4x4& m, float s);

}  // namespace math
}  // namespace legend

#endif  //! LEGEND_MATH_MATRIX_4X4_H_
