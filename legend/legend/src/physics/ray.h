#ifndef LEGEND_PHYSICS_RAY_H_
#define LEGEND_PHYSICS_RAY_H_

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/constant_buffer_structure.h"
#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/math/quaternion.h"

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
   * @brief �f�t�H���g�R���X�g���N�^
   * @param �n�_
   * @param �����x�N�g��
   * @param �ő�͈�
   */
  Ray(math::Vector3 start_position, math::Vector3 direction,
      float max_distance);
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
  void Draw();
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

 private:
  // !�n�_
  math::Vector3 start_position_;
  // !�����x�N�g��
  math::Vector3 direction_;
  // !�Փ˔͈�
  float max_distance_;

  directx::buffer::VertexBuffer vertex_buffer_;
  directx::buffer::IndexBuffer index_buffer_;
  directx::buffer::ConstantBuffer<
      directx::buffer::constant_buffer_structure::Transform>
      transform_constant_buffer_;
};

}  // namespace physics
}  // namespace legend

#endif  //! LEGEND_PHYSICS_RAY_H_
