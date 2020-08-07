#ifndef LEGEND_PLAYER_PLAYER_H_
#define LEGEND_PLAYER_PLAYER_H_

#include "src/actor/actor.h"
#include "src/physics/bounding_box.h"

namespace legend {
namespace player {

/**
 * @class Player
 * @brief �v���C���[�̃N���X
 */
class Player : public actor::Actor<physics::BoundingBox> {
 public:
  /**
   * @brief �������p�����[�^
   */
  struct InitializeParameter {
    util::Transform transform;
    math::Vector3 bouding_box_length;
    float min_power;
    float max_power;
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
  virtual bool Init(const InitializeParameter& parameter);
  /**
   * @brief �X�V
   */
  bool Update();
  /**
   * @brief ���W�̐ݒ�
   */
  void SetPosition(math::Vector3 position);
  /**
   * @brief ��]�ʂ̐ݒ�
   */
  void SetRotation(math::Quaternion rotation);
  /**
   * @brief �ړ��ʂ̐ݒ�
   */
  void SetVelocity();
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
   * @brief �ړ��I������̏�����
   */
  void ResetMoveEnd();
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
   * @brief ������͂̊����̎擾
   */
  float GetImpulse() const;
  /**
   * @brief ������͂̎擾
   */
  float GetPower();
  /**
   * @brief �ړ��I������̎擾
   */
  bool GetMoveEnd() const;
  /**
   * @brief �ړ�����̎擾
   */
  bool GetIsMove() const;

 private:
  //! ���x
  math::Vector3 velocity_;
  math::Vector3 input_velocity_;
  math::Vector3 change_amount_velocity_;
  //! ������
  float deceleration_x_;
  float deceleration_z_;
  //! �ړ�����
  bool is_move_;

  //���x�̍X�V����
  float velocity_update_time_;
  const float change_time_ = 0.1f;

  //! �ړ��ɉ������
  const float power_ = 10;
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
  //! �ړ��I������
  bool move_end_;

  //! �X�V����
  float update_time_;
};

}  // namespace player
}  // namespace legend

#endif  //! LEGEND_PLAYER_PLAYER_H_
