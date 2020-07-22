#ifndef LEGEND_PHYSICS_BOUNDING_BOX_H_
#define LEGEND_PHYSICS_BOUNDING_BOX_H_

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/game/game_device.h"
#include "src/math/matrix_4x4.h"

namespace legend {
namespace physics {

struct Transform {
  math::Matrix4x4 world;
};
struct WorldContext {
    math::Matrix4x4 view;
    math::Matrix4x4 projection;
};

/**
 * @class BoundingBox
 * @brief �����̂̃N���X
 */

class BoundingBox {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  BoundingBox();
  /**
   * @brief �f�X�g���N�^
   */
  ~BoundingBox();
  /**
   * @brief ������
   * @param �f�o�C�X
   */
  bool Initialize(directx::DirectX12Device& device);
  /**
   * @brief �X�V
   */
  void Update();
  /**
   * @brief �`��
   * @param �f�o�C�X
   */
  void Draw(directx::DirectX12Device& device);
  /*
   * @brief �����x�N�g�����擾
   * @param ���ԍ�
   */
  math::Vector3 GetDirection(i32 direction_num);
  /**
   * @brief �������擾
   * @param ���ԍ�
   */
  float GetLength(i32 length_num);
  /**
   * @brief ���݂̈ʒu���擾
   */
  math::Vector3 GetPosition();
  /**
   * @brief ���݂̊p�x���擾
   */
  math::Vector3 GetRotation();
  /**
   * @brief ���݂̃X�P�[�����擾
   */
  math::Vector3 GetScale();
  /**
   * @brief ������X�̎擾
   */
  math::Vector3 GetAxisX();
  /**
   * @brief ������Y�̎擾
   */
  math::Vector3 GetAxisY();
  /**
   * @brief ������Z�̎擾
   */
  math::Vector3 GetAxisZ();
  /**
   * @brief �e�����x�N�g���̐ݒ�
   * @param X����
   * @param Y����
   * @param Z����
   */
  void SetDirection(math::Vector3 direction_x, math::Vector3 direction_y,
                    math::Vector3 direction_z);
  /**
   * @brief �e�����̒����̐ݒ�
   * @param X�����̒���
   * @param Y�����̒���
   * @param Z�����̒���
   */
  void SetLength(float length_x, float length_y, float length_z);
  /**
   * @brief ���S���W�̍X�V
   * @param ���W
   */
  void SetPosition(math::Vector3 position);
  /**
   * @brief ��]�p�x�̍X�V
   * @param �p�x
   */
  void SetRotation(math::Vector3 rotate);
  /**
   * @brief �g��k���̍X�V
   * @param �X�P�[��
   */
  void SetScale(math::Vector3 scale);
  /**
   * @brief �������̍X�V
   */
  void SetAxis();

 private:
  //!���S���W
  math::Vector3 position_;
  //!�p�x
  math::Vector3 rotation_;
  //!�X�P�[��
  math::Vector3 scale_;
  //!�����x�N�g��
  std::vector<math::Vector3> directions_;
  //!�e�������̒���(���a)
  std::vector<float> lengthes_;
  //!������X
  math::Vector3 axis_x;
  //!������Y
  math::Vector3 axis_y;
  //!������Z
  math::Vector3 axis_z;

  directx::buffer::VertexBuffer vertex_buffer_;
  directx::buffer::IndexBuffer index_buffer_;
  directx::buffer::ConstantBuffer<Transform> transform_constant_buffer_;

  directx::buffer::ConstantBuffer<WorldContext> world_constant_buffer_;
  std::shared_ptr<directx::shader::RootSignature> root_signature_;
  directx::shader::GraphicsPipelineState pipeline_state_;
};

}  // namespace physics
}  // namespace legend

#endif  //! LEGEND_PHYSICS_BOUNDING_BOX_H_
