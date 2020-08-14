#ifndef LEGEND_SKILL_SKILL_SELECT_UI_H_
#define LEGEND_SKILL_SKILL_SELECTUI_H_

#include "src/skill/skill.h"
#include "src/draw/sprite_renderer.h"
#include "src/draw/texture_string.h"

/**
 * @file skill_select_ui.h
 * @brief �X�L���I��UI�N���X
 */
namespace legend {
namespace skill {

class SkillSelectUI {

public:
    /**
     * @brief �R���X�g���N�^
     */
    SkillSelectUI();
    /**
     * @brief �f�X�g���N�^
     */
    ~SkillSelectUI();
    /**
     * @brief ����������
     */
    void Init();
    /**
     * @brief �X�V����
     */
    void Update();
    /**
     * @brief �`�揈��
     */
    void Draw();
    /**
     * @brief �f�X�g���N�^
     */
    void AddSkill(/*const Skill* skill*/);

private:
    math::Vector2 icon_base_position_;
    math::Vector2 icon_scale_;

    std::vector<draw::Sprite2D> skill_icons_;
    draw::Sprite2D skill_explanatory_;

    bool is_select_mode_;
    i32 select_number_;

};
}  // namespace skill
}  // namespace legend

#endif  //! LEGEND_SKILL_SKILL_SELECT_UI_H_
