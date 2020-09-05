#ifndef LEGEND_SKILL_SKILL_ITEM_BOX_H_
#define LEGEND_SKILL_SKILL_ITEM_BOX_H_

#include "src/actor/actor.h"
#include "src/bullet/bounding_box.h"
#include "src/draw/particle/particle_emitter.h"
#include "src/skill/skill.h"
#include "src/util/timer.h"

namespace legend {
namespace skill {

class SkillItemBox : public actor::Actor {
  using Parent = actor::Actor;

 public:
  /**
   * @brief �������p�����[�^
   */
  struct InitializeParameter {
    util::Transform transform;
    math::Vector3 bounding_box_length;
    i32 skill_icon_model_num;
  };

 public:
  /**
   * @brief �R���X�g���N�^
   */
  SkillItemBox();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~SkillItemBox();
  /**
   * @brief ������
   * @param �����
   * @param �p�����[�^
   * @param �X�L��
   */
  virtual bool Init(actor::IActorMediator* mediator,
                    const InitializeParameter& parameter,
                    std::shared_ptr<Skill> skill);
  /**
   * @brief �X�V
   */
  bool Update() override;
  /**
   * @brief �`��
   */
  void Draw() override;

  /**
   * @brief ���S����̕ύX
   */
  void ChangeDead();
  /**
   * @brief ���S����̎擾
   */
  bool GetIsDead() const;
  /**
   * @brief �ݒ肳�ꂽ�X�L���̎擾
   */
  std::shared_ptr<Skill> GetSkill() const;

 private:
  //! ���S����
  bool is_dead_;
  //! �X�L��
  std::shared_ptr<Skill> skill_;
  //! �폜����
  util::CountDownTimer delete_time_;
  //! �R���C�_�[
  std::shared_ptr<bullet::BoundingBox> box_;
  //! �X�L�����e�̃A�C�R�����f��
  std::shared_ptr<draw::Model> skill_icon_model_;
  directx::buffer::ConstantBuffer<
      directx::buffer::constant_buffer_structure::Transform>
      skill_icon_transform_cb_;
  //! �ړ��p�[�e�B�N��
  std::shared_ptr<draw::particle::ParticleEmitter> move_particle_;
  //! ���x
  math::Vector3 velocity_;
};

}  // namespace skill
}  // namespace legend

#endif  //! LEGEND_SKILL_SKILL_ITEM_BOX_H_
