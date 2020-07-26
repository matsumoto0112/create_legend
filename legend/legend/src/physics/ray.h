#ifndef LEGEND_PHYSICS_RAY_H_
#define LEGEND_PHYSICS_RAY_H_

#include "src/physics/bounding_box.h"

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
  Ray(math::Vector3 start_position, math::Vector3 direction, float max_distance);
  /**
   * @brief �f�X�g���N�^
   */
  ~Ray();
  /**
   * @brief ������
   */
  bool Initialize(directx::DirectX12Device& device);
  /**
   * @brief �X�V
   */
  void Update();
  /**
   * @brief �`��
   */
  void Draw(directx::DirectX12Device& device);
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
  directx::buffer::ConstantBuffer<Transform> transform_constant_buffer_;

  directx::buffer::ConstantBuffer<WorldContext> world_constant_buffer_;
  directx::shader::GraphicsPipelineState pipeline_state_;
};

}  // namespace physics
}  // namespace legend

#endif  //! LEGEND_PHYSICS_RAY_H_
