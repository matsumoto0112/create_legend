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
    util::Transform transform;
    math::Vector3 bouding_box_length;
    float mass = 1.5f;
    float restitution = 0.5f;
    float friction = 1.0f;
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
   */
  void SetPosition(math::Vector3 position);
  /**
   * @brief �ړ��ʂ̐ݒ�
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
   */
  math::Vector3 GetPosition() const;
  /**
   * @brief �ړ��ʂ̎擾
   */
  virtual math::Vector3 GetVelocity() const;
  /**
   * @brief ��]�̎擾
   */
  math::Quaternion GetRotation() const;

  /**
   * @brief �ړ��ɉ�����͂̎擾
   */
  float GetPower() const;
  /**
   * @brief �ړ��ɉ�����͂̎擾
   */
  float GetStrength() const;
  /**
   * @brief �ړ��I������̎擾
   */
  bool GetMoveEnd() const;
  /**
   * @brief �ړ��I������̃��Z�b�g
   */
  void ResetMoveEnd();
  /**
   * @brief �v���C���[�Ƃ̋���
   */
  float DistanceWithPlayer();

  enemy::enemy_type::MoveType GetMoveType() { return enemy_ai_.move_type_; };
  bullet::BoundingBox* GetCollider() const { return box_.get(); }

  virtual void OnHit(bullet::Collider* other);
  void HitAction(bullet::Collider* other);
  /**
   * @brief �����p�����[�^�̍X�V
   */
  void UpdateStrength(const float& weak);

  void CreateFireParticle(const util::Transform& transform);

 protected:
  //! �ړ�����
  bool is_move_;

  //! �ړ��ɉ������
  const float power_ = 1;

  //! �X�V����
  float update_time_;

  //! �ړ��I������
  bool move_end_;
  //! ��̉����
  float strength_;
  //! ��̉�����
  float min_strength_;

  enemy::EnemyAI enemy_ai_;

  std::shared_ptr<bullet::BoundingBox> box_;
  std::shared_ptr<draw::particle::ParticleEmitter> enemy_move_particle_;

  //! ��Q���ɏՓ˂�����
  bool is_hit_obstacle_;
  util::CountDownTimer obstacle_hit_timer_;
};

}  // namespace enemy
}  // namespace legend

#endif  //! LEGEND_ENEMY_ENEMY_ACTTOR_H_