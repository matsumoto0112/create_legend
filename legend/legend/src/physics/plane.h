#ifndef LEGEND_PHYSICS_PLANE_H_
#define LEGEND_PHYSICS_PLANE_H_

#include "src/physics/bounding_box.h"

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
    math::Vector3 GetPosition();
    /**
     * @brief �@���x�N�g���̎擾
     */
    math::Vector3 GetNormal();
private:
    //! �ʒu
    math::Vector3 position_;
    //! �@���x�N�g��
    math::Vector3 normal_;

    directx::buffer::VertexBuffer vertex_buffer_;
    directx::buffer::IndexBuffer index_buffer_;
    directx::buffer::ConstantBuffer<Transform> transform_constant_buffer_;
};

}  // namespace physics
}  // namespace legend

#endif  //! LEGEND_PHYSICS_PLANE_H_
