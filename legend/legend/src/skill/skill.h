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
        //virtual void Update();
        //virtual void Draw();
    public:
        //! ���f����ID
        i32 model_id_;
        //! �g�p�\��
        i32 usable_count_;
        //! �Ďg�p�܂ł̃^�[����
        i32 recast_turn_;
        //! �X�L���̔����^�C�~���O
        SkillActivationTiming activetion_timing_;
        //! �X�L���̌��ʏI���^�C�~���O
        SkillEffectEndTiming end_timing_;
        //! ���O
        std::wstring name_;
        //! ������
        std::wstring explanatory_text_;
        //! �X�L���A�C�R����ID
        i32 skill_icon_id_;
};

}  // namespace skill
}  // namespace legend

#endif  //! LEGEND_SKILL_SKILL_H_
