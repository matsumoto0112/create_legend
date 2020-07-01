#ifndef LEGEND_DIRECTX_BUFFER_RENDER_TARGET_H_
#define LEGEND_DIRECTX_BUFFER_RENDER_TARGET_H_

/**
 * @file render_target.h
 * @brief レンダーターゲットクラス定義
 */

#include "src/directx/buffer/committed_resource.h"
#include "src/directx/directx_accessor.h"
#include "src/directx/heap_manager.h"

namespace legend {
namespace directx {
namespace buffer {
class RenderTarget {
 public:
  RenderTarget();
  ~RenderTarget();
  bool Init(IDirectXAccessor& accessor, DXGI_FORMAT format, u32 width,
            u32 height, const std::wstring& name);
  bool InitFromBuffer(IDirectXAccessor& accessor,
                      ComPtr<ID3D12Resource> buffer);
  void CreateShaderResourceView(IDirectXAccessor& accessor);
  void SetGraphicsCommandList(IDirectXAccessor& accessor) const;

  void Transition(IDirectXAccessor& accessor, D3D12_RESOURCE_STATES next_state);
  void EndDraw(IDirectXAccessor& accessor);
  ID3D12Resource* GetResource() const { return resource_.GetResource(); }

 public:
  CommittedResource resource_;
  DescriptorHandle rtv_handle_;
  DescriptorHandle srv_handle_;
};

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_BUFFER_RENDER_TARGET_H_
