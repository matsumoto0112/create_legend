#ifndef LEGEND_DIRECTX_BUFFER_MULTI_RENDER_TARGET_TEXTURE_H_
#define LEGEND_DIRECTX_BUFFER_MULTI_RENDER_TARGET_TEXTURE_H_

/**
 * @file multi_render_target_texture.h
 * @brief テクスチャ利用可能なマルチレンダーターゲットクラス定義
 */

#include "src/directx/buffer/render_target_texture.h"
#include "src/directx/directx_accessor.h"

namespace legend {
namespace directx {
namespace buffer {
class MultiRenderTargetTexture {
 public:
  struct RenderTargetCreateInfo {
    u32 register_num;
    util::Color4 clear_color;
  };

 public:
  MultiRenderTargetTexture();
  ~MultiRenderTargetTexture();
  bool Init(IDirectXAccessor& accessor, DXGI_FORMAT format, u32 width,
            u32 height, const std::vector<RenderTargetCreateInfo>& infos,
            const std::wstring& name_base);
  void WriteInfoToPipelineStateDesc(
      D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipeline_state_desc) const;
  /**
   * @brief レンダーターゲットにセットする
   * @param accessor DirextX12アクセサ
   */
  void SetRenderTarget(IDirectXAccessor& accessor);
  /**
   * @brief レンダーターゲットの色をクリアする
   * @param accessor DirextX12アクセサ
   * @details レンダーターゲットにセットされていないときは無効
   */
  void ClearRenderTarget(IDirectXAccessor& accessor) const;
  /**
   * @brief 描画終了
   * @param accessor DirextX12アクセサ
   */
  void DrawEnd(IDirectXAccessor& accessor);
  /**
   * @brief テクスチャをSRVとしてグローバルヒープにセットする
   * @param accessor DirextX12アクセサ
   */
  void SetToGlobalHeap(IDirectXAccessor& accessor) const;

 private:
  //! リソース
  std::vector<CommittedResource> render_target_resources_;
  std::vector<RenderTargetCreateInfo> infos_;
  std::vector<DescriptorHandle> handles_;
  std::vector<DescriptorHandle> srv_handles_;
};

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_BUFFER_MULTI_RENDER_TARGET_TEXTURE_H_
