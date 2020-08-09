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
  bool Update(Turn turn, math::Vector3 player_vel, bool player_move,
              float player_impulse, float player_power,
              std::vector<math::Vector3> enemies_vel, bool enemy_move);
  /**
   * @brief �v���C���[�����蔻��̓o�^
   * @param �v���C���[�̒�����
   */
  void SetPlayer(physics::BoundingBox& player_obb);
  /**
   * @brief �G�l�~�[�����蔻��̓o�^
   * @param �G�l�~�[�̒�����
   */
  void AddEnemy(physics::BoundingBox& enemy_obb);
  /**
   * @brief ���̂����蔻��̓o�^
   * @param ���̒�����
   */
  void AddDesk(physics::BoundingBox& desk_obb);
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
   * @brief �v���C���[�̈ړ�����
   * @param ���x
   */
  void PlayerMove(math::Vector3 vel, float player_impulse, float player_power);
  /**
   * @brief �G�l�~�[�̈ړ�����
   * @param ���x
   */
  void EnemyMove(std::vector<math::Vector3> enemies_vel);
  /**
   * @brief ��������
   * @param ������
   */
  void Deceleration(math::Vector3& vel, float deceleration_rate,
                    float deceleration_x, float deceleration_z);
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
  /**
   * @brief �X�V�����v���C���[�̑��x�̎擾
   */
  math::Vector3 GetPlayerVelocity() const;

  /**
   * @brief �X�V�����e�G�l�~�[�̑��x�̎擾
   */
  math::Vector3 GetEnemyVelocity(i32 index_num) const;
  /**
   * @brief �G�l�~�[�̈ړ�����̃��Z�b�g
   */
  void ResetEnemyMove();

 private:
  //! �v���C���[�̒�����
  physics::BoundingBox player_obb_;
  //! �G�l�~�[�̒�����
  std::vector<physics::BoundingBox> desk_obbs_;
  //! ���̒�����
  std::vector<physics::BoundingBox> enemy_obbs_;

  //! �d��
  float gravity_;
  //! �X�V����(deltaTime)
  float update_time_;

  float player_deceleration_x_;
  float player_deceleration_z_;
  std::vector<float>enemy_deceleration_x_;
  std::vector<float>enemy_deceleration_z_;

  //! �v���C���[���ړ�����
  bool is_player_move_;
  //! �v���C���[�̑��x
  math::Vector3 player_velocity_;

  //! �e�G�l�~�[�̑��x
  std::vector<math::Vector3> enemy_velocities_;
  //! �e�G�l�~�[�̈ړ��J�n����
  std::vector<bool> is_enemy_move_;
};

}  // namespace system
}  // namespace legend

#endif  //! LEGEND_SYSTEM_PHYSICS_FIELD_H_
