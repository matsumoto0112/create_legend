#ifndef LEGEND_UTIL_TRANSFORM_H_
#define LEGEND_UTIL_TRANSFORM_H_

/**
 * @file transform.h
 * @brief トランスフォーム管理クラス定義
 */

#include "src/math/matrix_4x4.h"
#include "src/math/quaternion.h"
#include "src/math/vector_3.h"

namespace legend {
namespace util {

/**
 * @class Transform
 * @brief トランスフォーム管理クラス
 */
class Transform {
 public:
  /**
   * @brief コンストラクタ
   * @param position 座標
   * @param rotation 回転
   * @param scale スケーリング
   */
  Transform(const math::Vector3& position = math::Vector3::kZeroVector,
            const math::Quaternion& rotation = math::Quaternion::kIdentity,
            const math::Vector3& scale = math::Vector3::kUnitVector);
  /**
   * @brief デストラクタ
   */
  ~Transform();

  /**
   * @brief 座標を設定する
   */
  inline void SetPosition(const math::Vector3& position) {
    this->position_ = position;
  }
  /**
   * @brief 座標を取得する
   */
  inline math::Vector3 GetPosition() const { return position_; }
  /**
   * @brief 回転を設定する
   */
  inline void SetRotation(const math::Quaternion& rotation) {
    this->rotation_ = rotation;
  }
  /**
   * @brief 回転を取得する
   */
  inline math::Quaternion GetRotation() const { return rotation_; }
  /**
   * @brief スケーリングを設定する
   */
  inline void SetScale(const math::Vector3& scale) { this->scale_ = scale; }
  /**
   * @brief スケーリングを取得する
   */
  inline math::Vector3 GetScale() const { return scale_; }

  /**
   * @brief ワールド行列を作成する
  */
  math::Matrix4x4 CreateWorldMatrix() const;

 private:
  //! 座標
  math::Vector3 position_;
  //! 回転
  math::Quaternion rotation_;
  //! スケーリング
  math::Vector3 scale_;
};

}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_TRANSFORM_H_
