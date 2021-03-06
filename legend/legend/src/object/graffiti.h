#ifndef LEGEND_OBJECT_GRAFFITI_H_
#define LEGEND_OBJECT_GRAFFITI_H_

#include "src/actor/actor.h"
#include "src/bullet/bounding_box.h"
#include "src/object/fragment.h"
#include "src/util/color_4.h"
#include "src/util/timer.h"
#include "src/util/transform.h"

/**
 * @file graffiti.h
 * @brief 落書きクラス
 */
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
  float can_erase_speed;
  i32 model_id;
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
  void Draw() override;
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
  std::unique_ptr<Fragment> InstanceFragment();

  //bool GetIsHit() const;

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
  /**
   * @brief 消しカス生成位置の設定
   * @param position 座標
   * @param velocity 速度
   */
  void SetInstancePosition(math::Vector3 position, math::Vector3 velocity);

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
  //! 消しゴムと衝突しているか
  bool is_hit_;
  //! コライダー
  std::shared_ptr<bullet::BoundingBox> box_;

  util::CountDownTimer delete_time_;
  //! 消しカスの生成位置
  math::Vector3 instance_position_;
  //! 落書きを消せるスピードの大きさ
  float can_erase_speed_;
  float update_time_;
};

}  // namespace object
}  // namespace legend
#endif  //! LEGEND_OBJECT_GRAFFITI_H_
