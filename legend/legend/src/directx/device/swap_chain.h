#ifndef LEGEND_DIRECTX_DEVICE_SWAP_CHAIN_H_
#define LEGEND_DIRECTX_DEVICE_SWAP_CHAIN_H_

/**
 * @file swap_chain.h
 * @brief スワップチェイン
 */

#include "src/directx/buffer/render_target.h"
#include "src/directx/device/dxgi_adapter.h"
#include "src/window/window.h"

namespace legend {
namespace directx {
namespace device {
class SwapChain {
  static constexpr u32 FRAME_COUNT = 3;

 public:
  SwapChain();
  ~SwapChain();
  bool Init(DXGIAdapter& adapter, window::Window& target_window,
            ID3D12CommandQueue* command_queue, DXGI_FORMAT format);
  bool CreateRenderTarget(IDirectXAccessor& accessor);
  void SetBackBuffer(IDirectXAccessor& accessor);
  void DrawEnd(IDirectXAccessor& accessor);
  bool Present();

  const buffer::RenderTarget& GetRenderTarget() const {
    return render_targets_[frame_index_];
  }
  u32 GetCurrentFrameIndex() const { return frame_index_; };
  void UpdateCurrentFrameIndex();

 private:
  ComPtr<IDXGISwapChain3> swap_chain_;
  std::array<buffer::RenderTarget, FRAME_COUNT> render_targets_;
  u32 frame_index_;
};

}  // namespace device
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DEVICE_SWAP_CHAIN_H_
