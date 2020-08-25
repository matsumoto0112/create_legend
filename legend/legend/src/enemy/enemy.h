#ifndef LEGEND_ENEMY_ENEMY_H_
#define LEGEND_ENEMY_ENEMY_H_

#include "src/actor/actor.h"
#include "src/bullet/bounding_box.h"
#include "src/util/transform.h"

namespace legend {
namespace enemy {

/**
 * @class Enemy
 * @brief �G�l�~�[�̃N���X
 */
class Enemy : public actor::Actor {
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
  };

 public:
  /**
   * @brief �R���X�g���N�^
   */
  Enemy();
  /**
   * @brief �f�X�g���N�^
   */
  ~Enemy();
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
  bool Update();
  /**
   * @brief �`��
   */
  virtual void Draw();

  /**
   * @brief �ړ�
   */
  void Move();
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
  ///**
  // * @brief ����
  // * @param ������(1���傫���l��)
  // */
  // void Deceleration(float deceleration_rate);
  /**
   * @brief ���W�̎擾
   */
  math::Vector3 GetPosition() const;
  /**
   * @brief �ړ��ʂ̎擾
   */
  math::Vector3 GetVelocity() const;
  /**
   * @brief ��]�̎擾
   */
  math::Quaternion GetRotation() const;

  /**
   * @brief �ړ��ɉ�����͂̎擾
   */
  float GetPower() const;
  /**
   * @brief �ړ��I������̎擾
   */
  bool GetMoveEnd() const;
  /**
   * @brief �ړ��I������̃��Z�b�g
   */
  void ResetMoveEnd();

  bullet::BoundingBox* GetCollider() const { return box_.get(); }

  void OnHit(bullet::Collider* other);
  /**
   * @brief ��̉�
   */
  void Weaking(const float& weak);

 private:
  ////! ���x
  //math::Vector3 velocity_;
  ////! ������
  // float deceleration_x_;
  // float deceleration_z_;
  //! �ړ�����
  bool is_move_;

  //! �ړ��ɉ������
  const float power_ = 1;
  ////! ���ۂɉ�����͂̉���
  // float impulse_;

  //! �X�V����
  float update_time_;

  //! �ړ��I������
  bool move_end_;
  //! ��̉����
  float strength_;
  //! ��̉�����
  float min_strength_;

  std::shared_ptr<bullet::BoundingBox> box_;
};

}  // namespace enemy
}  // namespace legend

#endif  //! LEGEND_ENEMY_ENEMY_H_