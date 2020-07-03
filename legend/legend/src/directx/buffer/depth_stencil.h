#ifndef LEGEND_DIRECTX_BUFFER_DEPTH_STENCIL_H_
#define LEGEND_DIRECTX_BUFFER_DEPTH_STENCIL_H_

/**
 * @file depth_stencil.h
 * @brief �f�v�X�E�X�e���V���N���X��`
 */

#include "src/directx/buffer/committed_resource.h"
#include "src/directx/descriptor_heap/descriptor_handle.h"
#include "src/directx/directx_accessor.h"

namespace legend {
namespace directx {
namespace buffer {
/**
 * @brief �f�v�X�E�X�e���V���N���X
 */
class DepthStencil {
public:
  struct ClearValue {
    float depth;
    u8 stencil;

    ClearValue(float depth = 1.0f, u8 stencil = 0)
        : depth(depth), stencil(stencil) {}
  };

 public:
  DepthStencil();
  ~DepthStencil();
  bool Init(IDirectXAccessor& accessor, DXGI_FORMAT format, u32 width,
            u32 height, const ClearValue& clear_value,
            const std::wstring& name);

  void ClearDepthStencil(IDirectXAccessor& accessor) const;
  void PrepareToSetCommandList(IDirectXAccessor& accessor);
  /**
   * @brief CPU�n���h�����擾����
   * @details RTV�̃Z�b�g���ɓ�����DSV���Z�b�g���邽�߁ARTV�����Ɍ��J
   */
  D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle() const {
    return handle_.cpu_handle_;
  }

 private:
  legend::directx::buffer::CommittedResource resource_;
  legend::directx::DescriptorHandle handle_;
  ClearValue clear_value_;
};

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_BUFFER_DEPTH_STENCIL_H_
