#ifndef LEGEND_DIRECTX_DEVICE_SWAP_CHAIN_H_
#define LEGEND_DIRECTX_DEVICE_SWAP_CHAIN_H_

/**
 * @file swap_chain.h
 * @brief スワップチェインクラス定義

 */

#include "src/directx/render_target/render_target.h"
#include "src/directx/device/dxgi_adapter.h"
#include "src/window/window.h"

namespace legend {
namespace directx {
namespace device {
/**
 * @brief スワップチェインクラス
 */
class SwapChain {
  //! バックバッファの枚数
  static constexpr u32 FRAME_COUNT = 3;

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
   * @param command_queue コマンドキュー
   * @param format バックバッファのフォーマット
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(IDirectXAccessor& accessor, DXGIAdapter& adapter,
            window::Window& target_window, ID3D12CommandQueue* command_queue,
            DXGI_FORMAT format);
  /**
   * @brief バックバッファをセットする
   * @param accessor DirectX12アクセサ
   */
  void SetBackBuffer(IDirectXAccessor& accessor);
  /**
   * @brief バックバッファをクリアする
   * @param accessor DirectX12アクセサ
   */
  void ClearBackBuffer(IDirectXAccessor& accessor);
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

 public:
  /**
   * @brief 現在のレンダーターゲットを取得する
   */
  const buffer::RenderTarget& GetRenderTarget() const {
    return render_targets_[frame_index_];
  }
  /**
   * @brief 現在のフレームインデックスを取得する
   */
  u32 GetCurrentFrameIndex() const { return frame_index_; };
  /**
   * @brief フレームインデックスを更新する
   */
  void UpdateCurrentFrameIndex();

 private:
  //! スワップチェイン
  ComPtr<IDXGISwapChain3> swap_chain_;
  //! レンダーターゲット
  std::array<buffer::RenderTarget, FRAME_COUNT> render_targets_;
  //! 現在フレームのインデックス
  u32 frame_index_;
  //テアリングが許可されているか
  bool allow_tearing_;
};

}  // namespace device
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DEVICE_SWAP_CHAIN_H_
