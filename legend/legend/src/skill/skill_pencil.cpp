#include "skill_pencil.h"

#include "src\\stdafx.h"

namespace legend {
namespace skill {

SkillPencil::SkillPencil() {
    //���f����id�͉���-1
    model_id_ = -1;
    //! �K��g�p�\��
    usable_count_ = 1;
    //! �c��g�p�\��
    remaining_usable_count_ = usable_count_;
    //! �Ďg�p�܂ŋK��̃^�[����
    recast_turn_ = 1;
    //! �c��Ďg�p�܂ł̃^�[����
    remaining_recast_turn_ = 0;
    //! �X�L���̔����^�C�~���O
    activetion_timing_ = SkillActivationTiming::NOW;
    //! �X�L���̌��ʏI���^�C�~���O
    end_timing_ = SkillEffectEndTiming::NOW;
    //! �X�L���A�C�R����ID
    skill_icon_id_ = -1;
    //! �X�L�������̉摜��ID
    skill_explanation_id_ = -1;
    //! �g�p����邩�̃t���O
    is_use_ = false;
}

SkillPencil::~SkillPencil() {}

}  // namespace skill
}  // namespace legend
