#ifndef LEGEND_SKILL_SKILL_PENCIL_H_
#define LEGEND_SKILL_SKILL_PENCIL_H_

#include "src/skill/skill.h"

namespace legend {
namespace skill {

class SkillPencil : public Skill {
    using Parent = actor::Actor;
 public:
  /**
   * @brief コンストラクタ
   */
  SkillPencil();
  /**
   * @brief デストラクタ
   */
  ~SkillPencil();

  /**
   * @brief 初期化
   */
  void Init(const player::Player &player) override;
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
  //モデルの位置
  math::Vector3 model_position_;
};

}  // namespace skill
}  // namespace legend
#endif  //! LEGEND_SKILL_SKILL_PENCIL_H_