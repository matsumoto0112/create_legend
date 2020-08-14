#ifndef LEGEND_SKILL_SKILL_MANAGER_H_
#define LEGEND_SKILL_SKILL_MANAGER_H_

#include "src/skill/skill.h"
#include "src/skill/skill_select_ui.h"
#include "src/skill/skill_type.h"

namespace legend {
namespace skill {
class SkillManager {
public:
    SkillManager();
    ~SkillManager();
    void Init();
    void Update();
    void GetSkill(i32 skill_id);
    void AddSkill(const Skill& skill);
    void AddSkill();
    void PlayerTurnEnd();
    void EnemyTurnEnd();
    void Draw();
 private:
  //! 所持できる最大スキル数
  i32 skill_max_count_;
  //! 所持しているスキル用リスト
  std::vector<Skill> skills_;
  //! スキル選択のUI用クラス
  SkillSelectUI select_ui_;
  //! このターン手に入れたスキル
  std::vector<Skill> this_turn_get_skills_;
};
}  // namespace skill
}  // namespace legend

#endif  //! LEGEND_SKILL_SKILL_MANAGER_H_
