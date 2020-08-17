#ifndef LEGEND_SKILL_SKILL_MANAGER_H_
#define LEGEND_SKILL_SKILL_MANAGER_H_

#include "src/skill/skill.h"
#include "src/skill/skill_select_ui.h"
#include "src/skill/skill_type.h"
#include "src/skill/skill_pencil.h"

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
  void Init();
  /**
   * @brief �X�L�����E�����ۂ̃��\�b�h
   */
  void GetSkill(i32 skill_id, player::Player *player);
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

 private:
  //! �����ł���ő�X�L����
  i32 skill_max_count_;
  //! �������Ă���X�L���p���X�g
  std::vector<std::shared_ptr<Skill>> skills_;
  //! �X�L���I����UI�p�N���X
  SkillSelectUI select_ui_;
  //! ���̃^�[����ɓ��ꂽ�X�L��
  std::vector<std::shared_ptr<Skill>> this_turn_get_skills_;
};
}  // namespace skill
}  // namespace legend

#endif  //! LEGEND_SKILL_SKILL_MANAGER_H_
