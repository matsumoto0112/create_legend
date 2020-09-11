#ifndef LEGEND_ENEMY_BOSS_H_
#define LEGEND_ENEMY_BOSS_H_

//#include "src/actor/actor.h"
#include "src/bullet/bounding_box.h"
#include "src/enemy/enemy_actor.h"
#include "src/enemy/enemy_type.h"
#include "src/util/transform.h"

namespace legend {
namespace enemy {

/**
 * @class Boss
 * @brief �{�X�̃N���X
 */
class Boss : public enemy::EnemyActor {
  using Parent = actor::Actor;

 public:
  /**
   * @brief �R���X�g���N�^
   */
  Boss();
  /**
   * @brief �f�X�g���N�^
   */
  ~Boss();
  /**
   * @brief ������
   */
  virtual bool Init(actor::IActorMediator* mediator,
                    const InitializeParameter& parameter);
  /**
   * @brief �X�V
   */
  virtual bool Update();
  /**
   * @brief �ړ��ʂ̐ݒ�
   */
  void SetVelocity(math::Vector3 velocity);
  /**
   * @brief �Փ˔���
   * @param �Փˑ���
   */
  virtual void OnHit(bullet::Collider* other);
  /**
   * @brief �G�l�~�[�^�C�v�ݒ�
   * @param type_index �G�^�C�v�ԍ�
   */
  virtual void SetType(i32 type_index = 0);

  /**
   * @brief �`���[�g���A���̍s��
   */
  void Boss_Tutorial();
  /**
   * @brief ���̏�ŉ�]����s��
   */
  void Boss_Rotate_Stand();
  /**
   * @brief ���i�s��
   */
  void Boss_Rush_Move();

 private:
  //! ��]���x
  float rotate_speed_ = 20.0f;
  //! ��]����
  const float rotate_time_ = 1.0f;
  //! �ғ��J�X����
  float rotate_timer_ = 0.0f;

  //! �`���[�g���A������
  bool is_tutorial_ = false;
  //! ���̏�ł̉�]����
  bool is_rotate_ = false;
  //! ���i�s������
  bool is_rush_ = false;

  //! ��]SE����
  bool is_play_spin_se_;
  //! �Փ�SE����
  bool is_play_strike_se_;
};

}  // namespace enemy
}  // namespace legend

#endif  //! LEGEND_ENEMY_BOSS_H_