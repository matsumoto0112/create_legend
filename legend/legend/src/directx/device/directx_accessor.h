#ifndef LEGEND_DIRECTX_DEVICE_DIRECTX_ACCESSOR_H_
#define LEGEND_DIRECTX_DEVICE_DIRECTX_ACCESSOR_H_

/**
 * @file directx_accessor.h
 */

#include "src/directx/descriptor_heap/descriptor_handle.h"

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

  virtual descriptor_heap::DescriptorHandle GetRTVHandle() = 0;
  virtual descriptor_heap::DescriptorHandle GetDSVHandle() = 0;
};

}  // namespace device
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DEVICE_DIRECTX_ACCESSOR_H_
