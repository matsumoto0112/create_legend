#ifndef LEGEND_SYSTEM_PHYSICS_FIELD_H_
#define LEGEND_SYSTEM_PHYSICS_FIELD_H_

#include "src/physics/bounding_box.h"
#include "src/system/turn_system.h"

namespace legend {
namespace system {

/**
 * @class PhysicsField
 * @brief �t�B�[���h�̊Ǘ��N���X
 */
class PhysicsField {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  PhysicsField();
  /**
   * @brief �f�X�g���N�^
   */
  ~PhysicsField();
  bool Init();
  /**
   * @brief �X�V
   */
  bool Update(math::Vector3 player_vel, bool player_move, float player_impulse,
              float player_power);
  /**
   * @brief �v���C���[�����蔻��̓o�^
   * @param �v���C���[�̒�����
   */
  void SetPlayer(physics::BoundingBox player_obb);
  /**
   * @brief �G�l�~�[�����蔻��̓o�^
   * @param �G�l�~�[�̒�����
   */
  void AddEnemy(physics::BoundingBox enemy_obb);
  /**
   * @brief ���̂����蔻��̓o�^
   * @param ���̒�����
   */
  void AddDesk(physics::BoundingBox desk_obb);
  /**
   * @brief �G�l�~�[�����蔻��̍폜
   * @param �i�[�ԍ�
   */
  void RemoveEnemy(i32 index_num);
  /**
   * @brief ���̂����蔻��̍폜
   * @param �i�[�ԍ�
   */
  void RemoveDesk(i32 index_num);
  /**
   * @brief �ړ�����
   * @param ���x
   */
  void PlayerMove(math::Vector3 vel, float player_impulse, float player_power);
  /**
   * @brief ��������
   * @param ������
   */
  void Deceleration(math::Vector3& vel, float deceleration_rate, float deceleration_x,
                    float deceleration_z);
  /**
   * @brief �d�͗���
   * @param �d��
   */
  void UpdateGravity(float gravity);
  /**
   * @brief �v���C���[�����蔻��̎擾
   */
  physics::BoundingBox GetPlayerOBB() const;
  /**
   * @brief �������蔻��̎擾
   */
  physics::BoundingBox GetDeskOBB(i32 index_num) const;
  /**
   * @brief �G�l�~�[�����蔻��̎擾
   */
  physics::BoundingBox GetEnemyOBB(i32 index_num) const;
  math::Vector3 GetPlayerVelocity() const;
  bool GetPlayerMove() const;

 private:
  physics::BoundingBox player_obb_;
  std::vector<physics::BoundingBox> desk_obbs_;
  std::vector<physics::BoundingBox> enemy_obbs_;

  //! �d��
  float gravity_;
  //! �X�V����(deltaTime)
  float update_time_;

  //! �v���C���[���ړ�����
  bool is_player_move_;
  math::Vector3 player_velocity_;
};

}  // namespace system
}  // namespace legend

#endif  //! LEGEND_SYSTEM_PHYSICS_FIELD_H_
