#ifndef LEGEND_ENEMY_ENEMY_ACTTOR_H_
#define LEGEND_ENEMY_ENEMY_ACTTOR_H_

#include "src/actor/actor.h"
#include "src/bullet/bounding_box.h"
#include "src/draw/particle/particle_emitter.h"
#include "src/enemy/enemy_ai.h"
#include "src/enemy/enemy_type.h"
#include "src/util/transform.h"

namespace legend {
namespace enemy {

/**
 * @class Enemy
 * @brief �G�l�~�[�̃N���X
 */
class EnemyActor : public actor::Actor {
  using Parent = actor::Actor;

 public:
  /**
   * @brief �������p�����[�^
   */
  struct InitializeParameter {
    //! �g�����X�t�H�[��
    util::Transform transform;
    //! �Փ˔���
    math::Vector3 bouding_box_length;
    //! �d��
    float mass = 1.5f;
    //! ����
    float restitution = 0.5f;
    //! ���C
    float friction = 1.0f;
    //! ���f��ID
    i32 model_id = 0;
    //! ��ޔԍ�
    i32 type_index = 0;
  };

 public:
  /**
   * @brief �R���X�g���N�^
   */
  EnemyActor();
  /**
   * @brief �f�X�g���N�^
   */
  ~EnemyActor();
  /**
   * @brief ������
   * @param mediator �����
   * @param parameter �������p�����[�^
   * @return ������������TRUE
   */
  virtual bool Init(actor::IActorMediator* mediator,
                    const InitializeParameter& parameter);
  /**
   * @brief �폜
   */
  virtual void Remove();
  /**
   * @brief �X�V
   */
  virtual bool Update();
  /**
   * @brief �`��
   */
  virtual void Draw();

  /**
   * @brief ���W�̐ݒ�
   * @param position ���W
   */
  void SetPosition(math::Vector3 position);
  /**
   * @brief ���x�̐ݒ�
   * @param velocity ���x
   */
  void SetVelocity(math::Vector3 velocity);
  /**
   * @brief ��]�ʂ̐ݒ�
   */
  void SetRotation();
  /**
   * @brief �ړ��ɕK�v�ȃp�����[�^�̏�����
   */
  void ResetParameter();
  /**
   * @brief ���W�̎擾
   * @return ���W�擾
   */
  math::Vector3 GetPosition() const { return transform_.GetPosition(); }
  /**
   * @brief �ړ��ʂ̎擾
   * @return ���x�擾
   */
  virtual math::Vector3 GetVelocity() const { return (box_->GetVelocity()); }
  /**
   * @brief ��]�̎擾
   * @return ��]�擾
   */
  math::Quaternion GetRotation() const { return transform_.GetRotation(); }

  /**
   * @brief �ړ��ɉ�����͂̎擾
   * @return �p���[�擾
   */
  float GetPower() const { return power_; }
  /**
   * @brief �ړ��ɉ�����͂̎擾
   * @return �����l�擾
   */
  float GetStrength() const { return strength_; }
  /**
   * @brief �ړ��I������̎擾
   * @return �ړ��I�����Ă��邩����
   */
  bool GetMoveEnd() const { return (!is_move_ && move_end_); }
  /**
   * @brief �ړ��I������̃��Z�b�g
   */
  void ResetMoveEnd() { move_end_ = false; }
  /**
   * @brief �v���C���[�Ƃ̋���
   * @return �����̎擾
   */
  float DistanceWithPlayer();
  /**
   * @brief �ړ��^�C�v�擾
   * @return �ړ��^�C�v�擾
   */
  enemy::enemy_type::MoveType GetMoveType() { return enemy_ai_.move_type_; };
  /**
   * @brief �Փˎ擾
   * @return �Փˎ擾
   */
  bullet::BoundingBox* GetCollider() const { return box_.get(); }
  /**
   * @brief �G�l�~�[�^�C�v�ݒ�
   * @param type_index �G�^�C�v�ԍ�
   */
  virtual void SetType(i32 type_index = 0);
  /**
   * @brief �Փ˔���
   * @param �Փˑ���
   */
  virtual void OnHit(bullet::Collider* other);
  /**
   * @brief �Փˎ��s��
   * @param �Փˑ���
   */
  void HitAction(bullet::Collider* other);
  /**
   * @brief �����p�����[�^�̍X�V
   * @param �ύX�l
   */
  void UpdateStrength(const float& weak);
  /**
   * @brief �p�[�e�B�N������
   * @param �g�����X�t�H�[��
   */
  void CreateFireParticle(const util::Transform& transform);

 protected:
  //! �ړ�����
  bool is_move_;
  //! ��x�̃A�N�V�����ł̏Փ˔���
  bool is_hit_done_;

  //! �ړ��ɉ������
  const float power_ = 1;

  //! �X�V����
  float update_time_;

  //! �ړ��I������
  bool move_end_;
  //! �����l
  float strength_;
  //! �����l�Œ�l
  float min_strength_;

  //! �G�l�~�[AI
  enemy::EnemyAI enemy_ai_;

  //! �Փ˔���
  std::shared_ptr<bullet::BoundingBox> box_;
  //! �p�[�e�B�N��
  std::shared_ptr<draw::particle::ParticleEmitter> enemy_move_particle_;

  //! ��Q���ɏՓ˂�����
  bool is_hit_obstacle_;
  //! �J�E���g�_�E���^�C�}�[
  util::CountDownTimer obstacle_hit_timer_;
};

}  // namespace enemy
}  // namespace legend

#endif  //! LEGEND_ENEMY_ENEMY_ACTTOR_H_