#ifndef LEGEND_DIRECTX_BUFFER_RENDER_TARGET_TEXTURE_H_
#define LEGEND_DIRECTX_BUFFER_RENDER_TARGET_TEXTURE_H_

/**
 * @file render_target_texture.h
 * @brief テクスチャ使用可能なレンダーターゲット
 */

#include "src/directx/buffer/render_target.h"

namespace legend {
namespace directx {
namespace buffer {
class RenderTargetTexture {
 public:
  RenderTargetTexture();
  ~RenderTargetTexture();
  bool Init(IDirectXAccessor& accessor, u32 register_num, DXGI_FORMAT format,
            u32 width, u32 height, const util::Color4& clear_color,
            const std::wstring& name);

  void SetRenderTarget(IDirectXAccessor& accessor);
  void ClearRenderTarget(IDirectXAccessor& accessor) const;
  void DrawEnd(IDirectXAccessor& accessor);
  void SetToGlobalHeap(IDirectXAccessor& accessor) const;

 private:
  RenderTarget render_target_;
  DescriptorHandle srv_handle_;
  u32 register_num_;
};

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_BUFFER_RENDER_TARGET_TEXTURE_H_
