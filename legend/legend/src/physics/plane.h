#ifndef LEGEND_PHYSICS_PLANE_H_
#define LEGEND_PHYSICS_PLANE_H_

#include "src/physics/bounding_box.h"
#include "src/directx/shader/graphics_pipeline_state.h"
#include "src/draw/model.h"

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
  bool Initialize(directx::DirectX12Device& device);
  /**
   * @brief �`��
   */
  void Draw(directx::DirectX12Device& device);
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
  //! �ʒu
  math::Vector3 position_;
  //! �@���x�N�g��
  math::Vector3 normal_;

  float right_;
  float left_;
  float forward_;
  float back_;
  float up_;
  float down_;

  //! �g�����X�t�H�[���]���p�R���X�^���g�o�b�t�@
  directx::buffer::ConstantBuffer<directx::constant_buffer_structure::Transform>
      transform_cb_;
};

}  // namespace physics
}  // namespace legend

#endif  //! LEGEND_PHYSICS_PLANE_H_
