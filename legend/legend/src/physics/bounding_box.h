#ifndef LEGEND_PHYSICS_BOUNDING_BOX_H_
#define LEGEND_PHYSICS_BOUNDING_BOX_H_

#include "src/math/vector_3.h"

namespace legend {
namespace physics {

/**
 * @class BoundingBox
 * @brief 直方体のクラス
 */

class BoundingBox {
 public:
  /**
   * @brief コンストラクタ
   */
  BoundingBox();
  /*
   * @brief 方向ベクトルを取得
   * @param 軸番号
   */
  math::Vector3 GetDirection(i32 direction_num);
  /**
   * @brief 長さを取得
   * @param 軸番号
   */
  float GetLength(i32 length_num);
  /**
   * @brief 現在の位置を取得
   */
  math::Vector3 GetPosition();
  /**
   * @brief 各方向ベクトルの設定
   * @param X方向
   * @param Y方向
   * @param Z方向
   */
  void SetDirection(math::Vector3 direction_x, math::Vector3 direction_y,
                    math::Vector3 direction_z);
  /**
   * @brief 各方向の長さの設定
   * @param X方向の長さ
   * @param Y方向の長さ
   * @param Z方向の長さ
   */
  void SetLength(float length_x, float length_y, float length_z);

 private:
  //!中心座標
  math::Vector3 position_;
  //!方向ベクトル
  std::vector<math::Vector3> directions_;
  //!各軸方向の長さ
  std::vector<float> lengthes_;
};

}  // namespace physics
}  // namespace legend

#endif  //! LEGEND_PHYSICS_BOUNDING_BOX_H_
