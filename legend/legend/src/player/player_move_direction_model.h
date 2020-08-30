#ifndef LEGEND_PLAYER_PLAYER_MOVE_DIRECTION_MODEL_H
#define LEGEND_PLAYER_PLAYER_MOVE_DIRECTION_MODEL_H

/**
 * @file player_move_direction_model.h
 * @brief プレイヤーの向いている方向を描画するモデル管理クラス
 */

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/constant_buffer_structure.h"
#include "src/util/transform.h"

namespace legend {
namespace player {

/**
 * @brief プレイヤーの向いている方向を描画するモデル
 */
class PlayerMoveDirectionModel {
 public:
  /**
   * @brief コンストラクタ
   */
  PlayerMoveDirectionModel();
  /**
   * @brief デストラクタ
   */
  virtual ~PlayerMoveDirectionModel();
  /**
   * @brief 初期化
   */
  bool Init();
  /**
   * @brief 座標を設定する
   */
  void SetPosition(const math::Vector3& position);
  /**
   * @brief 回転を設定する
   */
  void SetRotation(const math::Quaternion& rotation);
  /**
   * @brief 描画
   */
  void Draw();

 private:
  //! トランスフォーム
  util::Transform transform_;
  //! トランスフォームコンスタントバッファ
  directx::buffer::ConstantBuffer<
      directx::buffer::constant_buffer_structure::Transform>
      transform_cb_;
};
}  // namespace player
}  // namespace legend

#endif  //! LEGEND_PLAYER_PLAYER_MOVE_DIRECTION_MODEL_H
