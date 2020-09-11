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
namespace skill_name {
constexpr const wchar_t* SKILLPENCIL = L"SkiilPencil";
constexpr const wchar_t* SKILLPASTESTICK = L"SkiilPasteStick";
constexpr const wchar_t* SKILLCOMPASS = L"SkiilCompass";
}  // namespace skill_name

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
  /**
   * @brief �����X�L�����Ɉʒu�𒲐�
   */
  void AdjustPosition(math::Vector3 position);
  /**
   * @brief �I�𒆂̃X�L���̈ʒu��ύX
   */
  void ChangePosition(math::Vector3 position);
  /**
   * @brief �c��g�p�񐔂̎擾
   */
  i32 GetRemainingUsableCount();
  /**
   * @brief ���L���X�g�^�[���̎擾
   */
  i32 GetRemainingRecastTurn();
  /**
   * @brief �X�L�������^�C�~���O�̎擾
   */
  SkillActivationTiming GetActivetionTiming();
  /**
   * @brief �X�L���I���^�C�~���O�̎擾
   */
  SkillEffectEndTiming GetEndTiming();
  /**
   * @brief �X�L���A�C�R���摜�̎擾
   */
  std::shared_ptr<directx::buffer::Texture2D> GetIconTexture() const;
  /**
   * @brief �X�L���g�p���A�C�R���̎擾
   */
  std::shared_ptr<directx::buffer::Texture2D> GetIconUsingTexture() const;
  /**
   * @brief �����X�L���摜�̎擾
   */
  std::shared_ptr<directx::buffer::Texture2D> GetEquipmentTexture() const;
  /**
   * @brief �X�L�������摜�̎擾
   */
  std::shared_ptr<directx::buffer::Texture2D> GetExplanationTexture() const;
  /**
   * @brief �X�L���g�p�t���O�̎擾
   */
  bool GetUseFlag();
  /**
   * @brief ���o���t���O�̎擾
   */
  bool ProductionFlag();
  /**
   * @brief �R���C�_�[�̍폜
   */
  void RemoveCollider();
  /**
   * @brief �X�L���̉��o�I������擾
   */
  bool EndSkillProduction() const;
  /**
   * @brief �X�L�����̐ݒ�
   */
  void SetName(const std::wstring name);
  /**
   * @brief �X�L�����̎擾
   */
  std::wstring GetName() const;

 protected:
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

  //! �X�L���A�C�R���̃e�N�X�`��
  std::shared_ptr<directx::buffer::Texture2D> skill_icon_texture_;
  //! �X�L���g�p���A�C�R���̃e�N�X�`��
  std::shared_ptr<directx::buffer::Texture2D> skill_icon_using_texture_;
  //! �X�L�������̃e�N�X�`��
  std::shared_ptr<directx::buffer::Texture2D> skill_explanation_texture_;
  //! ������ԕ\���p�̃e�N�X�`��
  std::shared_ptr<directx::buffer::Texture2D> equipment_texture_;

  //! �g�p����邩�̃t���O
  bool is_use_;
  //! ���o�����ǂ���
  bool is_production_;
  //! �v���C���[
  const player::Player* player_;
  //! �R���C�_�[
  std::shared_ptr<bullet::BoundingBox> box_;
};

}  // namespace skill
}  // namespace legend

#endif  //! LEGEND_SKILL_SKILL_H_
