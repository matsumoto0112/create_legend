#ifndef LEGEND_PLAYER_PLAYER_H_
#define LEGEND_PLAYER_PLAYER_H_

#include "src/actor/actor.h"
#include "src/directx/buffer/constant_buffer.h"
#include "src/draw/model.h"
#include "src/physics/bounding_box.h"
#include "src/util/transform.h"

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
  void SetRotation(math::Quaternion rotation);
  /**
   * @brief �ړ��ʂ̐ݒ�
   */
  void SetVelocity();
  /**
   * @brief ������͂̐ݒ�
   */
  void SetImpulse();
  /**
   * @brief �d�͂ɂ��ړ�
   */
  void UpdateGravity(float gravity);
  /**
   * @brief �ړ��ɕK�v�ȃp�����[�^�̏�����
   */
  void ResetParameter();
  /**
   * @brief �ړ��I������̏�����
   */
  void ResetMoveEnd();
  /**
   * @brief ����
   * @param ������(1���傫���l��)
   */
  void Deceleration(float deceleration_rate);
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
   * @brief ������͂̎擾
   */
  float GetImpulse() const;
  /**
   * @brief �ړ��I������̎擾
   */
  bool GetMoveEnd() const;

 private:
  //! ���x
  math::Vector3 velocity_;
  math::Vector3 input_velocity_;
  math::Vector3 change_amount_velocity_;
  std::vector<math::Vector3> stick_velocities_;
  //! ���X�g�̍ő�l
  const i32 max_stick_velocity_num_ = 10;
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