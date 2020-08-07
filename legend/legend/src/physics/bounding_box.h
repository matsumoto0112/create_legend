#ifndef LEGEND_PHYSICS_BOUNDING_BOX_H_
#define LEGEND_PHYSICS_BOUNDING_BOX_H_

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/constant_buffer_structure.h"
#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/util/transform.h"

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
   * @brief �R���X�g���N�^
   * @param ���W
   * @param ��]
   * @param �X�P�[��
   */
  BoundingBox(math::Vector3 position, math::Quaternion rotation,
              math::Vector3 scale);
  /**
   * @brief �f�X�g���N�^
   */
  ~BoundingBox();
  /**
   * @brief ������
   * @param �f�o�C�X
   */
  bool Initialize();
  /**
   * @brief �X�V
   */
  void Update();
  /**
   * @brief �`��
   * @param �f�o�C�X
   */
  void Draw();
  /*
   * @brief �����x�N�g�����擾
   * @param ���ԍ�
   */
  math::Vector3 GetDirection(i32 direction_num) const;
  /**
   * @brief �������擾
   * @param ���ԍ�
   */
  float GetLength(i32 length_num) const;
  /**
   * @brief �X�P�[���{�̒������擾
   * @param ���ԍ�
   */
  float GetLengthByScale(i32 length_num) const;
  /**
   * @brief ���݂̈ʒu���擾
   */
  math::Vector3 GetPosition() const;
  /**
   * @brief ���݂̊p�x���擾
   */
  math::Quaternion GetRotation() const;
  /**
   * @brief ���݂̃X�P�[�����擾
   */
  math::Vector3 GetScale() const;
  /**
   * @brief ������X�̎擾
   */
  math::Vector3 GetAxisX() const;
  /**
   * @brief ������Y�̎擾
   */
  math::Vector3 GetAxisY() const;
  /**
   * @brief ������Z�̎擾
   */
  math::Vector3 GetAxisZ() const;
  /**
   * @brief �g���K�[����̎擾
   */
  bool GetIsTrigger() const;
  /**
   * @brief �ڒn����̎擾
   */
  bool GetOnGround() const;
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
  void SetRotation(math::Quaternion rotate);
  /**
   * @brief �g��k���̍X�V
   * @param �X�P�[��
   */
  void SetScale(math::Vector3 scale);
  /**
   * @brief �������̍X�V
   */
  void SetAxis();
  /**
   * @brief �g���K�[�ݒ�
   */
  void SetIsTrigger(bool trigger);
  /**
   * @brief �ڒn����̐ݒ�
   */
  void SetOnGround(bool is_ground);

 private:
  //! �g�����X�t�H�[��
  util::Transform transform_;
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

  //! �g���K�[���ǂ���
  bool is_trigger_;
  //! �ڒn����
  bool is_on_ground_;

  directx::buffer::VertexBuffer vertex_buffer_;
  directx::buffer::IndexBuffer index_buffer_;
  directx::buffer::ConstantBuffer<
      directx::buffer::constant_buffer_structure::Transform>
      transform_constant_buffer_;
};

}  // namespace physics
}  // namespace legend

#endif  //! LEGEND_PHYSICS_BOUNDING_BOX_H_
