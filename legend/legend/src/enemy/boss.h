#ifndef LEGEND_ENEMY_BOSS_H_
#define LEGEND_ENEMY_BOSS_H_

#include "src/actor/actor.h"
#include "src/bullet/bounding_box.h"
#include "src/util/transform.h"
#include "src/enemy/enemy_type.h"

namespace legend {
namespace enemy {

/**
 * @class Boss
 * @brief �{�X�̃N���X
 */
class Boss : public actor::Actor {
  using Parent = actor::Actor;

 public:
  /**
   * @brief �������p�����[�^
   */
  struct InitializeParameter {
    util::Transform transform;
    math::Vector3 bouding_box_length;
    float mass = 4.0f;
    float restitution = 0.0f;
    float friction = 1.5f;
  };

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
   * @brief �폜
   */
  virtual void Remove();
  /**
   * @brief �X�V
   */
  bool Update();
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

  enemy::enemy_type::MoveType GetMoveType() { return move_type_; };
  bullet::BoundingBox* GetCollider() const { return box_.get(); }

  void OnHit(bullet::Collider* other);
  void HitAction(bullet::Collider* other);

 private:
  //! �ړ�����
  bool is_move_;

  //! �ړ��ɉ������
  const float power_ = 1;

  //! �X�V����
  float update_time_;

  //! �ړ��I������
  bool move_end_;

  //! �ړ��^�C�v
  enemy::enemy_type::MoveType move_type_;
  //! �Փ˃^�C�v
  enemy::enemy_type::HitType hit_type_;

  std::shared_ptr<bullet::BoundingBox> box_;
};

}  // namespace enemy
}  // namespace legend

#endif  //! LEGEND_ENEMY_BOSS_H_