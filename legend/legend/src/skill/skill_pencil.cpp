#include "skill_pencil.h"

#include "src\\stdafx.h"

namespace legend {
namespace skill {

SkillPencil::SkillPencil() {
    //モデルのidは仮で-1
    model_id_ = -1;
    //! 規定使用可能回数
    usable_count_ = 1;
    //! 残り使用可能回数
    remaining_usable_count_ = usable_count_;
    //! 再使用まで規定のターン数
    recast_turn_ = 1;
    //! 残り再使用までのターン数
    remaining_recast_turn_ = 0;
    //! スキルの発動タイミング
    activetion_timing_ = SkillActivationTiming::NOW;
    //! スキルの効果終了タイミング
    end_timing_ = SkillEffectEndTiming::NOW;
    //! スキルアイコンのID
    skill_icon_id_ = -1;
    //! スキル説明の画像のID
    skill_explanation_id_ = -1;
    //! 使用されるかのフラグ
    is_use_ = false;
}

SkillPencil::~SkillPencil() {}

}  // namespace skill
}  // namespace legend
