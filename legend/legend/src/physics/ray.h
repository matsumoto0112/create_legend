#ifndef LEGEND_PHYSICS_RAY_H_
#define LEGEND_PHYSICS_RAY_H_

#include "src/primitive/line.h"

namespace legend {
namespace physics {

/**
 * @class Ray
 * @brief ���C�i�����j�N���X
 */
class Ray {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  Ray();
  /**
   * @brief �R���X�g���N�^
   * @param �����x�N�g��
   * @param �ő�͈�
   */
  Ray(math::Vector3 direction, float max_distance);
  /**
   * @brief �R���X�g���N�^
   * @param �n�_
   * @param �����x�N�g��
   * @param �ő�͈�
   * @param ��]
   */
  Ray(math::Vector3 start_position, math::Vector3 direction,
      float max_distance, math::Quaternion rotation);
  /**
   * @brief �f�X�g���N�^
   */
  ~Ray();
  /**
   * @brief ������
   */
  bool Initialize();
  /**
   * @brief �X�V
   */
  void Update();
  /**
   * @brief �`��
   */
  void Draw(directx::device::CommandList& command_list);
  /**
   * @brief �n�_�̎擾
   */
  math::Vector3 GetStartPosition() const;
  /**
   * @brief �����x�N�g���̎擾
   */
  math::Vector3 GetDirection() const;
  /**
   * @brief �ő�͈͂̎擾
   */
  float GetDistance() const;
  /**
   * @brief ��]�̎擾
   */
  math::Quaternion GetRotation() const;
  /**
   * @brief �n�_�̐ݒ�
   */
  void SetStartPosition(const math::Vector3& start_position);
  /**
   * @brief ��]�̐ݒ�
   */
  void SetRotation(const math::Quaternion& rotation);

 private:
  //! �n�_
  math::Vector3 start_position_;
  //! �����x�N�g��
  math::Vector3 direction_;
  //! �Փ˔͈�
  float max_distance_;
  //! ��]
  math::Quaternion rotation_;

  primitive::Line draw_line_;
};

}  // namespace physics
}  // namespace legend

#endif  //! LEGEND_PHYSICS_RAY_H_
