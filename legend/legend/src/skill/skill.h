#ifndef LEGEND_SKILL_SKILL_H_
#define LEGEND_SKILL_SKILL_H_

#include "src/draw/model.h"
#include "src/physics/bounding_box.h"
#include "src/player/player.h"
#include "src/skill/skill_type.h"
#include "src/actor/actor.h"

/**
 * @file skill.h
 * @brief シーンの基底クラス
 */
namespace legend {
namespace skill {

class Skill : public actor::Actor {
    using Parent = actor::Actor;
 public:
     Skill();
  /**
   * @brief 初期化
   */
  virtual void Init(const player::Player& player);
  /**
   * @brief 更新処理
   */
  virtual bool Update();
  ///**
  // * @brief 描画処理
  // */
  //void Draw() override;

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

  // i32 GetModelID();
  i32 GetRemainingUsableCount();
  i32 GetRemainingRecastTurn();
  SkillActivationTiming GetActivetionTiming();
  SkillEffectEndTiming GetEndTiming();
  i32 GetIconTexture();
  i32 GetExplanationTexture();
  bool GetUseFlag();
  bool ProductionFlag();

 protected:
  //! モデルのID
  // i32 model_id_;
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
  //! スキルアイコンのID
  // i32 skill_icon_id_;
  //! スキル説明の画像のID
  // i32 skill_explanation_id_;
  //! 使用されるかのフラグ
  bool is_use_;
  //! 演出中かどうか
  bool is_production_;
  //! プレイヤー
  const player::Player* player_;
};

}  // namespace skill
}  // namespace legend

#endif  //! LEGEND_SKILL_SKILL_H_
