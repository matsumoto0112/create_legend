#ifndef LEGEND_SKILL_SKILL_SELECT_UI_H_
#define LEGEND_SKILL_SKILL_SELECTUI_H_

#include "src/draw/sprite_renderer.h"
#include "src/draw/texture_string.h"
#include "src/skill/skill.h"

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
   * @brief �ǉ�����
   */
  void AddSkill(const Skill* skill);
  /**
   * @brief �폜����
   */
  void RemoveSkillUI(i32 index_num);
  /**
   * @brief �I�𒆂����擾
   */
  bool GetIsSelectMode() const;
  /**
   * @brief �I�����[�h�ؑ�
   */
  void ChangeIsSelectMode();
  /**
   * @brief �I�𒆂̃X�L���ԍ��̎擾
   */
  i32 GetSkillNumber();
  /**
   * @brief �I�𒆂̃X�L���ԍ��̍X�V
   */
  void SelectSkillNumber(i32 select_number);

 private:
  //! �X�L���A�C�R���̊�{���W
  math::Vector2 icon_base_position_;
  //! �X�L���A�C�R���̃X�P�[�����O
  math::Vector2 icon_scale_;

  //! �`�悷��X�L���A�C�R���̃��X�g
  std::vector<draw::Sprite2D> skill_icons_;
  //! �X�L���̐����摜
  draw::Sprite2D skill_explanatory_;
  //! �X�L���̑I�𒆃t���[���摜
  draw::Sprite2D skill_select_frame_;

  //! �X�L���I�����[�h��
  bool is_select_mode_;
  //! �I��ԍ�
  i32 select_number_;
};
}  // namespace skill
}  // namespace legend

#endif  //! LEGEND_SKILL_SKILL_SELECT_UI_H_
