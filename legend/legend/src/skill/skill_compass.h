#ifndef LEGEND_SKILL_SKILL_COMPASS_H_
#define LEGEND_SKILL_SKILL_COMPASS_H_

#include "src/skill/skill.h"

namespace legend {
namespace skill {

class SkillCompass : public Skill {
  using Parent = Skill;

 public:
  /**
   * @brief �R���X�g���N�^
   */
  SkillCompass();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~SkillCompass();
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
  /**
   * @brief �Փ˔���
   */
  void OnHit(bullet::Collider* other);

 private:
  //! �e����΂���
  float flip_off_power_;
  //! �o�ߎ���
  float elapsed_time_;
};

}  // namespace skill
}  // namespace legend

#endif  //! LEGEND_SKILL_SKILL_COMPASS_H_
