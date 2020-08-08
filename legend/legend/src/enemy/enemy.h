#ifndef LEGEND_ENEMY_ENEMY_H_
#define LEGEND_ENEMY_ENEMY_H_

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/shader/graphics_pipeline_state.h"
#include "src/draw/model.h"
#include "src/physics/collision.h"
#include "src/util/transform.h"

namespace legend {
namespace enemy {

/**
 * @class Enemy
 * @brief �G�l�~�[�̃N���X
 */
class Enemy : public actor::Actor<physics::BoundingBox> {
 public:
  /**
   * @brief �������p�����[�^
   */
  struct InitializeParameter {
    util::Transform transform;
    math::Vector3 bouding_box_length;
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
  virtual bool Init(const InitializeParameter& parameter);
  /**
   * @brief �X�V
   */
  bool Update();
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

 private:
  //! ���x
  math::Vector3 velocity_;
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
};

}  // namespace enemy
}  // namespace legend

#endif  //! LEGEND_ENEMY_ENEMY_H_