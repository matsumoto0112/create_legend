#ifndef LEGEND_PLAYER_PLAYER_H_
#define LEGEND_PLAYER_PLAYER_H_

#include "src/physics/collision.h"

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
  Player(math::Vector3 position, math::Vector3 rotation, math::Vector3 scale,
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
  void SetPower();
  /**
   * @brief �d�͂̔��f
   */
  void UseGravity();
  /**
   * @brief �ړ��ɕK�v�ȃp�����[�^�̏�����
   */
  void ResetParameter();
  /**
   * @brief ����
   */
  void Deceleration();
  /**
   * @brief ���W�̎擾
   */
  math::Vector3 GetPosition() const;
  /**
   * @brief �ړ��ʂ̎擾
   */
  math::Vector3 GetVelocity() const;
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

  //! ���S���W
  math::Vector3 position_;
  //! ��]
  math::Vector3 rotation_;
  //! �X�P�[��
  math::Vector3 scale_;

  //! ���x
  math::Vector3 velocity_;
  //! �O�t���[���̑��x
  math::Vector3 before_velocity_;
  std::vector<math::Vector3> stick_velocities_;
  //! ���X�g�̍ő�l
  const i32 max_stick_velocity_num_ = 10;
  //! ������
  float deceleration_x_;
  float deceleration_z_;
  //! �ړ�����
  bool is_move_;

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
  bool up_power_ = true;
  //! �p���[�ݒ�I����
  bool is_set_power_ = false;

  //! �X�V����
  const float update_time_ = 1 / 60.0f;

  //! �d��
  const float gravity_ = -9.8f;
};

}  // namespace player
}  // namespace legend

#endif  //! LEGEND_PLAYER_PLAYER_H_
