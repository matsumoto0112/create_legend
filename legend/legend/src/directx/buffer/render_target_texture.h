#ifndef LEGEND_DIRECTX_BUFFER_RENDER_TARGET_TEXTURE_H_
#define LEGEND_DIRECTX_BUFFER_RENDER_TARGET_TEXTURE_H_

/**
 * @file render_target_texture.h
 * @brief テクスチャ使用可能なレンダーターゲットクラス定義
 */

#include "src/directx/buffer/render_target.h"

namespace legend {
namespace directx {
namespace buffer {
/**
 * @class RenderTargetTexture
 * @brief テクスチャ使用可能なレンダーターゲット
 */
class RenderTargetTexture {
 public:
  /**
   * @brief コンストラクタ
   */
  RenderTargetTexture();
  /**
   * @brief デストラクタ
   */
  ~RenderTargetTexture();
  /**
   * @brief 初期化
   * @param accessor DirectX12アクセサ
   * @param register_num シェーダーのレジスター番号
   * @param format テクスチャフォーマット
   * @param width テクスチャ幅
   * @param height テクスチャ高さ
   * @param clear_color レンダーターゲットのクリア色
   * @param name リソース名
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(IDirectXAccessor& accessor, u32 register_num, DXGI_FORMAT format,
            u32 width, u32 height, const util::Color4& clear_color,
            const std::wstring& name);
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
  //! レンダーターゲット
  RenderTarget render_target_;
  //! SRVハンドル
  DescriptorHandle srv_handle_;
  //! シェーダーのレジスター番号
  u32 register_num_;
};

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_BUFFER_RENDER_TARGET_TEXTURE_H_
