#ifndef LEGEND_DIRECTX_BUFFER_RENDER_TARGET_H_
#define LEGEND_DIRECTX_BUFFER_RENDER_TARGET_H_

/**
 * @file render_target.h
 * @brief レンダーターゲットクラス定義
 */

#include "src/directx/buffer/committed_resource.h"
#include "src/directx/directx_accessor.h"

namespace legend {
namespace directx {
namespace buffer {
class RenderTarget {
 public:
  RenderTarget();
  ~RenderTarget();
  bool Init(IDirectXAccessor& accessor, u32 width, u32 height,
            DXGI_FORMAT format, ID3D12DescriptorHeap* heap, u32 index,
            const std::wstring& name);
  void SetGraphicsCommandList(IDirectXAccessor& accessor) const;

  void EndDraw(IDirectXAccessor& accessor);
  ID3D12Resource* GetResource() const { return resource_.Get(); }

 private:
  ComPtr<ID3D12Resource> resource_;
  D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle_;
  D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle_;
};

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_BUFFER_RENDER_TARGET_H_
