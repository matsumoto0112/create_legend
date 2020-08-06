#ifndef LEGEND_DIRECTX_DEVICE_DIRECTX_ACCESSOR_H_
#define LEGEND_DIRECTX_DEVICE_DIRECTX_ACCESSOR_H_

/**
 * @file directx_accessor.h
 */

#include "src/directx/descriptor_heap/descriptor_handle.h"
#include "src/directx/shader/shader_resource_type.h"

namespace legend {
namespace directx {
namespace device {

/**
 * @brief DirectXアクセサ関数インターフェース
 */
class IDirectXAccessor {
 public:
  /**
   * @brief デストラクタ
   */
  virtual ~IDirectXAccessor() = default;
  /**
   * @brief デバイスの取得
   */
  virtual ID3D12Device* GetDevice() const = 0;

  virtual descriptor_heap::DescriptorHandle GetRTVHandle() = 0;
  virtual descriptor_heap::DescriptorHandle GetDSVHandle() = 0;

  virtual void RegisterHandle(u32 register_num, shader::ResourceType type,
                              descriptor_heap::DescriptorHandle handle) = 0;
};

}  // namespace device
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DEVICE_DIRECTX_ACCESSOR_H_
