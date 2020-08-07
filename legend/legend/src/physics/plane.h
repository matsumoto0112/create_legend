#ifndef LEGEND_PHYSICS_PLANE_H_
#define LEGEND_PHYSICS_PLANE_H_

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/constant_buffer_structure.h"
#include "src/util/transform.h"

namespace legend {
namespace physics {

/**
 * @class Plane
 * @brief ���ʃN���X
 */
class Plane {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  Plane();
  /**
   * @brief �R���X�g���N�^
   * @param ���W
   * @param �@���x�N�g��
   */
  Plane(math::Vector3 position, math::Vector3 normal);
  /**
   * @brief �f�X�g���N�^
   */
  ~Plane();
  /**
   * @brief ������
   */
  bool Initialize();
  /**
   * @brief �`��
   */
  void Draw();
  /**
   * @brief �ʒu�̐ݒ�
   */
  void SetPosition(math::Vector3 position);
  /**
   * @brief �@���x�N�g���̐ݒ�
   */
  void SetNormal(math::Vector3 normal);
  /**
   * @brief �ʒu�̎擾
   */
  math::Vector3 GetPosition() const;
  /**
   * @brief �@���x�N�g���̎擾
   */
  math::Vector3 GetNormal() const;
  /**
   * @brief �������擾
   */
  float GetDistance(math::Vector3 position) const;

 private:
  util::Transform transform_;
  //! �@���x�N�g��
  math::Vector3 normal_;

  float right_;
  float left_;
  float forward_;
  float back_;
  float up_;
  float down_;

  //! �g�����X�t�H�[���]���p�R���X�^���g�o�b�t�@
  directx::buffer::ConstantBuffer<
      directx::buffer::constant_buffer_structure::Transform>
      transform_cb_;
};

}  // namespace physics
}  // namespace legend

#endif  //! LEGEND_PHYSICS_PLANE_H_
