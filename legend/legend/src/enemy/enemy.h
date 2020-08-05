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
class Enemy {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  Enemy();
  /**
   * @brief �R���X�g���N�^
   * @param ���W
   * @param ��]
   * @param �X�P�[��
   * @param �͂̍Œ�l
   * @param �͂̍ő�l
   */
  Enemy(math::Vector3 position, math::Quaternion rotation, math::Vector3 scale);
  /**
   * @brief �f�X�g���N�^
   */
  ~Enemy();
  /**
   * @brief ������
   */
  bool Initilaize(directx::DirectX12Device& device,
                  util::resource::Resource& resource);
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
   * @brief ��]�ʂ̐ݒ�
   */
  void SetRotation();
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
   * @brief �����̂̎擾
   */
  physics::BoundingBox& GetOBB();

 private:
  //�Փ˔���p�̒�����
  physics::BoundingBox obb_;

  //! �g�����X�t�H�[���]���p�R���X�^���g�o�b�t�@
  directx::buffer::ConstantBuffer<directx::constant_buffer_structure::Transform>
      transform_cb_;
  //! �g�����X�t�H�[��
  util::Transform transform_;

  //! ���x
  math::Vector3 velocity_;
  //! ������
  float deceleration_x_;
  float deceleration_z_;
  //! �ړ�����
  bool is_move_;

  //! �ړ��ɉ������
  const float power_ = 10;
  ////! ���ۂɉ�����͂̉���
  //float impulse_;

  //! �X�V����
  float update_time_;
};

}  // namespace enemy
}  // namespace legend

#endif  //! LEGEND_ENEMY_ENEMY_H_