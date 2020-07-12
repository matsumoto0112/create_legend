#ifndef LEGEND_MATH_QUATERNION_H_
#define LEGEND_MATH_QUATERNION_H_

/**
 * @file quaternion.h
 * @brief  四元数管理クラス定義
 */

#include "src/math/matrix_4x4.h"
#include "src/math/vector_3.h"

namespace legend {
namespace math {

/**
 * @class Quaternion
 * @brief 四元数
 */
class Quaternion {
 public:
  //! 単位四元数
  static const Quaternion kIdentity;

 public:
  //! xベクトル
  float x;
  //! yベクトル
  float y;
  //! zベクトル
  float z;
  //! 偏角
  float w;

 public:
  /**
   * @brief コンストラクタ
   */
  Quaternion();
  /**
   * @brief コンストラクタ
   */
  Quaternion(float x, float y, float z, float w);
  /**
   * @brief コンストラクタ
   * @param nv 正規化された回転軸
   * @param angle 回転量
   */
  Quaternion(const Vector3& nv, float angle);
  /**
   * @brief デストラクタ
   */
  ~Quaternion();
  /**
   * @brief 正規化四元数を取得する
   */
  Quaternion Normalized() const;
  /**
   * @brief 共役の四元数を求める
   */
  Quaternion Conjugate() const;
  /**
   * @brief ロール・ピッチ・ヨーから四元数を生成する
   */
  static Quaternion FromEular(float roll, float pitch, float yaw);
  /**
   * @brief ロール・ピッチ・ヨーから四元数を生成する
   */
  static Quaternion FromEular(const Vector3& v);
  /**
   * @brief オイラー角を取得する
   */
  Vector3 ToEular() const;
  /**
   * @brief 四元数をオイラー角に変換する
   */
  static Vector3 ToEular(const Quaternion& q);
  /**
   * @brief 回転行列に変換する
   */
  static Matrix4x4 ToMatrix(const Quaternion& q);
};

/**
 * @brief 乗算演算子
 */
Quaternion operator*(const Quaternion& q1, const Quaternion& q2);
/**
 * @brief ベクトルとの積
 */
Vector3 operator*(const Quaternion& q, const Vector3& v);

}  // namespace math
}  // namespace legend

#endif  //! LEGEND_MATH_QUATERNION_H_
