#ifndef LEGEND_SKILL_SKILL_PASTE_STICK_H_
#define LEGEND_SKILL_SKILL_PASTE_STICK_H_

#include "src/skill/skill.h"
#include "src/skill/skill_paste.h"

namespace legend {
namespace skill {
class SkillPasteStick : public Skill {
  using Parent = Skill;

 public:
  /**
   * @brief コンストラクタ
   */
  SkillPasteStick();
  /**
   * @brief デストラクタ
   */
  virtual ~SkillPasteStick();
  /**
   * @brief 初期化
   */
  bool Init(actor::IActorMediator* mediator, player::Player* player) override;
  /**
   * @brief 更新処理
   */
  bool Update() override;
  /**
   * @brief 描画処理
   */
  void Draw() override;
  /**
   * @brief スキルを選択された際のメソッド
   */
  void Use() override;
  /**
   * @brief 発動
   */
  void Action() override;
  /**
   * @brief スキル演出の更新
   */
  void ProductionUpdate() override;
  /**
   * @brief 終了
   */
  void EndAction() override;

 private:
  //! 糊
  std::vector<std::shared_ptr<SkillPaste>> pastes_;
  //! 生成数
  i32 instance_count_;
};
}  // namespace skill
}  // namespace legend

#endif  //! LEGEND_SKILL_SKILL_PASTE_STICK_H_
