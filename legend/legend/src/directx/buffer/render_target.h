#ifndef LEGEND_DIRECTX_BUFFER_RENDER_TARGET_H_
#define LEGEND_DIRECTX_BUFFER_RENDER_TARGET_H_

/**
 * @file render_target.h
 * @brief レンダーターゲットクラス定義
 */

#include "src/directx/buffer/committed_resource.h"
#include "src/directx/descriptor_heap/descriptor_handle.h"
#include "src/directx/directx_accessor.h"
#include "src/util/color_4.h"

namespace legend {
namespace directx {
namespace buffer {
class RenderTarget {
 public:
  /**
   * @brief コンストラクタ
   */
  RenderTarget();
  /**
   * @brief デストラクタ
   */
  ~RenderTarget();
  bool Init(IDirectXAccessor& accessor, DXGI_FORMAT format, u32 width,
            u32 height, const util::Color4& clear_color,
            const std::wstring& name);
  bool InitFromBuffer(IDirectXAccessor& accessor, ComPtr<ID3D12Resource> buffer,
                      const util::Color4& clear_color);

  void SetRenderTarget(IDirectXAccessor& accessor);
  void ClearRenderTarget(IDirectXAccessor& accessor) const;
  void DrawEnd(IDirectXAccessor& accessor);
  ID3D12Resource* GetResource() const { return resource_.GetResource(); }

 public:
  CommittedResource resource_;
  DescriptorHandle rtv_handle_;
  util::Color4 clear_color_;
};

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_BUFFER_RENDER_TARGET_H_
