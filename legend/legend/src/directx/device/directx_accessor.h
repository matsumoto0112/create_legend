#ifndef LEGEND_DIRECTX_DEVICE_DIRECTX_ACCESSOR_H_
#define LEGEND_DIRECTX_DEVICE_DIRECTX_ACCESSOR_H_

/**
 * @file directx_accessor.h
 * @brief DirectX�A�N�Z�T�֐���`
 */

#include "src/directx/descriptor_heap/descriptor_handle.h"
#include "src/directx/descriptor_heap/heap_parameter.h"
#include "src/directx/shader/shader_resource_type.h"

namespace legend {
namespace directx {
namespace device {

/**
 * @brief DirectX�A�N�Z�T�֐��C���^�[�t�F�[�X
 */
class IDirectXAccessor {
 public:
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~IDirectXAccessor() = default;
  /**
   * @brief �f�o�C�X�̎擾
   */
  virtual ID3D12Device* GetDevice() const = 0;
  /**
   * @brief �����_�[�^�[�Q�b�g�̃n���h�����擾����
   */
  virtual descriptor_heap::DescriptorHandle GetRTVHandle() = 0;
  /**
   * @brief �f�v�X�E�X�e���V���̃n���h�����擾����
   */
  virtual descriptor_heap::DescriptorHandle GetDSVHandle() = 0;
  /**
   * @brief ���[�J���n���h�����擾����
   * @param heap_id �g�p���郍�[�J���n���h����ID
   */
  virtual descriptor_heap::DescriptorHandle GetLocalHandle(
      descriptor_heap::heap_parameter::LocalHeapID heap_id) = 0;
  /**
   * @brief �n���h�����O���[�o���ɓo�^����
   * @param register_num �V�F�[�_�[�̃��W�X�^�[�ԍ�
   * @param type ���\�[�X�̎��
   * @param handle �o�^����n���h��
   */
  virtual void RegisterHandle(u32 register_num, shader::ResourceType type,
                              descriptor_heap::DescriptorHandle handle) = 0;
};

}  // namespace device
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DEVICE_DIRECTX_ACCESSOR_H_
