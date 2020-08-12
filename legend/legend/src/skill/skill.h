#ifndef LEGEND_SKILL_SKILL_H_
#define LEGEND_SKILL_SKILL_H_

#include "src/skill/skill_type.h"

/**
 * @file skill.h
 * @brief シーンの基底クラス
 */
namespace legend {
namespace skill {

    class Skill {
    public:
        //virtual void Init();
        //virtual void Update();
        //virtual void Draw();
    public:
        //! モデルのID
        i32 model_id_;
        //! 使用可能回数
        i32 usable_count_;
        //! 再使用までのターン数
        i32 recast_turn_;
        //! スキルの発動タイミング
        SkillActivationTiming activetion_timing_;
        //! スキルの効果終了タイミング
        SkillEffectEndTiming end_timing_;
        //! 名前
        std::wstring name_;
        //! 説明文
        std::wstring explanatory_text_;
        //! スキルアイコンのID
        i32 skill_icon_id_;
};

}  // namespace skill
}  // namespace legend

#endif  //! LEGEND_SKILL_SKILL_H_
