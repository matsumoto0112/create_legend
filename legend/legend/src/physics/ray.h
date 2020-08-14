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
   * @param �n�_
   * @param �����x�N�g��
   */
  Ray(math::Vector3 start_position, math::Vector3 direction);
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
   * @brief �n�_�̐ݒ�
   */
  void SetStartPosition(const math::Vector3& start_position);
  /**
   * @brief �n�_�̐ݒ�
   */
  void SetDirection(const math::Vector3& direction);

 private:
  //! �n�_
  math::Vector3 start_position_;
  //! �����x�N�g��
  math::Vector3 direction_;

  primitive::Line draw_line_;
};

}  // namespace physics
}  // namespace legend

#endif  //! LEGEND_PHYSICS_RAY_H_
