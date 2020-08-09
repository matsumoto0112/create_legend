#ifndef LEGEND_DIRECTX_DEVICE_SWAP_CHAIN_H_
#define LEGEND_DIRECTX_DEVICE_SWAP_CHAIN_H_

/**
 * @file swap_chain.h
 * @brief スワップチェインクラス定義
 */

#include "src/directx/device/command_list.h"
#include "src/directx/device/directx_accessor.h"
#include "src/directx/device/dxgi_adapter.h"
#include "src/directx/render_target/render_target.h"

namespace legend {
namespace directx {
namespace device {
/**
 * @class SwapChain
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
   * @param
   * @return 初期化に成功したらtrueを返す
   */

  /**
   * @brief 初期化
   * @param accessor DirectXデバイスアクセサ
   * @param adapter アダプター
   * @param frame_count バックバッファのフレーム数
   * @param width 画面の幅
   * @param height 画面の高さ
   * @param format バックバッファのフォーマット
   * @param hwnd ウィンドウハンドル
   * @param command_queue コマンドキュー
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(IDirectXAccessor& accessor, DXGIAdapter& adapter, u32 frame_count,
            u32 width, u32 height, DXGI_FORMAT format, HWND hwnd,
            ID3D12CommandQueue* command_queue);
  /**
   * @brief バックバッファを取得する
   * @param index バックバッファのインデックス
   */
  ComPtr<ID3D12Resource> GetBuffer(u32 index) const;
  /**
   * @brief 描画内容を表示する
   * @return 成功したらtrueを返す
   */
  bool Present();
  /**
   * @brief 現在のバックバッファのインデックスを取得する
   */
  u32 GetCurrentBackBufferIndex() const;

 public:
  //! スワップチェイン
  ComPtr<IDXGISwapChain3> swap_chain_;
  //! テアリングが許可されているか
  bool allow_tearing_;
};

}  // namespace device
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DEVICE_SWAP_CHAIN_H_
