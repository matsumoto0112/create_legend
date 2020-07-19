#ifndef LEGEND_DIRECTX_BUFFER_RENDER_TARGET_H_
#define LEGEND_DIRECTX_BUFFER_RENDER_TARGET_H_

/**
 * @file render_target.h
 * @brief レンダーターゲットクラス定義
 */

#include "src/directx/buffer/committed_resource.h"
#include "src/directx/buffer/depth_stencil.h"
#include "src/directx/descriptor_heap/descriptor_handle.h"
#include "src/directx/directx_accessor.h"
#include "src/util/color_4.h"

namespace legend {
namespace directx {
namespace buffer {
/**
 * @class RenderTarget
 * @brief レンダーターゲット
 */
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
  /**
   * @brief 初期化
   * @param accessor DirextX12アクセサ
   * @param format フォーマット
   * @param width ターゲット幅
   * @param height ターゲット高さ
   * @param clear_color バッファのクリア色
   * @param name リソース名
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(IDirectXAccessor& accessor, DXGI_FORMAT format, u32 width,
            u32 height, const util::Color4& clear_color,
            const std::wstring& name);
  /**
   * @brief バッファから初期化する
   * @param accessor DirextX12アクセサ
   * @param buffer もととなるバッファ
   * @param clear_color バッファのクリア色
   * @param name リソース名
   * @return 初期化に成功したらtrueを返す
   * @detials バックバッファに基本的に使う
   */
  bool InitFromBuffer(IDirectXAccessor& accessor, ComPtr<ID3D12Resource> buffer,
                      const util::Color4& clear_color,
                      const std::wstring& name);
  /**
   * @brief デプス・ステンシルを作成する
   * @param accessor DirextX12アクセサ
   * @param format フォーマット
   * @param width 幅
   * @param height 高さ
   * @param clear_value 初期化値
   * @param name リソース名
   * @return 作成に成功したらtrueを返す
   */
  bool CreateDepthStencil(IDirectXAccessor& accessor, DXGI_FORMAT format,
                          u32 width, u32 height,
                          const DepthStencil::ClearValue& clear_value,
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
   * @brief 状態を遷移させる
   * @param accessor DirextX12アクセサ
   * @param next_state 次の状態
   */
  void Transition(IDirectXAccessor& accessor, D3D12_RESOURCE_STATES next_state);
  /**
   * @brief パイプラインステートデスクにRTV情報を書き込む
   * @param pipeline_state_desc 書き込む対象
   */
  void WriteInfoToPipelineStateDesc(
      D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipeline_state_desc,
      bool write_with_depth_stencil) const;

 public:
  /**
   * @brief リソースを取得する
   */
  ID3D12Resource* GetResource() const { return resource_.GetResource(); }
  /**
   * @brief ハンドルを取得する
   */
  DescriptorHandle GetHandle() const { return rtv_handle_; }

 private:
  //! リソース
  CommittedResource resource_;
  //! レンダーターゲットハンドル
  DescriptorHandle rtv_handle_;
  //! バッファのクリア色
  util::Color4 clear_color_;
  //! レンダーターゲットのフォーマット
  DXGI_FORMAT format_;
  //! デプス・ステンシル
  std::unique_ptr<DepthStencil> depth_stencil_;
};

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_BUFFER_RENDER_TARGET_H_
