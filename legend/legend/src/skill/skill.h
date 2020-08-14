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
        //virtual void Action();
        //virtual void Draw();

        virtual void Use();
        virtual void Action();


        void RemaingRecastTurnUpdate();

        i32 GetModelID();
        i32 GetRemainingUsableCount();
        i32 GetRemainingRecastTurn();
        SkillActivationTiming GetActivetionTiming();
        SkillEffectEndTiming GetEndTiming();
        i32 GetIconID();
        i32 GetExplanationID();
        bool GetUseFlag();

    protected:
        //! モデルのID
        i32 model_id_;
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
        i32 skill_icon_id_;
        //! スキル説明の画像のID
        i32 skill_explanation_id_;
        //! 使用されるかのフラグ
        bool is_use_;
};

}  // namespace skill
}  // namespace legend

#endif  //! LEGEND_SKILL_SKILL_H_
