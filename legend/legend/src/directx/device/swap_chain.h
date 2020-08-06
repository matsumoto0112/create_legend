#ifndef LEGEND_DIRECTX_DEVICE_SWAP_CHAIN_H_
#define LEGEND_DIRECTX_DEVICE_SWAP_CHAIN_H_

/**
 * @file swap_chain.h
 * @brief スワップチェインクラス定義
 */

#include "src/directx/device/dxgi_adapter.h"
#include "src/directx/render_target/render_target.h"
#include "src/window/window.h"

namespace legend {
namespace directx {
namespace device {
/**
 * @brief スワップチェインクラス
 */
class SwapChain {
 public:
  /**
   * @brief コンストラクタ
   */
  SwapChain();
  /**
   * @brief デストラクタ
   */
  ~SwapChain();
  /**
   * @brief 初期化
   * @param accessor DirectX12アクセサ
   * @param apapter アダプター
   * @param target_window 描画対象のウィンドウk
   * @param format バックバッファのフォーマット
   * @param back_buffer_count バックバッファの枚数
   * @param command_queue コマンドキュー
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(ID3D12Device& accessor, DXGIAdapter& adapter,
            window::Window& target_window, DXGI_FORMAT format,
            u32 back_buffer_count, ID3D12CommandQueue* command_queue);
  /**
   * @brief バックバッファをクリアする
   * @param accessor DirectX12アクセサ
   */
  void ClearBackBuffer(IDirectXAccessor& accessor);
  /**
   * @brief 描画開始
   * @param accessor DirectX12アクセサ
   */
  void DrawBegin(IDirectXAccessor& accessor);
  /**
   * @brief 描画終了
   * @param accessor DirectX12アクセサ
   */
  void DrawEnd(IDirectXAccessor& accessor);
  /**
   * @brief バックバッファを表示する
   * @return 表示に成功したらtrueを返す
   */
  bool Present();
  /**
   * @brief ビューポートをセットする
   * @param accessor DirectX12アクセサ
   */
  void SetViewport(IDirectXAccessor& accessor) const;
  /**
   * @brief シザー矩形をセットする
   * @param accessor DirectX12アクセサ
   */
  void SetScissorRect(IDirectXAccessor& accessor) const;

 public:
  /**
   * @brief 現在のレンダーターゲットを取得する
   */
  const render_target::RenderTarget& GetRenderTarget() const {
    return render_targets_[GetCurrentFrameIndex()];
  }
  /**
   * @brief 現在のフレームインデックスを取得する
   */
  u32 GetCurrentFrameIndex() const;

 private:
  //! スワップチェイン
  ComPtr<IDXGISwapChain3> swap_chain_;
  //! レンダーターゲット
  std::vector<render_target::RenderTarget> render_targets_;
  //テアリングが許可されているか
  bool allow_tearing_;
  D3D12_VIEWPORT viewport_;
  D3D12_RECT scissor_rect_;
};

}  // namespace device
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DEVICE_SWAP_CHAIN_H_
