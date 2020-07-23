#ifndef LEGEND_DIRECTX_DIRECTX_ACCESSOR_H_
#define LEGEND_DIRECTX_DIRECTX_ACCESSOR_H_

/**
 * @file directx_accessor.h
 * @brief DirectX�n�̃A�N�Z�T�֐��C���^�[�t�F�[�X��`
 */

#include "src/directx/descriptor_heap/descriptor_handle.h"
#include "src/directx/descriptor_heap/descriptor_heap_type.h"
#include "src/directx/descriptor_heap/heap_parameter.h"
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

  virtual descriptor_heap::DescriptorHandle GetLocalHeapHandle(
      descriptor_heap::heap_parameter::LocalHeapID id) = 0;
  virtual descriptor_heap::DescriptorHandle GetRTVHandle() = 0;
  virtual descriptor_heap::DescriptorHandle GetDSVHandle() = 0;
  virtual void SetToGlobalHeap(
      u32 register_num, ResourceType resource_type,
      const descriptor_heap::DescriptorHandle& handle) = 0;

  virtual void SetBackBuffer(IDirectXAccessor& accessor) = 0;
  virtual void ClearBackBufferTarget(IDirectXAccessor& accessor) = 0;
  virtual descriptor_heap::DescriptorHandle GetBackBufferHandle() const = 0;
  virtual DXGI_FORMAT GetBackBufferFormat() const = 0;
};
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DIRECTX_ACCESSOR_H_
