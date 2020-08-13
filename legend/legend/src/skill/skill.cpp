#include "skill.h"

#include "src\\stdafx.h"

namespace legend {
namespace skill {

i32 Skill::GetModelID() { return model_id_; }

i32 Skill::GetRemainingUsableCount() { return remaining_usable_count_; }

i32 Skill::GetRemainingRecastTurn() { return remaining_recast_turn_; }

SkillActivationTiming Skill::GetActivetionTiming() {
  return activetion_timing_;
}

SkillEffectEndTiming Skill::GetEndTiming() { return end_timing_; }

i32 Skill::GetIconID() { return skill_icon_id_; }

i32 Skill::GetExplanationID() { return skill_explanation_id_; }

}  // namespace skill
}  // namespace legend