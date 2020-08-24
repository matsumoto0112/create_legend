#ifndef LEGEND_PLAYER_PLAYER_H_
#define LEGEND_PLAYER_PLAYER_H_

#include "src/actor/actor.h"
#include "src/bullet/bounding_box.h"
#include "src/skill/skill_manager.h"

namespace legend {
namespace player {

/**
 * @class Player
 * @brief �v���C���[�̃N���X
 */
class Player : public actor::Actor {
  using Parent = actor::Actor;

 public:
  /**
   * @brief �������p�����[�^
   */
  struct InitializeParameter {
    util::Transform transform;
    math::Vector3 bouding_box_length;
    float min_power;
    float max_power;
    float min_strength;
    float max_strength;
  };

 public:
  /**
   * @brief �R���X�g���N�^
   */
  Player();
  /**
   * @brief �f�X�g���N�^
   */
  ~Player();
  /**
   * @brief ������
   */
  virtual bool Init(actor::IActorMediator* mediator,
                    const InitializeParameter& parameter);
  /**
   * @brief �X�V
   */
  bool Update();
  /**
   * @brief �`��
   */
  void Draw() override;
  /**
   * @brief ���W�̐ݒ�
   */
  void SetPosition(math::Vector3 position);
  /**
   * @brief ��]�ʂ̐ݒ�
   */
  void SetRotation(math::Quaternion rotation);
  /**
   * @brief �X�P�[���̐ݒ�
   */
  void SetScale(math::Vector3 scale);
  /**
   * @brief �ړ��ʂ̐ݒ�
   */
  void CheckImpulse();
  /**
   * @brief �ړ��ʂ̐ݒ�
   */
  void SetVelocity(math::Vector3 velocity);
  /**
   * @brief ������͂̐ݒ�
   */
  void SetImpulse();
  /**
   * @brief �ړ��ɕK�v�ȃp�����[�^�̏�����
   */
  void ResetParameter();
  /**
   * @brief ���W�̎擾
   */
  math::Vector3 GetPosition() const;
  /**
   * @brief �X�P�[���̎擾
   */
  math::Vector3 GetScale() const;
  /**
   * @brief �ړ��ʂ̎擾
   */
  math::Vector3 GetVelocity() const;
  /**
   * @brief ��]�̎擾
   */
  math::Quaternion GetRotation() const;
  /**
   * @brief ������͂̊����̎擾
   */
  float GetImpulse() const;
  /**
   * @brief ������͂̎擾
   */
  float GetPower() const;
  /**
   * @brief �s���I���̎擾
   */
  bool GetMoveEnd() const;
  /**
   * @brief �ړ�����̎擾
   */
  bool GetIsMove() const;
  /**
   * @brief �����p�����[�^�̍X�V
   */
  void UpdateStrength(const float& add_strength);
  /**
   * @brief ���݂̋����x�������擾
   */
  float GetStrength() const;
  /**
   * @brief �X�L���I�𒆂����擾
   */
  bool GetSkillSelect();
  /**
   * @brief �X�L���I�𒆂����擾
   */
  bool GetPlayerDeathFlag();
  /**
   * @brief �ʂ̃R���C�_�[�ƏՓ˂����Ƃ��̃R�[���o�b�N
   */
  void OnHit(bullet::Collider* other);

  bullet::Collider* GetCollider() const { return box_.get(); }

  void SkillUpdate();

 private:
  //! ���x
  math::Vector3 velocity_;
  math::Vector3 input_velocity_;
  math::Vector3 change_amount_velocity_;
  //! �ړ�����
  bool is_move_;

  //���x�̍X�V����
  float velocity_update_time_;
  const float change_time_ = 0.1f;

  //! �ړ��ɉ������
  const float power_ = 50.0f;
  //! ���ۂɉ�����͂̉���
  float impulse_;
  //! �ŏ��l
  float min_power_;
  //! �ő�l
  float max_power_;
  //! ���͏�Ԃ�
  bool is_input_;

  //! �Q�[�W���㏸���ǂ���
  bool up_power_;
  //! �p���[�ݒ�I����
  bool is_set_power_;

  //! �X�V����
  float update_time_;

  //! �����p�����[�^
  float strength_;
  //! �������
  float max_strength_;
  //! ��������
  float min_strength_;

  //! �X�L���}�l�[�W���[
  skill::SkillManager skill_manager_;

  //! �R���C�_�[
  std::shared_ptr<bullet::BoundingBox> box_;
};

}  // namespace player
}  // namespace legend

#endif  //! LEGEND_PLAYER_PLAYER_H_
