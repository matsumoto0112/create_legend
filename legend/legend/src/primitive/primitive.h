#ifndef LEGEND_PRIMITIVE_PRIMITIVE_H_
#define LEGEND_PRIMITIVE_PRIMITIVE_H_

/**
 * @file primitive.h
 * @brief �v���~�e�B�u�`����N���X��`
 */

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/constant_buffer_structure.h"
#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/directx/device/command_list.h"
#include "src/directx/vertex.h"
#include "src/util/transform.h"

namespace legend {
namespace primitive {

/**
 * @brief �v���~�e�B�u�`����N���X
 */
class PrimitiveBase {
 protected:
  using TransformStruct = directx::buffer::constant_buffer_structure::Transform;

 public:
  /**
   * @brief �R���X�g���N�^
   * @param name �v���~�e�B�u��
   */
  PrimitiveBase(const std::wstring& name);
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~PrimitiveBase();
  /**
   * @brief ������
   * @return �������ɐ���������true��Ԃ�
   */
  virtual bool Init() = 0;
  /**
   * @brief �`��
   * @param command_list �R�}���h���X�g
   */
  virtual void Render(directx::device::CommandList& command_list);

  /**
   * @brief �g�����X�t�H�[����ݒ肷��
   */
  inline void SetTransform(const util::Transform& transform) {
    this->transform_ = transform;
  }
  /**
   * @brief �g�����X�t�H�[�����擾����
   */
  inline util::Transform GetTransform() const { return this->transform_; }

 protected:
  /**
   * @brief �o�b�t�@�̏�����
   * @param vertices ���_�z��
   * @param indices �C���f�b�N�X�z��
   * @return �������ɐ���������true��Ԃ�
   */
  virtual bool InitBuffer(const std::vector<directx::PhysicsVertex>& vertices,
                          const std::vector<u16>& indices);

 protected:
  //! �v���~�e�B�u��
  std::wstring name_;
  //! ���_�o�b�t�@
  directx::buffer::VertexBuffer vertex_buffer_;
  //! �C���f�b�N�X�o�b�t�@
  directx::buffer::IndexBuffer index_buffer_;
  //! �g�����X�t�H�[��
  util::Transform transform_;
  //! �g�����X�t�H�[���R���X�^���g�o�b�t�@
  directx::buffer::ConstantBuffer<TransformStruct> transform_cb_;
};

}  // namespace primitive
}  // namespace legend
#endif  //! LEGEND_PRIMITIVE_PRIMITIVE_H_
