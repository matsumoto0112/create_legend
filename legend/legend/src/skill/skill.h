#ifndef LEGEND_SKILL_SKILL_H_
#define LEGEND_SKILL_SKILL_H_

#include "src/actor/actor.h"
#include "src/bullet/bounding_box.h"
#include "src/draw/model.h"
#include "src/skill/skill_type.h"

/**
 * @file skill.h
 * @brief �X�L���̊��N���X
 */
namespace legend {
namespace player {
class Player;
}

namespace skill {

class Skill : public actor::Actor {
  using Parent = actor::Actor;

 public:
  Skill();
  virtual ~Skill();
  /**
   * @brief ������
   */
  virtual bool Init(actor::IActorMediator* mediator, player::Player* player);
  /**
   * @brief �X�V����
   */
  virtual bool Update();
  ///**
  // * @brief �`�揈��
  // */
  // void Draw() override;

  /**
   * @brief �X�L����I�����ꂽ�ۂ̃��\�b�h
   */
  virtual void Use();
  /**
   * @brief ����
   */
  virtual void Action();
  /**
   * @brief �X�L�����o�̍X�V
   */
  virtual void ProductionUpdate();
  /**
   * @brief �I��
   */
  virtual void EndAction();
  /**
   * @brief �Ďg�p�܂ł̃^�[�����̍X�V
   */
  void RemaingRecastTurnUpdate();

  void AdjustPosition(math::Vector3 adjust_position);

  // i32 GetModelID();
  i32 GetRemainingUsableCount();
  i32 GetRemainingRecastTurn();
  SkillActivationTiming GetActivetionTiming();
  SkillEffectEndTiming GetEndTiming();
  //i32 GetIconTexture();
  std::shared_ptr<directx::buffer::Texture2D> GetIconTexture() const;
  //i32 GetExplanationTexture();
  bool GetUseFlag();
  bool ProductionFlag();
  void RemoveCollider();
  /**
   * @brief �X�L���̉��o�I������擾
   */
  bool EndSkillProduction() const;

 protected:
  //! ���f����ID
  // i32 model_id_;
  //! �K��g�p�\��
  i32 usable_count_;
  //! �c��g�p�\��
  i32 remaining_usable_count_;
  //! �Ďg�p�܂ŋK��̃^�[����
  i32 recast_turn_;
  //! �c��Ďg�p�܂ł̃^�[����
  i32 remaining_recast_turn_;
  //! �X�L���̔����^�C�~���O
  SkillActivationTiming activetion_timing_;
  //! �X�L���̌��ʏI���^�C�~���O
  SkillEffectEndTiming end_timing_;
  ////! �X�L���A�C�R����ID
  // i32 skill_icon_id_;

  //! �X�L���A�C�R���̃e�N�X�`��
  std::shared_ptr<directx::buffer::Texture2D> skill_icon_texture_;
  //! �X�L�������̉摜��ID
  // i32 skill_explanation_id_;

  //! �g�p����邩�̃t���O
  bool is_use_;
  //! ���o�����ǂ���
  bool is_production_;
  //! �v���C���[
  const player::Player* player_;
  std::shared_ptr<bullet::BoundingBox> box_;
};

}  // namespace skill
}  // namespace legend

#endif  //! LEGEND_SKILL_SKILL_H_
