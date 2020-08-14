#ifndef LEGEND_PHYSICS_RAY_H_
#define LEGEND_PHYSICS_RAY_H_

#include "src/primitive/line.h"

namespace legend {
namespace physics {

/**
 * @class Ray
 * @brief レイ（光線）クラス
 */
class Ray {
 public:
  /**
   * @brief コンストラクタ
   */
  Ray();
  /**
   * @brief コンストラクタ
   * @param 方向ベクトル
   * @param 最大範囲
   */
  Ray(math::Vector3 direction, float max_distance);
  /**
   * @brief コンストラクタ
   * @param 始点
   * @param 方向ベクトル
   * @param 最大範囲
   * @param 回転
   */
  Ray(math::Vector3 start_position, math::Vector3 direction,
      float max_distance, math::Quaternion rotation);
  /**
   * @brief デストラクタ
   */
  ~Ray();
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
  void Draw(directx::device::CommandList& command_list);
  /**
   * @brief 始点の取得
   */
  math::Vector3 GetStartPosition() const;
  /**
   * @brief 方向ベクトルの取得
   */
  math::Vector3 GetDirection() const;
  /**
   * @brief 最大範囲の取得
   */
  float GetDistance() const;
  /**
   * @brief 回転の取得
   */
  math::Quaternion GetRotation() const;
  /**
   * @brief 始点の設定
   */
  void SetStartPosition(const math::Vector3& start_position);
  /**
   * @brief 回転の設定
   */
  void SetRotation(const math::Quaternion& rotation);

 private:
  //! 始点
  math::Vector3 start_position_;
  //! 方向ベクトル
  math::Vector3 direction_;
  //! 衝突範囲
  float max_distance_;
  //! 回転
  math::Quaternion rotation_;

  primitive::Line draw_line_;
};

}  // namespace physics
}  // namespace legend

#endif  //! LEGEND_PHYSICS_RAY_H_
