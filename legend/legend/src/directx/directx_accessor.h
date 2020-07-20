#ifndef LEGEND_DIRECTX_DIRECTX_ACCESSOR_H_
#define LEGEND_DIRECTX_DIRECTX_ACCESSOR_H_

/**
 * @file directx_accessor.h
 * @brief DirectX�n�̃A�N�Z�T�֐��C���^�[�t�F�[�X��`
 */

#include "src/directx/descriptor_heap/descriptor_handle.h"
#include "src/directx/descriptor_heap/descriptor_heap_type.h"
#include "src/directx/resource_type.h"

namespace legend {
namespace directx {

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
   * @brief �R�}���h���X�g�̎擾
   */
  virtual ID3D12GraphicsCommandList4* GetCommandList() const = 0;

  virtual DescriptorHandle GetHandle(DescriptorHeapType heap_type) = 0;

  virtual void SetToGlobalHeap(u32 register_num, ResourceType resource_type,
                               const DescriptorHandle& handle) = 0;

  virtual void ClearBackBufferTarget(IDirectXAccessor& accessor) = 0;
  virtual DescriptorHandle GetBackBufferHandle() const = 0;
};
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DIRECTX_ACCESSOR_H_
