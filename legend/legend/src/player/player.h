#ifndef LEGEND_PLAYER_PLAYER_H_
#define LEGEND_PLAYER_PLAYER_H_

#include "src/physics/collision.h"
#include "src/util/transform.h"

namespace legend {
namespace player {

/**
 * @class Player
 * @brief �v���C���[�̃N���X
 */
class Player {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  Player();
  /**
   * @brief �R���X�g���N�^
   * @param ���W
   * @param ��]
   * @param �X�P�[��
   * @param �͂̍Œ�l
   * @param �͂̍ő�l
   */
  Player(math::Vector3 position, math::Quaternion rotation, math::Vector3 scale,
         float min_power, float max_power);
  /**
   * @brief �f�X�g���N�^
   */
  ~Player();
  /**
   * @brief ������
   */
  bool Initilaize(directx::DirectX12Device& device);
  /**
   * @brief �X�V
   */
  bool Update();
  /**
   * @brief �`��
   */
  void Draw(directx::DirectX12Device& device);
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
   * @brief �ړ��ʂ̐ݒ�
   */
  void SetVelocity();
  /**
   * @brief ������͂̐ݒ�
   */
  void SetImpulse();
  /**
   * @brief �ړ��ɕK�v�ȃp�����[�^�̏�����
   */
  void ResetParameter();
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
   * @brief �����̂̎擾
   */
  physics::BoundingBox& GetOBB();

 private:
  //�Փ˔���p�̒�����
  physics::BoundingBox obb_;

  util::Transform transform_;

  //! ���x
  math::Vector3 velocity_;
  math::Vector3 input_velocity_;
  //! �O�t���[���̑��x
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
  bool is_set_power_ = false;

  //! �X�V����
  float update_time_;
};

}  // namespace player
}  // namespace legend

#endif  //! LEGEND_PLAYER_PLAYER_H_
