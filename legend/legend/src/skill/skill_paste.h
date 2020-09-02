#ifndef LEGEND_SKILL_SKILL_PASTE_H_
#define LEGEND_SKILL_SKILL_PASTE_H_

#include "src/actor/actor.h"
#include "src/bullet/bounding_box.h"

namespace legend {
namespace skill {
class SkillPaste : public actor::Actor {
  using Parent = actor::Actor;

 public:
  /**
   * @brief コンストラクタ
   */
  SkillPaste();
  /**
   * @brief デストラクタ
   */
  virtual ~SkillPaste();
  /**
   * @brief 初期化
   */
  void Init(math::Vector3 position, actor::IActorMediator* mediator);
  /**
   * @brief 更新
   */
  bool Update();
  /**
   * @brief 描画
   */
  void Draw();
  /**
   * @brief 削除
   */
  void Destroy(actor::IActorMediator* mediator);
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
  //! コライダー
  std::shared_ptr<bullet::BoundingBox> box_;
  //! テクスチャリソース
  ComPtr<ID3D12Resource> mask_texture_;
  //! テクスチャの書き換えに必要なコピーリソース
  ComPtr<ID3D12Resource> mask_texture_copy_;
  //! テクスチャハンドル
  directx::descriptor_heap::DescriptorHandle handle_;
  //! ピクセルデータ
  std::array<u8, MASK_WIDTH * MASK_HEIGHT * PIXEL_SIZE> pixels_;
};
}  // namespace skill
}  // namespace legend

#endif  //! LEGEND_SKILL_SKILL_PASTE_H_
