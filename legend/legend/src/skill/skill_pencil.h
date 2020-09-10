#ifndef LEGEND_SKILL_SKILL_PENCIL_H_
#define LEGEND_SKILL_SKILL_PENCIL_H_

#include "src/bullet/bounding_sphere.h"
#include "src/skill/explosion_pencil.h"
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
  /**
   * @brief �Փ˔���
   */
  void OnHit(bullet::Collider* other);
  /**
   * @brief �����J�n
   */
  void Explosion(util::Transform transform);
  /**
   * @brief �����X�V
   */
  void ExplosionUpdate();

private:
    /**
     * @brief target�Ɍ�������]������
     * @param ����
     */
    math::Quaternion LookAt(const math::Vector3& target);

 private:
  //! �^�C�}�[
  util::CountDownTimer explosion_timer_;
  //! �����N���X
  std::shared_ptr<ExplosionPencil> explosion_pencil_;
  //! ��������
  bool is_explosion_;
  //! �ˏo�p�x
  float shoot_theta_;
  math::Vector3 velocity_;
};

}  // namespace skill
}  // namespace legend
#endif  //! LEGEND_SKILL_SKILL_PENCIL_H_