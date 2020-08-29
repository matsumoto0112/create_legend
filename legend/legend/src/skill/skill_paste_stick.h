#ifndef LEGEND_SKILL_SKILL_PASTE_STICK_H_
#define LEGEND_SKILL_SKILL_PASTE_STICK_H_

#include "src/skill/skill.h"
#include "src/skill/skill_paste.h"

namespace legend {
namespace skill {
class SkillPasteStick : public Skill {
  using Parent = Skill;

 public:
  /**
   * @brief �R���X�g���N�^
   */
  SkillPasteStick();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~SkillPasteStick();
  /**
   * @brief ������
   */
  bool Init(actor::IActorMediator* mediator, player::Player* player) override;
  /**
   * @brief �X�V����
   */
  bool Update() override;
  /**
   * @brief �`�揈��
   */
  void Draw() override;
  /**
   * @brief �X�L����I�����ꂽ�ۂ̃��\�b�h
   */
  void Use() override;
  /**
   * @brief ����
   */
  void Action() override;
  /**
   * @brief �X�L�����o�̍X�V
   */
  void ProductionUpdate() override;
  /**
   * @brief �I��
   */
  void EndAction() override;

 private:
  //! ��
  std::vector<std::shared_ptr<SkillPaste>> pastes_;
  //! ������
  i32 instance_count_;
};
}  // namespace skill
}  // namespace legend

#endif  //! LEGEND_SKILL_SKILL_PASTE_STICK_H_
