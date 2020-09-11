#ifndef LEGEND_SKILL_SKILL_H_
#define LEGEND_SKILL_SKILL_H_

#include "src/actor/actor.h"
#include "src/bullet/bounding_box.h"
#include "src/draw/model.h"
#include "src/skill/skill_type.h"

/**
 * @file skill.h
 * @brief スキルの基底クラス
 */
namespace legend {
namespace player {
class Player;
}

namespace skill {
namespace skill_name {
constexpr const wchar_t* SKILLPENCIL = L"SkiilPencil";
constexpr const wchar_t* SKILLPASTESTICK = L"SkiilPasteStick";
constexpr const wchar_t* SKILLCOMPASS = L"SkiilCompass";
}  // namespace skill_name

class Skill : public actor::Actor {
  using Parent = actor::Actor;

 public:
  Skill();
  virtual ~Skill();
  /**
   * @brief 初期化
   */
  virtual bool Init(actor::IActorMediator* mediator, player::Player* player);
  /**
   * @brief 更新処理
   */
  virtual bool Update();
  /**
   * @brief スキルを選択された際のメソッド
   */
  virtual void Use();
  /**
   * @brief 発動
   */
  virtual void Action();
  /**
   * @brief スキル演出の更新
   */
  virtual void ProductionUpdate();
  /**
   * @brief 終了
   */
  virtual void EndAction();
  /**
   * @brief 再使用までのターン数の更新
   */
  void RemaingRecastTurnUpdate();
  /**
   * @brief 装備スキル順に位置を調整
   */
  void AdjustPosition(math::Vector3 position);
  /**
   * @brief 選択中のスキルの位置を変更
   */
  void ChangePosition(math::Vector3 position);
  /**
   * @brief 残り使用回数の取得
   */
  i32 GetRemainingUsableCount();
  /**
   * @brief リキャストターンの取得
   */
  i32 GetRemainingRecastTurn();
  /**
   * @brief スキル発動タイミングの取得
   */
  SkillActivationTiming GetActivetionTiming();
  /**
   * @brief スキル終了タイミングの取得
   */
  SkillEffectEndTiming GetEndTiming();
  /**
   * @brief スキルアイコン画像の取得
   */
  std::shared_ptr<directx::buffer::Texture2D> GetIconTexture() const;
  /**
   * @brief スキル使用中アイコンの取得
   */
  std::shared_ptr<directx::buffer::Texture2D> GetIconUsingTexture() const;
  /**
   * @brief 装備スキル画像の取得
   */
  std::shared_ptr<directx::buffer::Texture2D> GetEquipmentTexture() const;
  /**
   * @brief スキル説明画像の取得
   */
  std::shared_ptr<directx::buffer::Texture2D> GetExplanationTexture() const;
  /**
   * @brief スキル使用フラグの取得
   */
  bool GetUseFlag();
  /**
   * @brief 演出中フラグの取得
   */
  bool ProductionFlag();
  /**
   * @brief コライダーの削除
   */
  void RemoveCollider();
  /**
   * @brief スキルの演出終了判定取得
   */
  bool EndSkillProduction() const;
  /**
   * @brief スキル名の設定
   */
  void SetName(const std::wstring name);
  /**
   * @brief スキル名の取得
   */
  std::wstring GetName() const;

 protected:
  //! 規定使用可能回数
  i32 usable_count_;
  //! 残り使用可能回数
  i32 remaining_usable_count_;
  //! 再使用まで規定のターン数
  i32 recast_turn_;
  //! 残り再使用までのターン数
  i32 remaining_recast_turn_;
  //! スキルの発動タイミング
  SkillActivationTiming activetion_timing_;
  //! スキルの効果終了タイミング
  SkillEffectEndTiming end_timing_;

  //! スキルアイコンのテクスチャ
  std::shared_ptr<directx::buffer::Texture2D> skill_icon_texture_;
  //! スキル使用中アイコンのテクスチャ
  std::shared_ptr<directx::buffer::Texture2D> skill_icon_using_texture_;
  //! スキル説明のテクスチャ
  std::shared_ptr<directx::buffer::Texture2D> skill_explanation_texture_;
  //! 装備状態表示用のテクスチャ
  std::shared_ptr<directx::buffer::Texture2D> equipment_texture_;

  //! 使用されるかのフラグ
  bool is_use_;
  //! 演出中かどうか
  bool is_production_;
  //! プレイヤー
  const player::Player* player_;
  //! コライダー
  std::shared_ptr<bullet::BoundingBox> box_;
};

}  // namespace skill
}  // namespace legend

#endif  //! LEGEND_SKILL_SKILL_H_
