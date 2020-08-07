#ifndef LEGEND_PHYSICS_BOUNDING_BOX_H_
#define LEGEND_PHYSICS_BOUNDING_BOX_H_

#include "src/physics/collider.h"

namespace legend {
namespace physics {

/**
 * @class BoundingBox
 * @brief 直方体のクラス
 */

class BoundingBox : public Collider {
 public:
  /**
   * @brief コンストラクタ
   */
  BoundingBox();
  /**
   * @brief コンストラクタ
   * @param 座標
   * @param 回転
   * @param スケール
   */
  BoundingBox(math::Vector3 position, math::Quaternion rotation,
              math::Vector3 scale);
  /**
   * @brief デストラクタ
   */
  ~BoundingBox();
  /**
   * @brief 初期化
   * @param デバイス
   */
  bool Init() override;
  /**
   * @brief 更新
   */
  void Update() override;
  /*
   * @brief 方向ベクトルを取得
   * @param 軸番号
   */
  math::Vector3 GetDirection(i32 direction_num) const;
  /**
   * @brief 長さを取得
   * @param 軸番号
   */
  float GetLength(i32 length_num) const;
  /**
   * @brief スケール倍の長さを取得
   * @param 軸番号
   */
  float GetLengthByScale(i32 length_num) const;
  /**
   * @brief 現在の位置を取得
   */
  math::Vector3 GetPosition() const;
  /**
   * @brief 現在の角度を取得
   */
  math::Quaternion GetRotation() const;
  /**
   * @brief 現在のスケールを取得
   */
  math::Vector3 GetScale() const;
  /**
   * @brief 分離軸Xの取得
   */
  math::Vector3 GetAxisX() const;
  /**
   * @brief 分離軸Yの取得
   */
  math::Vector3 GetAxisY() const;
  /**
   * @brief 分離軸Zの取得
   */
  math::Vector3 GetAxisZ() const;
  /**
   * @brief トリガー判定の取得
   */
  bool GetIsTrigger() const;
  /**
   * @brief 接地判定の取得
   */
  bool GetOnGround() const;
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
  /**
   * @brief 中心座標の更新
   * @param 座標
   */
  void SetPosition(math::Vector3 position);
  /**
   * @brief 回転角度の更新
   * @param 角度
   */
  void SetRotation(math::Quaternion rotate);
  /**
   * @brief 拡大縮小の更新
   * @param スケール
   */
  void SetScale(math::Vector3 scale);
  /**
   * @brief 分離軸の更新
   */
  void SetAxis();
  /**
   * @brief トリガー設定
   */
  void SetIsTrigger(bool trigger);
  /**
   * @brief 接地判定の設定
   */
  void SetOnGround(bool is_ground);

 private:
  //!方向ベクトル
  std::vector<math::Vector3> directions_;
  //!各軸方向の長さ(半径)
  std::vector<float> lengthes_;
  //!分離軸X
  math::Vector3 axis_x;
  //!分離軸Y
  math::Vector3 axis_y;
  //!分離軸Z
  math::Vector3 axis_z;

  //! トリガーかどうか
  bool is_trigger_;
  //! 接地判定
  bool is_on_ground_;
};

}  // namespace physics
}  // namespace legend

#endif  //! LEGEND_PHYSICS_BOUNDING_BOX_H_
