#ifndef LEGEND_PHYSICS_BOUNDING_BOX_H_
#define LEGEND_PHYSICS_BOUNDING_BOX_H_

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/game/game_device.h"
#include "src/math/matrix_4x4.h"
#include "src/util/loader/glb_loader.h"

namespace legend {
namespace physics {

struct Transform {
  math::Matrix4x4 world;
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
   * @brief ������
   * @param �f�o�C�X
   * @param �I�u�W�F�N�g�l�[��
   * @param GLBLoader
   */
  bool Initialize(directx::DirectX12Device& device, const std::wstring name,
                  util::loader::GLBLoader& loader);
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

 private:
  //!���S���W
  math::Vector3 position_;
  //!�����x�N�g��
  std::vector<math::Vector3> directions_;
  //!�e�������̒���
  std::vector<float> lengthes_;

  directx::buffer::VertexBuffer vertex_buffer_;
  directx::buffer::IndexBuffer index_buffer_;
  directx::buffer::ConstantBuffer<Transform> transform_constant_buffer_;
};

}  // namespace physics
}  // namespace legend

#endif  //! LEGEND_PHYSICS_BOUNDING_BOX_H_
