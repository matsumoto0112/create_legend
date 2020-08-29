#ifndef LEGEND_SKILL_SKILL_MANAGER_H_
#define LEGEND_SKILL_SKILL_MANAGER_H_

#include "src/skill/skill.h"
#include "src/skill/skill_paste_stick.h"
#include "src/skill/skill_pencil.h"
#include "src/skill/skill_select_ui.h"
#include "src/skill/skill_type.h"

namespace legend {
namespace skill {
class SkillManager {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  SkillManager();
  /**
   * @brief �f�X�g���N�^
   */
  ~SkillManager();
  /**
   * @brief ����������
   */
  void Init(actor::IActorMediator* mediator, player::Player* player);
  /**
   * @brief �X�L�����E�����ۂ̃��\�b�h
   */
  void GetSkill(i32 skill_id);
  /**
   * @brief �X�L���̊l������
   */
  void AddSkill(std::shared_ptr<Skill> skill);
  /**
   * @brief �X�V����
   */
  void Update();
  /**
   * @brief �v���C���[�̍s�����1�x�����Ă΂�郁�\�b�h
   */
  void PlayerTurnEnd();
  /**
   * @brief �G�̍s�����1�x�����Ă΂�郁�\�b�h
   */
  void EnemyTurnEnd();
  /**
   * @brief �`�揈��
   */
  void Draw();
  /**
   * @brief ���o�����ǂ���
   */
  bool IsProductionNow();
  /**
   * @brief �X�L�����X�g�̎擾
   */
  std::vector<std::shared_ptr<Skill>> GetSkillList();
  /**
   * @brief �X�L���̍폜
   */
  void RemoveSkill();
  /**
   * @brief �X�L���̑I��
   */
  bool SelectSkill();
  /**
   * @brief �X�L���̎g�p
   */
  void UseSkill();
  /**
   * @brief �X�L���I��
   */
  void EndSkill();

  void SetPosition(std::shared_ptr<Skill> skill, i32 skill_num);

 private:
  actor::IActorMediator* mediator_;
  //! �����ł���ő�X�L����
  i32 skill_max_count_;
  //! �������Ă���X�L���p���X�g
  std::vector<std::shared_ptr<Skill>> skills_;
  //! �X�L���I����UI�p�N���X
  SkillSelectUI select_ui_;
  //! ���̃^�[����ɓ��ꂽ�X�L��
  std::vector<std::shared_ptr<Skill>> this_turn_get_skills_;

  //! ���͔���
  bool select_move_;
  //! �v���C���[
  player::Player* player_;
  //! ��
  std::vector<SkillPaste*> pastes_;
};
}  // namespace skill
}  // namespace legend

#endif  //! LEGEND_SKILL_SKILL_MANAGER_H_
