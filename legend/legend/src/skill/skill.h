#ifndef LEGEND_SKILL_SKILL_H_
#define LEGEND_SKILL_SKILL_H_

#include "src/skill/skill_type.h"

/**
 * @file skill.h
 * @brief �V�[���̊��N���X
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
        //! ���f����ID
        i32 model_id_;
        //! �K��g�p�\��
        i32 usable_count_;
        //! �c��g�p�\��
        i32 remaining_usable_count_;
        //! �Ďg�p�܂ŋK��̃^�[����
        i32 recast_turn_;
        //! �c��Ďg�p�܂ł̃^�[����
        i32 remaining_recast_turn_;
        //! �X�L���̔����^�C�~���O
        SkillActivationTiming activetion_timing_;
        //! �X�L���̌��ʏI���^�C�~���O
        SkillEffectEndTiming end_timing_;
        //! �X�L���A�C�R����ID
        i32 skill_icon_id_;
        //! �X�L�������̉摜��ID
        i32 skill_explanation_id_;
        //! �g�p����邩�̃t���O
        bool is_use_;
};

}  // namespace skill
}  // namespace legend

#endif  //! LEGEND_SKILL_SKILL_H_
