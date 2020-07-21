#ifndef LEGEND_DIRECTX_RENDER_TARGET_RENDER_TARGET_TEXTURE_H_
#define LEGEND_DIRECTX_RENDER_TARGET_RENDER_TARGET_TEXTURE_H_

/**
 * @file render_target_texture.h
 * @brief テクスチャ使用可能なレンダーターゲットクラス定義
 */

#include "src/directx/render_target/render_target.h"
#include "src/directx/shader/graphics_pipeline_state.h"

namespace legend {
namespace directx {
namespace render_target {
/**
 * @class RenderTargetTexture
 * @brief テクスチャ使用可能なレンダーターゲット
 */
class MultiRenderTargetTexture {
  struct RenderTargetTexture {
    u32 register_num;
    RenderTarget render_target;
    DescriptorHandle srv_handle;
  };

 public:
  struct Info {
    u32 register_num;
    DXGI_FORMAT format;
    u32 width;
    u32 height;
    util::Color4 clear_color;
    std::wstring name;
  };

 public:
  /**
   * @brief コンストラクタ
   */
  MultiRenderTargetTexture();
  /**
   * @brief デストラクタ
   */
  ~MultiRenderTargetTexture();
  /**
   * @brief 初期化
   * @param accessor DirectX12アクセサ
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(IDirectXAccessor& accessor, const Info& info);
  bool Init(IDirectXAccessor& accessor, const std::vector<Info>& infos);
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
  void SetToGlobalHeap(IDirectXAccessor& accessor,
                       u32 render_target_number) const;
  void WriteInfoToPipelineDesc(shader::GraphicsPipelineState& pipeline);

  void PrepareToUseRenderTarget(IDirectXAccessor& accessor);
  std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> GetRTVHandles() const;

 private:
  std::vector<RenderTargetTexture> render_targets_;
};

}  // namespace render_target
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_RENDER_TARGET_RENDER_TARGET_TEXTURE_H_
