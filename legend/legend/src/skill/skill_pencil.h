#ifndef LEGEND_SKILL_SKILL_PENCIL_H_
#define LEGEND_SKILL_SKILL_PENCIL_H_

#include "src/bullet/bounding_box.h"
#include "src/skill/skill.h"
#include "src/util/timer.h"

namespace legend {
namespace skill {

class SkillPencil : public Skill {
  using Parent = Skill;

 public:
  /**
   * @brief �R���X�g���N�^
   */
  SkillPencil();
  /**
   * @brief �f�X�g���N�^
   */
  ~SkillPencil();

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
  void OnHit(bullet::Collider* other);
  /**
   * @brief ����
   */
  void Explosion();

 private:
  //! �^�C�}�[
  util::CountDownTimer explosion_timer_;
  std::shared_ptr<bullet::BoundingBox> box_;
};

}  // namespace skill
}  // namespace legend
#endif  //! LEGEND_SKILL_SKILL_PENCIL_H_