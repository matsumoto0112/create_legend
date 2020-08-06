#ifndef LEGEND_DIRECTX_DEVICE_SWAP_CHAIN_H_
#define LEGEND_DIRECTX_DEVICE_SWAP_CHAIN_H_

/**
 * @file swap_chain.h
 * @brief スワップチェインクラス定義
 */

#include "src/directx/device/command_list.h"
#include "src/directx/device/directx_accessor.h"
#include "src/directx/device/dxgi_adapter.h"

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
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(IDirectXAccessor& accessor, DXGIAdapter& adapter, u32 frame_count,
            u32 width, u32 height, DXGI_FORMAT format, HWND hwnd,
            ID3D12CommandQueue* command_queue);

  bool Present();
  u32 GetCurrentBackBufferIndex() const;

 public:
  ComPtr<IDXGISwapChain3> swap_chain_;
  std::vector<ComPtr<ID3D12Resource>> render_targets_;

  D3D12_VIEWPORT viewport_;
  D3D12_RECT scissor_rect_;

  bool allow_tearing_;
};

}  // namespace device
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DEVICE_SWAP_CHAIN_H_
