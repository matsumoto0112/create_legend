#ifndef LEGEND_PHYSICS_CAPSULE_H_
#define LEGEND_PHYSICS_CAPSULE_H_

#include "src/physics/bounding_box.h"

namespace legend {
namespace physics {

/**
 * @class Capsule
 * @brief カプセルのクラス
 */
class Capsule {
 public:
  /**
   * @brief コンストラクタ
   */
  Capsule();
  /**
   * @brief コンストラクタ
   */
  Capsule(math::Vector3 end_position, float radius, float scale);
  /**
   * @brief コンストラクタ
   */
  Capsule(math::Vector3 start_position, math::Vector3 end_position,
          float radius, float scale);
  /**
   * @brief デストラクタ
   */
  ~Capsule();
  /**
   * @brief 初期化
   */
  bool Initialize();
  /**
   * @brief 更新
   */
  void Update();
  /**
   * @brief 描画
   */
  void Draw();
  /**
   * @brief 始点の設定
   */
  void SetStartPosition(math::Vector3 position);
  /**
   * @brief 終点の設定
   */
  void SetEndPosition(math::Vector3 position);
  /**
   * @brief 回転量の設定
   */
  void SetRotation(math::Quaternion rotate);
  /**
   * @brief スケールの設定
   */
  void SetScale(float scale);
  /**
   * @brief 半径の設定
   */
  void SetRadius(float radius);
  /**
   * @brief 始点の取得
   */
  math::Vector3 GetStartPosition();
  /**
   * @brief 終点の取得
   */
  math::Vector3 GetEndPosition();
  /**
   * @brief 回転量の取得
   */
  math::Quaternion GetRotation();
  /**
   * @briefスケールの取得
   */
  float GetScale();
  /**
   * @brief 半径の取得
   */
  float GetRadius();
  /**
   * @brief 長さの取得
   */
  float GetLength();

 private:
  //! 始点
  math::Vector3 start_position_;
  //! 終点
  math::Vector3 end_position_;
  //! 回転量
  math::Quaternion rotation_;
  //! スケール
  float scale_;
  //! 半径
  float radius_;
};

}  // namespace physics
}  // namespace legend

#endif  //! LEGEND_PHYSICS_CAPSULE_H_
