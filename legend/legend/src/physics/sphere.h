#ifndef LEGEND_PHYSICS_SPHERE_H_
#define LEGEND_PHYSICS_SPHERE_H_

#include "src/physics/bounding_box.h"

namespace legend {
namespace physics {
/**
 * @class Sphere
 * @brief ���N���X
 */
class Sphere {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  Sphere();
  /**
   * @brief �f�X�g���N�^
   */
  ~Sphere();
  /**
   * @brief ������
   */
  bool Initialize(directx::DirectX12Device& device);
  /**
   * @brief �X�V
   */
  void Update();
  /**
   * @brief �f�X�g���N�^
   */
  void Draw(directx::DirectX12Device& device);
  /**
   * @brief ���W�̎擾
   */
  math::Vector3 GetPosition();
  /**
   * @brief ��]�ʂ̎擾
   */
  math::Vector3 GetRotation();
  /**
   * @brief �X�P�[���̎擾
   */
  math::Vector3 GetScale();
  /**
   * @brief ���a�̎擾
   */
  float GetRadius();
  /**
   * @brief ���W�̐ݒ�
   */
  void SetPosition(math::Vector3 position);
  /**
   * @brief ��]�ʂ̐ݒ�
   */
  void SetRotation(math::Vector3 rotate);
  /**
   * @brief �X�P�[���̐ݒ�
   */
  void SetScale(math::Vector3 scale);
  /**
   * @brief ���a�̎擾
   */
  void SetRadius(float radius);

 private:
  //!���S���W
  math::Vector3 position_;
  //!��]
  math::Vector3 rotation_;
  //!�X�P�[��
  math::Vector3 scale_;
  //!���a
  float radius_;

  const i32 u_max = 24;
  const i32 v_max = 12;

  directx::buffer::VertexBuffer vertex_buffer_;
  directx::buffer::IndexBuffer index_buffer_;
  directx::buffer::ConstantBuffer<Transform> transform_constant_buffer_;

  directx::buffer::ConstantBuffer<WorldContext> world_constant_buffer_;
  std::shared_ptr<directx::shader::RootSignature> root_signature_;
  directx::shader::GraphicsPipelineState pipeline_state_;
};

}  // namespace physics
}  // namespace legend

#endif  //! LEGEND_PHYSICS_SPHERE_H_
