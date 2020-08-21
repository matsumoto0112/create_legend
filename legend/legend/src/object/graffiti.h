#ifndef LEGEND_OBJECT_GRAFFITI_H_
#define LEGEND_OBJECT_GRAFFITI_H_

/**
 * @file graffiti.h
 */

#include "src/actor/actor.h"
#include "src/bullet/bounding_box.h"
#include "src/object/fragment.h"
#include "src/util/color_4.h"
#include "src/util/timer.h"
#include "src/util/transform.h"

namespace legend {
namespace object {

/**
 * @brief 落書きの初期化パラメータ
 */
struct GraffitiInitializeParameter {
  // math::Vector3 position;
  // math::Vector3 scale;
  util::Transform transform;
  math::Vector3 bounding_box_length;
  float remaining_graffiti;
};

/**
 * @brief 落書きオブジェクトクラス
 */
class Graffiti : public actor::Actor {
  using Parent = actor::Actor;

 public:
  /**
   * @brief コンストラクタ
   */
  Graffiti();
  /**
   * @brief デストラクタ
   */
  virtual ~Graffiti();
  /**
   * @brief 初期化
   * @param param 初期化パラメータ
   * @param command_list コマンドリスト
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(actor::IActorMediator* mediator,
            const GraffitiInitializeParameter& param,
            directx::device::CommandList& command_list);
  /**
   * @brief 更新処理
   */
  bool Update();
  /**
   * @brief 描画処理
   * @param command_list コマンドリスト
   */
  void Draw(directx::device::CommandList& command_list);
  /**
   * @brief 落書きを消す処理
   * @param 消す割合
   */
  void DecreaseGraffiti(const float& percentage);
  /**
   * @brief 落書きの残量の取得
   */
  float GetRemainingGraffiti() const;
  /**
   * @brief 消えているかを取得
   */
  bool GetIsErase() const;
  /**
   * @brief 消しカス生成処理
   */
   //void InstanceFragment();

  void OnHit(bullet::Collider* other);

 private:
  /**
   * @brief テクスチャの色を書き換える
   * @param x UV座標
   * @param x UV座標
   * @param color 書き換える色
   */
  void SetTextureColor(u32 x, u32 y, const util::Color4& color);
  /**
   * @brief テクスチャを更新する
   * @param command_list コマンドリスト
   */
  void UpdateTexture(directx::device::CommandList& command_list);

 private:
  //! ピクセルのバイトサイズ
  static constexpr u32 PIXEL_SIZE = 4;
  //! マスク画像の幅
  static constexpr u32 MASK_WIDTH = 16;
  //! マスク画像の高さ
  static constexpr u32 MASK_HEIGHT = 16;

 private:
  //! テクスチャリソース
  ComPtr<ID3D12Resource> mask_texture_;
  //! テクスチャの書き換えに必要なコピーリソース
  ComPtr<ID3D12Resource> mask_texture_copy_;
  //! テクスチャハンドル
  directx::descriptor_heap::DescriptorHandle handle_;
  //! ピクセルデータ
  std::array<u8, MASK_WIDTH * MASK_HEIGHT * PIXEL_SIZE> pixels_;

  //! 落書き残量
  float remaining_graffiti_;
  //! 消えたかどうか
  bool is_erase_;
  std::shared_ptr<bullet::BoundingBox> box_;

  util::CountDownTimer delete_time_;
};

}  // namespace object
}  // namespace legend
#endif  //! LEGEND_OBJECT_GRAFFITI_H_
