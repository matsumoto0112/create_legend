#ifndef LEGEND_SKILL_SKILL_MANAGER_H_
#define LEGEND_SKILL_SKILL_MANAGER_H_

#include "src/skill/skill.h"
#include "src/skill/skill_compass.h"
#include "src/skill/skill_paste_stick.h"
#include "src/skill/skill_pencil.h"
#include "src/skill/skill_select_ui.h"
#include "src/skill/skill_type.h"
#include "src/ui/player_ui.h"

/**
 * @file skill_manager.h
 * @brief �X�L���Ǘ��N���X
 */

namespace legend {
namespace skill {
enum class Mode {
  NONE,
  RISE_PLAYER,
  FALL_PLAYER,
};

/**
 * @brief �X�L���}�l�[�W���[
 */
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
  void GetSkill(std::shared_ptr<Skill> skill);
  /**
   * @brief �X�L���̊l������
   */
  void AddSkill(std::shared_ptr<Skill> skill);
  /**
   * @brief �X�V����
   */
  void Update();
  /**
   * @brief �����X�V����
   */
  void EquipmentProductionUpdate();
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
  /**
   * @brief �X�L���̍��W�ݒ�
   */
  void SetPosition(std::shared_ptr<Skill> skill, i32 skill_num);
  /**
   * @brief �I�𒆃X�L���̍��W�ݒ�
   */
  void SetPositionSelectSkill(i32 skill_num);
  /**
   * @brief UI�̕\���ؑ�
   */
  void ChangeEnable(bool enable);

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
  //! �������UI
  ui::PlayerUI player_ui_;

  //! ���݂̉��o���
  Mode current_mode_;
  //! �������o����
  bool is_equipment_production_;
  //! �������I������
  bool complete_eqquipment_;
  //! �ЃX�L�����g������
  bool paste_skill_use_;
  //! �I��ԍ�
  i32 select_skill_number_;
  //! �O�̑I��ԍ�
  i32 previous_select_number_;

  //! UI�̕\����\��
  bool ui_enable_;
  //! �������o�J�n��SE��������
  bool is_play_fly_se_;
};
}  // namespace skill
}  // namespace legend

#endif  //! LEGEND_SKILL_SKILL_MANAGER_H_
